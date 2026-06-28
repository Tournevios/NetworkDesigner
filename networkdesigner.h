#ifndef NETWORKDESIGNER_H
#define NETWORKDESIGNER_H

#include <QtGui/QWidget>
#include <QtGui/QToolBar>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include "ui_mainWindow.h"
#include "NetworkDesignerParser.h"
#include "EvenementHandler.h"
#include "SignalsSlotsConnector.h"


class NetworkDesigner : public QMainWindow
{
    Q_OBJECT
public:
    NetworkDesigner(QWidget *parent = 0);
    ~NetworkDesigner();

    Network * getNetwork() const;
    void setNetwork(Network * network);

    UpdateSchedulingPlan * getUpdateSchedulingplan() const;
    void setUpdateSchedulingPlan(UpdateSchedulingPlan * updateSchedulingPlan);

    void load(char * path);

public slots:
    void updateStatusBar();

private:
    void setupToolBar();
    void setupStatusBar();

    Ui::MainWindow ui;
    Network * network;
    UpdateSchedulingPlan * updateSchedulingPlan;

    EvenementHandler * evenementHandler;
    SignalsSlotsConnector * ssc;

    // Status bar widgets
    QLabel * statusNeurons;
    QLabel * statusSynapses;
    QLabel * statusSimState;
};

#endif // NETWORKDESIGNER_H
