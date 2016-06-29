#ifndef SIMULATIONACTIVITY_H_
#define SIMULATIONACTIVITY_H_

#include "Simulation.h"

class SimulationActivity : public Simulation
{

private:
	int updateType;
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

	void setUpdateType(int updateType);
	int getUpdateType() const;


public slots:
	void tick();
};

#endif /*SIMULATIONACTIVITY_H_*/
