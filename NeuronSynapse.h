#ifndef NEURONSYNAPSE_H_
#define NEURONSYNAPSE_H_

#include <cmath>
#include <vector>
#include <queue>
#include <QtGui/QPainter>
#include <QtGui/QPen>

class Neuron;
class Synapse
{
public:
	// Getters and Setters
	Synapse();
	Synapse(Neuron * baseNeuron, Neuron * finalNeuron, double weight, int delay=0);
	virtual ~Synapse();

	Neuron * getBaseNeuron() const;
	void setBaseNeuron(Neuron* baseNeuron);

	Neuron * getFinalNeuron() const;
	void setFinalNeuron(Neuron* finalNeuron);

	double getWeight() const;
	void setWeight(double weight);

	float getGExcentricity() const;
	void setGExcentricity(float gExcentricity);

	bool getSelected() const;
	void setSelected(bool selected);

	int getDelay() const;
	void setDelay(int delay);
	void refreshMe();
	int getStateOfTheFinalNeuron();

	void setCX(double cx);
	void setCY(double cx);
	double getCX() const;
	double getCY() const;

	void drawMe(QPainter * painter, double scale, double transX, double transY);
	bool selectable(double x, double y);


private:

	Neuron * baseNeuron;
	Neuron * finalNeuron;
	double weight;
	float gExcentricity;
	double cx, cy;
	bool selected;
	int delay;
	std::queue<int> states;

	QPainterPath gPath;

};

class Neuron
{
public:
	// Constructors and destructor
	Neuron();
	Neuron(int index);
	//Neuron(int index, bool state);
	Neuron(int index, int state, int nbStates, std::vector<double> thresholds);
	Neuron(const Neuron& neuron);
	virtual ~Neuron();


	// Getters and setters
	int getIndex() const;
	void setIndex(int index);

	int getState() const;
	void setState(int state);

	int getNbStates() const;
	void setNbStates(int nbStates);

	void setTheNewState(int theNewState);

	double getTemperature() const;
	void setTemperature(double temperature);

	double getThreshold(int stateIndex) const;
	std::vector<double> getThresholds() const;
	void setThreshold(int stateIndex, double threshold);

	int getNb_neighbors() const;
	void setNb_neighbors(int nb_neighbors);

/*	void setColor(int red, int green, int blue);
	int* getColor();
*/
	void setXY(double x,double y);
	void setX(double x);
	void setY(double y);
	double getX() const;
	double getY() const;

	bool getSelected() const;
	void setSelected(bool selected);

	bool getYellowMe() const;
	void setYellowMe(bool yellowMe);

	// Class's Methods and functions
	void compute(double temperature);
	void compute2(double temperature);
	void substitute();
	bool addSynapse(Neuron* neighbor, double weight, int delay=0);
	//void addSynapse(Neuron * neighbor, double weight, int red, int green, int blue);
	void delSynapseBySynapseIndex(int synapseIndex);
	int getSynapseByNeighborIndex(int neighborIndex) const;
	Synapse * getSynapse(int synapseIndex) const;
	Synapse * getSelfSynapse() const;
/*
	void setSynapseColor(int synapseIndex, int red, int green, int blue);
	int* getSynapseColor(int synapseIndex);
*/
	void setSynapseWeight(int synapseIndex, double weight);
	double getSynapseWeight(int synapseIndex) const;
	void drawMe(QPainter* painter, double scale, double transX, double transY);
	void drawSynapses(QPainter* painter, double scale, double transX, double transY);

	void setSynapseGE(int synapseIndex, float gE);
	float getSynapseGE(int synapseIndex) const;

	void setSynapseDelay(int synapseIndex, int synapseDelay);
	int getSynapseDelay(int synapseIndex) const;
	void refreshSynapses();

	void setNodeID(char * nodeID);
	const char * getNodeID() const;

	Neuron * getNeighbor(int synapseIndex) const;

protected:
	void init();
private:

	int index;
	int state;									// Active or not
	int nbStates;
	std::vector<double> threshold;
	double temperature;
	char nodeID[255];

	// Synapses
	int nb_neighbors;
	std::vector<Synapse*> synapses;

	bool hasANewState;							// Set to true if a computation was done
	int theNewState;							// The new state of the neuron

	// Drawing parameters
	double x, y;									// x and y in the widget coordinate system
/*	int color[3];								// The color of the neuron in the RGB system
	vector<int[3]> synapsesColor;				// Synapses colors in the RGB system
*/
	bool selected;								//If the neuron is selected draw a blue circle
	bool yellowMe;								//If the neuron is selected draw a yellow circle for UpdateBlock's using
};



#endif /*NEURONSYNAPSE_H_*/
