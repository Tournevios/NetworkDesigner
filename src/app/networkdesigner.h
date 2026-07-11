#pragma once

#include <memory>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include "ui_mainWindow.h"
#include "NetworkDesignerParser.h"
#include "EvenementHandler.h"
#include "SignalsSlotsConnector.h"


class NetworkDesigner : public QMainWindow
{
    Q_OBJECT
public:
    explicit NetworkDesigner(QWidget *parent = nullptr);
    ~NetworkDesigner();

    Network * getNetwork() const;
    void setNetwork(Network * network);

    UpdateSchedulingPlan * getUpdateSchedulingplan() const;
    void setUpdateSchedulingPlan(UpdateSchedulingPlan * updateSchedulingPlan);

    // Load a .nml document without any dialog (startup files, tests, intents)
    void loadFile(const QString & path);

private slots:
    void updateStatusBar();
    void onDocumentLoaded();
    void onCanvasMouseMoved(int worldX, int worldY);
    void onSimulationStarted();
    void onSimulationFinished();

private:
    void setupToolBar();
    void setupStatusBar();

    Ui::MainWindow ui;
    Network * network = nullptr;
    UpdateSchedulingPlan * updateSchedulingPlan = nullptr;

    std::unique_ptr<EvenementHandler> evenementHandler;
    std::unique_ptr<SignalsSlotsConnector> ssc;

    // Status bar widgets (Qt-parent-owned)
    QLabel * statusSimState  = nullptr;
    QLabel * statusCoords    = nullptr;
    QLabel * statusNeurons   = nullptr;
    QLabel * statusSynapses  = nullptr;
};
