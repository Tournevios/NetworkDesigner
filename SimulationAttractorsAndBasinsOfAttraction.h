#ifndef SIMULATIONATTRACTORSANDBASINSOFATTRACTION_H_
#define SIMULATIONATTRACTORSANDBASINSOFATTRACTION_H_

#include "Simulation.h"
#include "SetOfNetworkStatesSet.h"
#include <vector>

class SimulationAttractorsAndBasinsOfAttraction : public Simulation
{
private:
	int updateType;
	int affinity;
	static const double temperature = 0.0;
	int numberOfIterations;
	int numberOfCycles;
	int numberOfFixedPoints;
	int numberOfVisitedStates;
	int totalNumberOfStates;
	NetworkState * nbStates;
	NetworkStatesSet * nonExploredSpace;
	std::vector<NetworkStatesSet*> attractors;
	std::vector<NetworkStatesSet*> basinsOfAttraction;
	std::vector<SetOfNetworkStatesSet*> solutions;
	NetworkStatesSet * visitedStates;
	NetworkStatesSet * stateBuffer;

public:
	SimulationAttractorsAndBasinsOfAttraction(Computer * computer);
	virtual ~SimulationAttractorsAndBasinsOfAttraction();
	void run();

	void setAffinity(int affinity);
	int getAffinity() const;

	void setUpdateType(int updateType);
	int getUpdateType() const;

	NetworkStatesSet * pPredecessorOf(NetworkState* x);
	NetworkStatesSet * bpPredecessorOf(NetworkState* x);
	NetworkStatesSet * bsPredecessorOf(NetworkState* x);
	NetworkStatesSet * sPredecessorOf(NetworkState* x);
	NetworkStatesSet * predecessorOf(NetworkState* x);

	NetworkStatesSet * pBackward(NetworkStatesSet * attractor);
	NetworkState * getRandomNetworkState();
	void addAndCompress(NetworkStatesSet * set, NetworkState * networkState);

	NetworkState * getANonVisitedNetworkState();

	void solveAndStore(int neuronIndex);

public slots:
	void tick();
};

#endif /*SIMULATIONATTRACTORSANDBASINSOFATTRACTION_H_*/
