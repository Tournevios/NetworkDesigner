#ifndef COMPUTER_H_
#define COMPUTER_H_

#include "UpdateSchedulingPlan.h"
#include "random-singleton.h"
#include "ui_mainWindow.h"
#include "NetworkState.h"

class Computer: public QWidget
{
	Q_OBJECT
private:
	Network * network;
	//Simulation * simulation;
	UpdateSchedulingPlan * updateSchedulingPlan;
	Ui::MainWindow * ui;
	bool stop;
	bool repaintDesign;
	int nb_iterations;

public:

	// Constructor and destructors
	Computer();
	Computer(const Computer& computer);
	Computer(Network * network, UpdateSchedulingPlan * updateSchedulingPlan, Ui::MainWindow * ui);
	virtual ~Computer();

	// getters and setters
	Network * getNetwork() const;
	void setNetwork(Network * network);

	void stopComputing();

	/*
	Simulation * getSimulation() const;
	void setSimulation(Simulation * simulation);
	*/
	UpdateSchedulingPlan * getUpdateSchedulingPlan() const;
	void setUpdateSchedulingPlan(UpdateSchedulingPlan * updateSchedulingPlan);
	void setProgressBarValue(int value);

	virtual int getNbIterations() const;
	virtual void setNbIterations(int nb_iterations);
	virtual Ui::MainWindow * getUi() const;
	void setStateOfTheNetwork(NetworkState* networkState);

	// Class methods
	void computeBP();
	void computeBS();
	void computeP();
	void computeS();

signals:
	void tick();
	void setProgressBarValue_Signal(int);
};

#endif /*COMPUTER_H_*/
