#ifndef NETWORKDESIGNER_H
#define NETWORKDESIGNER_H

#include <QtGui/QWidget>
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
	 
private:
    Ui::MainWindow ui;
    Network * network;
    UpdateSchedulingPlan * updateSchedulingPlan;
 
    EvenementHandler * evenementHandler;
    SignalsSlotsConnector * ssc;

};

#endif // NETWORKDESIGNER_H
