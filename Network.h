#ifndef NETWORK_H_
#define NETWORK_H_

#include "NeuronSynapse.h"
//#include "UpdateSchedulingPlan.h"

class Network
{
public:
	
	// Constructor and destructor
	Network();
	Network(double temperature);
	Network(const Network& network);
	void addNetwork(Network * network);
	virtual ~Network();
	
	// Class's methods and functions
	Neuron* getNeuron(int index) const;
	//void computeBP(UpdateSchedulingPlan* sp, int nb_iterations, double synchronyRate);
	//void computeBS(UpdateSchedulingPlan* sp, double synchronyRate);
	
	void addNeuron(Neuron * neuron);
	void delNeuron(int index);
	int getState(bool* states) const; 
	
	// Attributes getters and setters 	
	int getNbNeurons() const;
	
	double getTemperature() const;
	void setTemperature(double temperature);
	
	void setUniformalTemperature(bool uniformalTemperature);
	bool getUniformalTemperature() const;
	
	// Virtual method
	//virtual void showNetwork() = 0; 
	void drawMe(QPainter* painter, float scale, int transX, int transY);
	Neuron* getNeuron(int x, int y, int radius) const;
	Synapse * getSynapse(int x, int y) const;
	
	void deselectAll();

private:
	int nb_neurons;
	std::vector<Neuron*> neurons;
	double temperature;
	bool uniformalTemperature;
	
};

#endif /*NETWORK_H_*/
