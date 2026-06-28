#pragma once
#include "ui_mainWindow.h"
#include "EvenementHandler.h"

class SignalsSlotsConnector : public QObject
{
public:
	SignalsSlotsConnector(EvenementHandler * evenementHandler, Ui::MainWindow * ui, QMainWindow * theWindow);
	virtual ~SignalsSlotsConnector();
};
