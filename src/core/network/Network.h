#pragma once

#include "NeuronSynapse.h"
//#include "UpdateSchedulingPlan.h"
#include <memory>

class Network
{
public:
	
	// Constructor && destructor
	Network();
	Network(double temperature);
	Network(const Network& network);
	void addNetwork(Network * network);
	virtual ~Network();
	
	// Class's methods && functions
	Neuron* getNeuron(int index) const;
	//void computeBP(UpdateSchedulingPlan* sp, int nb_iterations, double synchronyRate);
	//void computeBS(UpdateSchedulingPlan* sp, double synchronyRate);
	
	void addNeuron(Neuron * neuron);
	void delNeuron(int index);
	int getState(bool* states) const; 
	
	// Attributes getters && setters 	
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
	std::vector<std::unique_ptr<Neuron>> neurons;
	double temperature;
	bool uniformalTemperature;
	
};
