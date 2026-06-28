#pragma once

#include "Simulation.h"

class SimulationDiffusion : public Simulation
{

private:
	UpdateType updateType;
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

	void setUpdateType(UpdateType updateType);
	UpdateType getUpdateType() const;


public slots:
	void tick();
};
