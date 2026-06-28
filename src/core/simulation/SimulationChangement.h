#ifndef SIMULATIONCHANGEMENT_H_
#define SIMULATIONCHANGEMENT_H_

#include "Simulation.h"
#include <vector>

class SimulationChangement : public Simulation
{
private:
	int updateType;
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
	~SimulationChangement() override;
	void run() override;

	void setAffinity(int affinity);
	int getAffinity() const;

	void setUpdateType(int updateType);
	int getUpdateType() const;


public slots:
	void tick() override;

};

#endif /*SIMULATIONCHANGEMENT_H_*/
