// GUI acceptance tests ("UAT"): drive the real application widgets the way a
// user would — start the app, load every example, click on the canvas, run
// every simulation type — and fail on any crash or broken behavior.
//
// Runs headless with QT_QPA_PLATFORM=offscreen (set by CTest); screenshots of
// the real UI are saved next to the test binary under uat-artifacts/.

#include <gtest/gtest.h>

#include <QApplication>
#include <QComboBox>
#include <QDir>
#include <QImage>
#include <QPushButton>
#include <QElapsedTimer>
#include <QSpinBox>
#include <QTemporaryDir>
#include <QTest>
#include <QTimer>

#include "networkdesigner.h"
#include "designplan.h"
#include "Network.h"

namespace {

QString examplesDir() {
    const QByteArray env = qgetenv("EXAMPLES_DIR");
    if (!env.isEmpty()) return QString::fromLocal8Bit(env);
    return QStringLiteral(EXAMPLES_DIR_DEFAULT);
}

void pump(int times = 3) {
    for (int i = 0; i < times; ++i)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
}

QString artifactsDir() {
    QDir dir(QCoreApplication::applicationDirPath() + "/uat-artifacts");
    dir.mkpath(".");
    return dir.absolutePath();
}

int totalSynapses(Network* net) {
    int total = 0;
    for (int i = 0; i < net->getNbNeurons(); ++i)
        total += net->getNeuron(i)->getNb_neighbors();
    return total;
}

} // namespace

// ── 1. The startup path (the one that crashes on Android) ───────────────────

TEST(UatSmoke, MainWindowStartsShowsAndRenders) {
    NetworkDesigner w;
    w.resize(1280, 800);
    w.show();
    pump();

    ASSERT_TRUE(w.isVisible());

    const QImage shot = w.grab().toImage();
    ASSERT_FALSE(shot.isNull());
    EXPECT_GT(shot.width(), 100);
    EXPECT_GT(shot.height(), 100);
    shot.save(artifactsDir() + "/01-startup.png");
}

// ── 2. Every shipped example must load and render ───────────────────────────

TEST(UatSmoke, LoadsEveryExampleNetwork) {
    NetworkDesigner w;
    w.resize(1280, 800);
    w.show();
    pump();

    QDir ex(examplesDir());
    const QStringList files = ex.entryList({"*.nml"}, QDir::Files, QDir::Name);
    ASSERT_FALSE(files.isEmpty()) << "no .nml examples found in "
                                  << examplesDir().toStdString();

    for (const QString& f : files) {
        w.loadFile(ex.absoluteFilePath(f));
        pump();

        ASSERT_NE(w.getNetwork(), nullptr) << f.toStdString();
        EXPECT_GT(w.getNetwork()->getNbNeurons(), 0)
            << "loaded an empty network from " << f.toStdString();

        const QImage shot = w.grab().toImage();
        ASSERT_FALSE(shot.isNull()) << f.toStdString();
        shot.save(artifactsDir() + "/02-loaded-" + f + ".png");
    }
}

// ── 3. Save → reload round trip preserves the network ────────────────────────

TEST(UatSmoke, SaveReloadRoundTrip) {
    QTemporaryDir tmp;
    ASSERT_TRUE(tmp.isValid());

    DesignPlan a(nullptr);
    a.load(examplesDir() + "/CardioRespiratoire.nml");
    const int neurons = a.getNetwork()->getNbNeurons();
    const int synapses = totalSynapses(a.getNetwork());
    ASSERT_GT(neurons, 0);

    const QString path = tmp.filePath("roundtrip.nml");
    a.save(path);

    DesignPlan b(nullptr);
    b.load(path);
    EXPECT_EQ(b.getNetwork()->getNbNeurons(), neurons);
    EXPECT_EQ(totalSynapses(b.getNetwork()), synapses);
}

// ── 4. Canvas interaction: create, connect, select, delete ──────────────────

TEST(UatSmoke, CanvasCreateConnectAndDeleteNeurons) {
    DesignPlan dp(nullptr);
    dp.resize(400, 300);
    dp.show();
    pump();

    // Left click on empty canvas creates a neuron
    QTest::mouseClick(&dp, Qt::LeftButton, Qt::NoModifier, QPoint(100, 100));
    pump(1);
    ASSERT_EQ(dp.getNetwork()->getNbNeurons(), 1);

    QTest::mouseClick(&dp, Qt::LeftButton, Qt::NoModifier, QPoint(220, 160));
    pump(1);
    ASSERT_EQ(dp.getNetwork()->getNbNeurons(), 2);

    // Right-press on the second neuron, release on the first → synapse
    QTest::mousePress(&dp, Qt::RightButton, Qt::NoModifier, QPoint(220, 160));
    QTest::mouseRelease(&dp, Qt::RightButton, Qt::NoModifier, QPoint(100, 100));
    pump(1);
    EXPECT_EQ(totalSynapses(dp.getNetwork()), 1);

    // Select the first neuron and delete it
    QTest::mouseClick(&dp, Qt::LeftButton, Qt::NoModifier, QPoint(100, 100));
    QTest::keyClick(&dp, Qt::Key_Delete);
    pump(1);
    EXPECT_EQ(dp.getNetwork()->getNbNeurons(), 1);
}

// ── 5. Every simulation type × schedule runs without crashing ───────────────

TEST(UatSmoke, RunsEverySimulationTypeAndSchedule) {
    NetworkDesigner w;
    w.resize(1280, 800);
    w.show();
    pump();

    w.loadFile(examplesDir() + "/allPositive_And_Cycle.nml");
    pump();
    ASSERT_GT(w.getNetwork()->getNbNeurons(), 0);

    auto* simType   = w.findChild<QComboBox*>("cmbSimulationType");
    auto* schedType = w.findChild<QComboBox*>("cmbScheduleType");
    auto* start     = w.findChild<QPushButton*>("pbStart");
    auto* stop      = w.findChild<QPushButton*>("pbStop");
    auto* iters     = w.findChild<QSpinBox*>("sbUpdatesNumber");
    auto* speed     = w.findChild<QSpinBox*>("sbSpeedPercent");
    ASSERT_TRUE(simType && schedType && start && stop);

    if (iters) iters->setValue(5);
    if (speed) speed->setValue(speed->maximum()); // no artificial delay

    ASSERT_GT(simType->count(), 0);
    ASSERT_GT(schedType->count(), 0);

    for (int s = 0; s < simType->count(); ++s) {
        simType->setCurrentIndex(s);
        // The diffusion sweep is a batch job (999 temperature steps × 1000
        // samples) — exercised by StopButtonCancelsDiffusionSweep below
        // instead of at full scale here.
        if (simType->currentText() == "Diffusion analysis") continue;
        for (int sched = 0; sched < schedType->count(); ++sched) {
            schedType->setCurrentIndex(sched);
            // Announce before clicking so a hang names its culprit in CI logs
            printf("[uat] running simulation='%s' schedule='%s'\n",
                   qPrintable(simType->currentText()),
                   qPrintable(schedType->currentText()));
            fflush(stdout);
            QTest::mouseClick(start, Qt::LeftButton);
            pump(1);
        }
    }
    QTest::mouseClick(stop, Qt::LeftButton);
    pump(1);

    w.grab().toImage().save(artifactsDir() + "/03-after-simulations.png");
}

// ── 5b. The Stop button must cancel a running long simulation ───────────────

TEST(UatSmoke, StopButtonCancelsDiffusionSweep) {
    NetworkDesigner w;
    w.resize(1280, 800);
    w.show();
    pump();

    w.loadFile(examplesDir() + "/allPositive_And_Cycle.nml");
    pump();

    auto* simType   = w.findChild<QComboBox*>("cmbSimulationType");
    auto* schedType = w.findChild<QComboBox*>("cmbScheduleType");
    auto* start     = w.findChild<QPushButton*>("pbStart");
    auto* stop      = w.findChild<QPushButton*>("pbStop");
    auto* iters     = w.findChild<QSpinBox*>("sbUpdatesNumber");
    auto* speed     = w.findChild<QSpinBox*>("sbSpeedPercent");
    ASSERT_TRUE(simType && schedType && start && stop);

    if (iters) iters->setValue(5);
    if (speed) speed->setValue(100);

    simType->setCurrentText("Diffusion analysis");
    schedType->setCurrentText("Parallel");

    // The sweep pumps the event loop between temperature steps, so this
    // queued Stop click gets delivered mid-run and must abort it.
    QTimer::singleShot(300, stop, [stop] { stop->click(); });

    QElapsedTimer timer;
    timer.start();
    QTest::mouseClick(start, Qt::LeftButton); // returns when run() finishes
    const qint64 elapsed = timer.elapsed();

    // Full sweep takes minutes; a cancelled one must return promptly.
    EXPECT_LT(elapsed, 30000) << "Stop button did not cancel the sweep";
    printf("[uat] diffusion sweep cancelled after %lld ms\n",
           static_cast<long long>(elapsed));
}

// ── 6. New document resets state through the whole window ───────────────────

TEST(UatSmoke, LoadThenStatusBarTracksDocument) {
    NetworkDesigner w;
    w.show();
    pump();

    w.loadFile(examplesDir() + "/CardioRespiratoire.nml");
    pump();
    const int loaded = w.getNetwork()->getNbNeurons();
    EXPECT_GT(loaded, 0);

    // The main window must track the *current* document, not the startup one
    EXPECT_EQ(w.getNetwork(), w.findChild<DesignPlan*>("frmDesign")->getNetwork());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    QApplication app(argc, argv);
    return RUN_ALL_TESTS();
}
