#ifndef SIGNALSSLOTSCONNECTOR_H_
#define SIGNALSSLOTSCONNECTOR_H_
#include "ui_mainWindow.h"
#include "EvenementHandler.h"

class SignalsSlotsConnector : public QObject
{
public:
	SignalsSlotsConnector(EvenementHandler * evenementHandler, Ui::MainWindow * ui, QMainWindow * theWindow);
	virtual ~SignalsSlotsConnector();
};

#endif /*SIGNLASSLOTSCONNECTOR_H_*/
