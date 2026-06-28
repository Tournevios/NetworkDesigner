#include "Network.h"
#include "constFile.h"
#include "random-singleton.h"
#include <cmath>
#include <iostream>
using namespace std;


Network::Network(double temperature)
{
	nb_neurons = 0;
	uniformalTemperature = true;
	this->temperature = temperature;
	Random::Uniform<double>(0.0,1.0);
}

Network::Network(){
	nb_neurons = 0;
	temperature = 0.001;
	uniformalTemperature = true;
	Random::Uniform<double>(0.0,1.0);
}

Network::Network(const Network& network){
	bool okki;
	nb_neurons = 0;
	temperature = network.getTemperature();
	uniformalTemperature = network.getUniformalTemperature();
	Random::Uniform<double>(0.0,1.0);
	
	for(int i=0; i<network.getNbNeurons(); i++){
		addNeuron(new Neuron(*network.getNeuron(i)));	
	}
	
	for(int i=0; i<nb_neurons; i++){
		for(int j=0; j<network.getNeuron(i)->getNb_neighbors();j++){
			okki = getNeuron(i)->addSynapse(getNeuron(network.getNeuron(i)->getSynapse(j)->getFinalNeuron()->getIndex()), network.getNeuron(i)->getSynapseWeight(j), network.getNeuron(i)->getSynapseDelay(j));
			getNeuron(i)->getSynapse(j)->setCX(network.getNeuron(i)->getSynapse(j)->getCX());
			getNeuron(i)->getSynapse(j)->setCY(network.getNeuron(i)->getSynapse(j)->getCY());
		}
	}
}

void Network::addNetwork(Network * network){
	int initial_nb_neurons = nb_neurons;
	bool okki;
	deselectAll();
	for(int i=0; i<network->getNbNeurons(); i++){
		addNeuron(new Neuron(*(network->getNeuron(i))));
		getNeuron(initial_nb_neurons + i)->setSelected(true);
	}
	
	for(int i=0; i<network->getNbNeurons(); i++){
		for(int j=0; j<network->getNeuron(i)->getNb_neighbors();j++){
				okki = getNeuron(i+initial_nb_neurons)->addSynapse(getNeuron(network->getNeuron(i)->getSynapse(j)->getFinalNeuron()->getIndex()+initial_nb_neurons), network->getNeuron(i)->getSynapseWeight(j), network->getNeuron(i)->getSynapseDelay(j));
				getNeuron(i+initial_nb_neurons)->getSynapse(j)->setCX(network->getNeuron(i)->getSynapse(j)->getCX());
				getNeuron(i+initial_nb_neurons)->getSynapse(j)->setCY(network->getNeuron(i)->getSynapse(j)->getCY());
		}
	}
}

Network::~Network()
{
	neurons.clear();
}

/*
 * Nb_Neurons's getter
 */
int Network::getNbNeurons() const{
	return nb_neurons;
}

/*
 * Uniformal Temperature's setter
 */
void Network::setUniformalTemperature(bool uniformalTemperature){
	this->uniformalTemperature = uniformalTemperature;
}

/*
 * Uniformal Temperature's getter
 */
bool Network::getUniformalTemperature() const{
	return uniformalTemperature;
}


/*
 * Temperature's setter
 */
void Network::setTemperature(double temperature){
	this->temperature = temperature;
}

/*
 * Temperature's getter
 */
double Network::getTemperature() const{
	return temperature;
}

/*
 * Deselect all the neurons
 */
void Network::deselectAll(){
	for (Neuron* n : neurons) {
		n->setSelected(false);
		for (int j = 0; j < n->getNb_neighbors(); ++j)
			n->getSynapse(j)->setSelected(false);
	}
}

/*
 * Return a neuron base on its x and y and the radius
 */
Neuron* Network::getNeuron(int x, int y, int radius) const{
	for(int i=0; i< nb_neurons; i++){
		if((abs(neurons[i]->getX()-x) <= radius) and (abs(neurons[i]->getY()-y)<= radius)){
			return neurons[i].get();
		}
	}
	return nullptr;
}

/*
 * Return a synapse based on the mouse x and y
 */
Synapse * Network::getSynapse(int x, int y) const{
	for(int i=0; i< nb_neurons; i++){
		for(int j=0; j < neurons[i]->getNb_neighbors();j++){
			if(neurons[i]->getSynapse(j)->selectable(x,y)){
				return neurons[i]->getSynapse(j);			
			}
		}
	}
	return nullptr;
}
/*
 * This function return a neuron based on its index
 */
Neuron* Network::getNeuron(int index) const{
	if((index>=0) and (index<nb_neurons))
		return neurons[index].get();
	return nullptr;
}

/*
 * The neuron is added to the network and indexed by its creation date
 */
void Network::addNeuron(Neuron* neuron){
	neuron->setIndex(nb_neurons);
	neurons.push_back(std::unique_ptr<Neuron>(neuron));
	nb_neurons++;
}

/*
 * Routine for suppression of a neuron from the network
 */
void Network::delNeuron(int index){
	if(getNeuron(index) == nullptr) return;

	// Delete all synapses pointing to this neuron
	for(int i = 0; i<nb_neurons; i++){
		if(getNeuron(i)->getSynapseByNeighborIndex(index) < getNeuron(i)->getNb_neighbors()){
			getNeuron(i)->delSynapseBySynapseIndex(getNeuron(i)->getSynapseByNeighborIndex(index));
			i--;
		}
	}

	// Decrement indices of neurons after the deleted one
	for(int i = index+1; i<nb_neurons; i++){
		getNeuron(i)->setIndex(i-1);
	}

	// Erase from vector (unique_ptr destructor frees the Neuron)
	neurons.erase(neurons.begin() + index);
	nb_neurons--;
}

/*
 * Get the states of all the neurons of the network
 * and return their number
 */
int Network::getState(bool* states) const{
	int i = 0;
	for (const Neuron* n : neurons) states[i++] = n->getState();
	return i;
}


/*
 * Draw the synapses then the neurons
 */
void Network::drawMe(QPainter* painter, float scale, int transX, int transY){
	for (Neuron* n : neurons) n->drawSynapses(painter, scale, transX, transY);
	for (Neuron* n : neurons) n->drawMe(painter, scale, transX, transY);
}
