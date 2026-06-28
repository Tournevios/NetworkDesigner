#include "networkdesigner.h"
#include "NetworkDesignerParser.h"
#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include <QtGui/QStatusBar>

// Helper: load an SVG from Qt resources as a fixed-size QIcon.
// Qt renders SVGs natively if Qt SVG module is available; otherwise
// QPixmap will load the file and display it at its natural size.
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

    evenementHandler = new EvenementHandler(&ui, network, updateSchedulingPlan);
    ssc = new SignalsSlotsConnector(evenementHandler, &ui, this);
    evenementHandler->updateMe();

    setupToolBar();
    setupStatusBar();

    // Update status bar whenever the network changes
    connect(ui.frmDesign, SIGNAL(networkIsModified()), this, SLOT(updateStatusBar()));
    connect(ui.frmDesign, SIGNAL(updateCalled()),      this, SLOT(updateStatusBar()));
    connect(ui.pbStart,   SIGNAL(clicked(bool)),       this, SLOT(updateStatusBar()));
    connect(ui.pbStop,    SIGNAL(clicked(bool)),       this, SLOT(updateStatusBar()));

    updateStatusBar();
}

// ── Toolbar ──────────────────────────────────────────────────────────────────

void NetworkDesigner::setupToolBar() {
    QToolBar* toolbar = addToolBar("Main");
    toolbar->setObjectName("mainToolBar");
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(20, 20));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // Style the toolbar itself via object name so the QSS can target it
    toolbar->setStyleSheet(
        "QToolBar { background:#181825; border-bottom:1px solid #313244; padding:2px 4px; spacing:4px; }"
        "QToolButton { background:transparent; color:#a6adc8; border:none; border-radius:5px;"
        "              padding:3px 8px; font-size:10px; }"
        "QToolButton:hover  { background:#313244; color:#cdd6f4; }"
        "QToolButton:pressed { background:#45475a; }"
        "QToolBar::separator { background:#313244; width:1px; margin:4px 6px; }"
    );

    auto addTB = [&](QAction* action, const QString& iconPath, const QString& tip) {
        action->setIcon(svgIcon(iconPath));
        action->setToolTip(tip);
        toolbar->addAction(action);
    };

    addTB(ui.action_New,  ":/resources/icons/new.svg",  "New");
    addTB(ui.actionO_pen, ":/resources/icons/open.svg", "Open");
    addTB(ui.action_Save, ":/resources/icons/save.svg", "Save");

    toolbar->addSeparator();

    addTB(ui.actionDelete, ":/resources/icons/delete.svg", "Delete");

    toolbar->addSeparator();

    // Start / Stop — wire directly to the panel buttons so signals still fire
    auto* actStart = toolbar->addAction(svgIcon(":/resources/icons/start.svg"), "Start");
    actStart->setToolTip("Run simulation (Ctrl+R)");
    actStart->setShortcut(QKeySequence("Ctrl+R"));
    connect(actStart, SIGNAL(triggered(bool)), ui.pbStart, SIGNAL(clicked(bool)));

    auto* actStop = toolbar->addAction(svgIcon(":/resources/icons/stop.svg"), "Stop");
    actStop->setToolTip("Stop simulation");
    connect(actStop, SIGNAL(triggered(bool)), ui.pbStop, SIGNAL(clicked(bool)));
}

// ── Status bar ────────────────────────────────────────────────────────────────

void NetworkDesigner::setupStatusBar() {
    QStatusBar* sb = statusBar();

    // Left side: simulation state
    statusSimState = new QLabel("  Idle  ");
    statusSimState->setStyleSheet("color:#a6e3a1; font-size:11px; padding:0 8px;");
    sb->addWidget(statusSimState);

    // Spacer to push permanent widgets to the right
    QLabel* spacer = new QLabel();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sb->addWidget(spacer);

    // Permanent: neuron count with icon label
    QLabel* iconN = new QLabel();
    QPixmap pmN(12, 12);
    pmN.load(":/resources/icons/neuron.svg");
    iconN->setPixmap(pmN);
    iconN->setStyleSheet("padding:0 2px 0 8px;");
    sb->addPermanentWidget(iconN);

    statusNeurons = new QLabel("0 neurons");
    statusNeurons->setStyleSheet("color:#89b4fa; font-size:11px; padding:0 6px 0 0;");
    sb->addPermanentWidget(statusNeurons);

    // Permanent: synapse count with icon label
    QLabel* iconS = new QLabel();
    QPixmap pmS(12, 12);
    pmS.load(":/resources/icons/synapse.svg");
    iconS->setPixmap(pmS);
    iconS->setStyleSheet("padding:0 2px 0 8px;");
    sb->addPermanentWidget(iconS);

    statusSynapses = new QLabel("0 synapses");
    statusSynapses->setStyleSheet("color:#cba6f7; font-size:11px; padding:0 8px 0 0;");
    sb->addPermanentWidget(statusSynapses);
}

void NetworkDesigner::updateStatusBar() {
    if (!network) return;

    // Count total synapses across all neurons
    int totalSynapses = 0;
    for (int i = 0; i < network->getNbNeurons(); ++i)
        totalSynapses += network->getNeuron(i)->getNb_neighbors();

    const int n = network->getNbNeurons();
    statusNeurons->setText(QString("%1 neuron%2").arg(n).arg(n != 1 ? "s" : ""));
    statusSynapses->setText(QString("%1 synapse%2").arg(totalSynapses).arg(totalSynapses != 1 ? "s" : ""));
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
    delete ssc;
    delete evenementHandler;
}
