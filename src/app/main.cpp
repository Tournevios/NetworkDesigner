#include "networkdesigner.h"

#include <QtGui>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QStandardPaths>
#include "Network.h"
#include "UpdateSchedulingPlan.h"

#ifdef Q_OS_ANDROID
#include <android/log.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "NetworkDesigner", __VA_ARGS__)
#else
#define LOGD(...) qDebug(__VA_ARGS__)
#endif

int main(int argc, char *argv[])
{
    LOGD("main: starting");
    QApplication a(argc, argv);
    LOGD("main: QApplication created");

#ifdef Q_OS_ANDROID
    // The Activity can momentarily close and recreate the Qt window during
    // startup (splash teardown, IME/system focus changes). Because
    // quitOnLastWindowClosed defaults to true, that flicker becomes a silent
    // clean exit — the "window appears then closes" symptom. The Activity owns
    // the process lifetime on Android, so opt out entirely.
    a.setQuitOnLastWindowClosed(false);
    // The platform style on Android is unreliable for Qt Widgets; Fusion is
    // the supported cross-platform choice. Must be set after QApplication so
    // the style plugin paths are initialized.
    QApplication::setStyle("Fusion");
#endif

    // Apply the bundled modern dark theme. It was shipped in resources but
    // never loaded, so the app fell back to the default platform palette.
    QFile qss(":/resources/style.qss");
    if (qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(QString::fromUtf8(qss.readAll()));
        qss.close();
    }

    NetworkDesigner w;
    LOGD("main: NetworkDesigner constructed");
    w.show();
    LOGD("main: window shown, entering event loop");

    // --- Diagnostic instrumentation (enabled by a marker file) ------------
    // If <writable-app-dir>/autoload.nml exists, load it a moment after the
    // window is up, and emit a heartbeat every 500 ms. If the main thread
    // freezes, the heartbeats stop and the last breadcrumb pinpoints where.
    // The marker is pushed only by the CI emulator test; normal runs never
    // create it, so production behaviour is unchanged.
    QString probeDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (probeDir.isEmpty())
        probeDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    const QString autoload = QDir(probeDir).filePath("autoload.nml");
    if (QFile::exists(autoload)) {
        LOGD("diag: autoload marker found at %s", qPrintable(autoload));
        auto* beat = new QTimer(&w);
        static int n = 0;
        QObject::connect(beat, &QTimer::timeout, [] { LOGD("diag: heartbeat %d", n++); });
        beat->start(500);
        QTimer::singleShot(1500, &w, [&w, autoload] {
            LOGD("diag: autoloading network now");
            w.loadFile(autoload);
            LOGD("diag: autoload returned");
        });
    }

    return a.exec();
}
