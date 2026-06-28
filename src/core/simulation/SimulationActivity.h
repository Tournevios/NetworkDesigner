#pragma once

#include "Simulation.h"

class SimulationActivity : public Simulation
{

private:
	UpdateType updateType;
	int affinity;
	int totalNumberOfOne;
	int totalNumberOfZero;
	int numberOfIterations;
	double oneRating;
	double zeroRating;
	std::string datfile;
	std::ofstream *out;
	int countIter;
public:
	SimulationActivity(Computer * computer);
	virtual ~SimulationActivity();
	void run();

	void setAffinity(int affinity);
	int getAffinity() const;

	void setUpdateType(UpdateType updateType);
	UpdateType getUpdateType() const;


public slots:
	void tick();
};
