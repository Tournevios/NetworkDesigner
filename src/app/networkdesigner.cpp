#include "networkdesigner.h"
#include "NetworkDesignerParser.h"
#include "designplan.h"
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QStatusBar>
#include <QApplication>

// Helper: load an SVG from Qt resources as a fixed-size QIcon.
static QIcon svgIcon(const QString& path, int size = 20) {
    QPixmap pm(size, size);
    pm.load(path);
    return QIcon(pm);
}

NetworkDesigner::NetworkDesigner(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    QStringList qslScheduleTypes;
    qslScheduleTypes << "Parallel" << "Block Parallel" << "Block Sequential" << "Sequential";
    ui.cmbScheduleType->insertItems(-1, qslScheduleTypes);
    ui.cmbUpdateBlock->insertItem(-1, "Add a new block...");

    QStringList qslUpdateMethods;
    qslUpdateMethods << "Compute" << "Fixe" << "All to 1"
                     << "All to 0" << "Alternate 01" << "Alternate 10" << "Randomly";
    ui.cmbUpdateMethods->insertItems(-1, qslUpdateMethods);

    QStringList qslSimulations;
    qslSimulations << "No simulation" << "Activity analysis" << "Changement analysis"
                   << "Diffusion analysis" << "Attractor and basins" << "Attractor and basins V2";
    ui.cmbSimulationType->insertItems(-1, qslSimulations);

    setWindowTitle("Network Designer");

    network = ui.frmDesign->getNetwork();
    updateSchedulingPlan = ui.frmDesign->getUpdateSchedulingPlan();

    evenementHandler = std::make_unique<EvenementHandler>(&ui, network, updateSchedulingPlan);
    ssc = std::make_unique<SignalsSlotsConnector>(evenementHandler.get(), &ui, this);
    evenementHandler->updateMe();

    setupToolBar();
    setupStatusBar();

    // Network change → update counters
    connect(ui.frmDesign, &DesignPlan::networkIsModified, this, &NetworkDesigner::updateStatusBar);
    connect(ui.frmDesign, &DesignPlan::updateCalled,      this, &NetworkDesigner::updateStatusBar);

    // Simulation state indicator via Start / Stop buttons
    connect(ui.pbStart, &QPushButton::clicked, this, &NetworkDesigner::onSimulationStarted);
    connect(ui.pbStop,  &QPushButton::clicked, this, &NetworkDesigner::onSimulationFinished);

    updateStatusBar();
}

// ── Toolbar ──────────────────────────────────────────────────────────────────

void NetworkDesigner::setupToolBar() {
    QToolBar* toolbar = addToolBar("Main");
    toolbar->setObjectName("mainToolBar");
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(20, 20));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    auto addTB = [&](QAction* action, const QString& iconPath, const QString& tip) {
        action->setIcon(svgIcon(iconPath));
        action->setToolTip(tip);
        toolbar->addAction(action);
    };

    addTB(ui.action_New,  ":/resources/icons/new.svg",  "New\nCtrl+N");
    addTB(ui.actionO_pen, ":/resources/icons/open.svg", "Open\nCtrl+O");
    addTB(ui.action_Save, ":/resources/icons/save.svg", "Save\nCtrl+S");

    toolbar->addSeparator();

    addTB(ui.actionDelete, ":/resources/icons/delete.svg", "Delete\nDel");

    toolbar->addSeparator();

    // Start / Stop — proxy signals to the panel buttons
    auto* actStart = toolbar->addAction(svgIcon(":/resources/icons/start.svg"), "Start");
    actStart->setToolTip("Run simulation\nCtrl+R");
    actStart->setShortcut(QKeySequence("Ctrl+R"));
    connect(actStart, &QAction::triggered, ui.pbStart, &QPushButton::clicked);

    auto* actStop = toolbar->addAction(svgIcon(":/resources/icons/stop.svg"), "Stop");
    actStop->setToolTip("Stop simulation");
    connect(actStop, &QAction::triggered, ui.pbStop, &QPushButton::clicked);

    toolbar->addSeparator();

    // Zoom controls
    auto* actZoomIn = toolbar->addAction("+");
    actZoomIn->setToolTip("Zoom in\n+");
    actZoomIn->setShortcut(QKeySequence("+"));
    connect(actZoomIn, &QAction::triggered, [this] {
        double s = ui.frmDesign->getScale();
        if (s + 0.1 <= 2.0) ui.frmDesign->setScale(s + 0.1);
    });

    auto* actZoomOut = toolbar->addAction("−");
    actZoomOut->setToolTip("Zoom out\n−");
    actZoomOut->setShortcut(QKeySequence("-"));
    connect(actZoomOut, &QAction::triggered, [this] {
        double s = ui.frmDesign->getScale();
        if (s - 0.1 > 0.1) ui.frmDesign->setScale(s - 0.1);
    });

    auto* actReset = toolbar->addAction("1:1");
    actReset->setToolTip("Reset view\nCtrl+0");
    actReset->setShortcut(QKeySequence("Ctrl+0"));
    connect(actReset, SIGNAL(triggered(bool)), ui.frmDesign, SLOT(resetView()));
}

// ── Status bar ────────────────────────────────────────────────────────────────

void NetworkDesigner::setupStatusBar() {
    QStatusBar* sb = statusBar();

    // Left: simulation state pill
    statusSimState = new QLabel("  ● Idle  ");
    statusSimState->setStyleSheet(
        "color:#a6e3a1; font-size:11px; padding:0 8px; background:transparent;");
    sb->addWidget(statusSimState);

    // Separator
    QFrame* sep = new QFrame();
    sep->setFrameShape(QFrame::VLine);
    sep->setFrameShadow(QFrame::Sunken);
    sep->setStyleSheet("color:#313244;");
    sb->addWidget(sep);

    // Canvas coordinates
    statusCoords = new QLabel("  x: 0  y: 0  ");
    statusCoords->setStyleSheet(
        "color:#6c7086; font-size:11px; font-family:monospace; padding:0 4px; background:transparent;");
    statusCoords->setMinimumWidth(120);
    sb->addWidget(statusCoords);

    // Spacer
    QLabel* spacer = new QLabel();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sb->addWidget(spacer);

    // Neuron count
    QLabel* iconN = new QLabel();
    QPixmap pmN(12, 12); pmN.load(":/resources/icons/neuron.svg");
    iconN->setPixmap(pmN);
    iconN->setStyleSheet("padding:0 2px 0 8px; background:transparent;");
    sb->addPermanentWidget(iconN);

    statusNeurons = new QLabel("0 neurons");
    statusNeurons->setStyleSheet(
        "color:#89b4fa; font-size:11px; padding:0 6px 0 0; background:transparent;");
    sb->addPermanentWidget(statusNeurons);

    // Synapse count
    QLabel* iconS = new QLabel();
    QPixmap pmS(12, 12); pmS.load(":/resources/icons/synapse.svg");
    iconS->setPixmap(pmS);
    iconS->setStyleSheet("padding:0 2px 0 8px; background:transparent;");
    sb->addPermanentWidget(iconS);

    statusSynapses = new QLabel("0 synapses");
    statusSynapses->setStyleSheet(
        "color:#cba6f7; font-size:11px; padding:0 8px 0 0; background:transparent;");
    sb->addPermanentWidget(statusSynapses);
}

// ── Slots ─────────────────────────────────────────────────────────────────────

void NetworkDesigner::updateStatusBar() {
    if (!network) return;

    int totalSynapses = 0;
    for (int i = 0; i < network->getNbNeurons(); ++i)
        totalSynapses += network->getNeuron(i)->getNb_neighbors();

    const int n = network->getNbNeurons();
    statusNeurons->setText(QString("%1 neuron%2").arg(n).arg(n != 1 ? "s" : ""));
    statusSynapses->setText(
        QString("%1 synapse%2").arg(totalSynapses).arg(totalSynapses != 1 ? "s" : ""));
}

void NetworkDesigner::onCanvasMouseMoved(int worldX, int worldY) {
    statusCoords->setText(QString("  x: %1  y: %2  ").arg(worldX).arg(worldY));
}

void NetworkDesigner::onSimulationStarted() {
    statusSimState->setText("  ● Simulating…  ");
    statusSimState->setStyleSheet(
        "color:#fab387; font-size:11px; padding:0 8px; background:transparent;");
    QApplication::processEvents(); // flush UI before blocking simulation
}

void NetworkDesigner::onSimulationFinished() {
    statusSimState->setText("  ● Idle  ");
    statusSimState->setStyleSheet(
        "color:#a6e3a1; font-size:11px; padding:0 8px; background:transparent;");
    updateStatusBar();
}

// ── Getters / setters ─────────────────────────────────────────────────────────

Network* NetworkDesigner::getNetwork() const { return network; }

void NetworkDesigner::setNetwork(Network* network) {
    this->network = network;
    ui.frmDesign->setNetwork(network);
    updateStatusBar();
}

UpdateSchedulingPlan* NetworkDesigner::getUpdateSchedulingplan() const {
    return updateSchedulingPlan;
}

void NetworkDesigner::setUpdateSchedulingPlan(UpdateSchedulingPlan* usp) {
    updateSchedulingPlan = usp;
    ui.frmDesign->setUpdateSchedulingPlan(usp);
}

NetworkDesigner::~NetworkDesigner() {
    for (int i = 0; i < network->getNbNeurons(); ++i)
        network->delNeuron(i);
}
