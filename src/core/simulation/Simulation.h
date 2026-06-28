#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "UpdateBlock.h"
#include "Network.h"
#include "Computer.h"
#include <fstream>
#include <QThread>

#define P 0
#define BP 1
#define BS 2
#define S 3

class Simulation : public QThread // , public QWidget
{
	Q_OBJECT
protected:
	std::string name;
	std::string filepath;
	UpdateBlock * view;
	Network * network;
	Network * initialNetwork;
	Computer * computer;

public:

	Simulation(Computer * computer);
	Simulation();
	virtual ~Simulation();

	// name getter and setter
	virtual std::string getName() const;
	virtual void setName(std::string name);

	virtual std::string getFilepath() const;
	virtual void setFilepath(std::string filepath);

	virtual void setView(UpdateBlock * view);
	virtual UpdateBlock * getView() const;

	//virtual void runSimulation()=0;
	virtual void run()=0;
	virtual Network * getInitialNetwork();
	virtual void copyNetwork(Network * network);

	virtual Computer * getComputer() const;

public slots:
	virtual void tick()=0;

};

#endif /*SIMULATION_H_*/
