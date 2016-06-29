#ifndef SIMULATIONDIFFUSION_H_
#define SIMULATIONDIFFUSION_H_

#include "Simulation.h"

class SimulationDiffusion : public Simulation
{

private:
	int updateType;
	int affinity;
	int numberOfIterations;
	std::vector<int> frequencyOfStates;
	std::vector<int> frequencyOfJumps;

public:
	SimulationDiffusion();
	SimulationDiffusion(Computer * computer);
	virtual ~SimulationDiffusion();
	void run();

	void setAffinity(int affinity);
	int getAffinity() const;

	void setUpdateType(int updateType);
	int getUpdateType() const;


public slots:
	void tick();
};

#endif /*SIMULATIONDIFFUSION_H_*/
