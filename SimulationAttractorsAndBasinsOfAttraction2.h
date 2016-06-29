/*
 * SimulationAttractorsAndBasinsOfAttraction2.h
 *
 *  Created on: 09-Jul-2008
 *      Author: hbenamor
 */

#ifndef SIMULATIONATTRACTORSANDBASINSOFATTRACTION2_H_
#define SIMULATIONATTRACTORSANDBASINSOFATTRACTION2_H_

#include "Simulation.h"
#include "SetOfNetworkStatesSet.h"
#include "NetworkState2.h"
#include <vector>
#include <QThread>
#include <QMutex>

class SimulationAttractorsAndBasinsOfAttraction2;
class pCalculateTransitions: public QThread {
public:
	pCalculateTransitions();
	pCalculateTransitions(SimulationAttractorsAndBasinsOfAttraction2 * simulation, Computer * computer, int startIndex, int endIndex);
	pCalculateTransitions(const pCalculateTransitions& pcalculateTransitions);
	void run();
	virtual ~pCalculateTransitions();
protected:
	SimulationAttractorsAndBasinsOfAttraction2* sim;
	Computer * computer;
	int startIndex;
	int endIndex;
	QMutex mutexTrans;
	QMutex mutexProg;
	QMutex mutexAttNumber;
};

class SimulationAttractorsAndBasinsOfAttraction2 : public Simulation
{
	friend class pCalculateTransitions;
private:
	int updateType;
	int affinity;
	double temperature;
	int numberOfIterations;
	int numberOfCycles;
	int numberOfFixedPoints;
	int numberOfVisitedStates;
	int totalNumberOfStates;
	NetworkState * nbStates;
	double mTransitions[100][100];
	int sizeOfBasins [100];
	double percentageOfActivity[100];
	std::vector<NetworkStatesSet*> attractors;
	NetworkStatesSet * allCombinations;
public:
	SimulationAttractorsAndBasinsOfAttraction2();
	SimulationAttractorsAndBasinsOfAttraction2(Computer * computer);
	virtual ~SimulationAttractorsAndBasinsOfAttraction2();

	void run();

	void setAffinity(int affinity);
	int getAffinity() const;

	Computer * getComputer() const;

	void setUpdateType(int updateType);
	int getUpdateType() const;

	NetworkStatesSet * getAllCombinations() const;
	NetworkState *getNbStates() const;
	int * getSizeOfBasins() const;
	double** getMTransitions() const;
	int getTotalNumberOfStates() const;
	void setNumberOfVisitedStates(int numberOfVisitedStates);
	int getNumberOfVisitedStates() const;
	void setProgressBarValue(int value);
	void setMTransitions(int i, int j, double value);
	double getMTransitions(int i, int j) const;

	void generateAllStates();
	void linkThem(int startingIndex);
	int indexOf(NetworkState* networkState);
	void attractorFound(int startingIndex);
	void setAttractorNumber(int startingIndex, int attractorNumber);
	void calculateTransitions();
	void calculateTransitions(int numberOfThreads);
	void calculateSizeOfBasins();
	void calculateActivityPerBasins();
	int numberOfOne(NetworkState * networkState);
	void initializeMTransitions();


	NetworkState * getRandomNetworkState();
	void addAndCompress(NetworkStatesSet * set, NetworkState * networkState);

	NetworkState * getANonVisitedNetworkState();

public slots:
	void tick();
signals:
	void startThreads(QThread::Priority);
};

#endif /* SIMULATIONATTRACTORSANDBASINSOFATTRACTION2_H_ */
