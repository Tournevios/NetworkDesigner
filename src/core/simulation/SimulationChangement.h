#pragma once

#include "Simulation.h"
#include <vector>

class SimulationChangement : public Simulation
{
private:
	UpdateType updateType;
	int affinity;

	int totalNumberOfChangeToOne;
	int totalNumberOfChangeToZero;
	int totalNumberOfZeroHolding;
	int totalNumberOfOneHolding;
	double changeToOneRating;
	double changeToZeroRating;
	double changeRating;
	double holdingInOneRating;
	double holdingInTwoRating;

	int numberOfIterations;
	std::vector<int> oldNetworkState;

public:
	SimulationChangement(Computer * computer);
	virtual ~SimulationChangement();
	void run();

	void setAffinity(int affinity);
	int getAffinity() const;

	void setUpdateType(UpdateType updateType);
	UpdateType getUpdateType() const;


public slots:
	void tick();

};
