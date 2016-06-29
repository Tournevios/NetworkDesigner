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
	for(int i=0; i< nb_neurons; i++){
		neurons[i]->setSelected(false);
		for(int j=0; j < neurons[i]->getNb_neighbors();j++){
			neurons[i]->getSynapse(j)->setSelected(false);
		}
	}
}

/*
 * Return a neuron base on its x and y and the radius
 */
Neuron* Network::getNeuron(int x, int y, int radius) const{
	
	for(int i=0; i< nb_neurons; i++){
		if((abs(neurons[i]->getX()-x) <= radius) and (abs(neurons[i]->getY()-y)<= radius)){
			return neurons[i];			
		}
	}
	return NULL;
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
	return NULL;
}
/*
 * This function return a neuron based on its index
 */
Neuron* Network::getNeuron(int index) const{
	if((index>=0) and (index<nb_neurons))
			return neurons[index];
	else 
		return NULL;
}

/*
 * This methods calculate the new global stat of the network using a block sequential update

void Network::computeBS(UpdateSchedulingPlan* sp, double synchronyRate){
	
	int nb_blocks = sp->getNb_blocks();
	double rnd;
	for(int i=0;i< nb_blocks;i++){
		
		UpdateBlock* ub = sp->getUpdateBlock(i);
		if(ub->getUpdateMethods() == COMPUTE){
			// All neuron compute their new state using a parallele scheme
			for(int j=0; j < ub->getSize(); j++){
				rnd = Random::Uniform();//(double)rand() / ((double)RAND_MAX+1.0);
				if(rnd <= synchronyRate){
					if(not uniformalTemperature) getNeuron(ub->getNeuronIndex(j))->compute(getNeuron(ub->getNeuronIndex(j))->getTemperature());
					else getNeuron(ub->getNeuronIndex(j))->compute(temperature);
				}
			} 
			// When all is done theNewState is set
			for(int j=0; j < ub->getSize(); j++)
							getNeuron(ub->getNeuronIndex(j))->substitute();
		}
		
		else if(ub->getUpdateMethods() == FIXE_1){
			for(int j=0; j < ub->getSize(); j++){
				rnd = Random::Uniform();
				if(rnd <= synchronyRate)
							getNeuron(ub->getNeuronIndex(j))->setState(true);
			}
		}
		
		else if(ub->getUpdateMethods() == FIXE_0){
			for(int j=0; j < ub->getSize(); j++){
				rnd = Random::Uniform();
				if(rnd <= synchronyRate)
							getNeuron(ub->getNeuronIndex(j))->setState(false);
			}
		}
		
		else if(ub->getUpdateMethods() == FIXE_01){
			for(int j=0; j < ub->getSize(); j++){
				rnd = Random::Uniform();
				if(rnd <= synchronyRate)
							getNeuron(ub->getNeuronIndex(j))->setState((bool)(j%2));
			}
		}
		
		else if(ub->getUpdateMethods() == FIXE_10){
			for(int j=0; j < ub->getSize(); j++){
				rnd = Random::Uniform();
				if(rnd <= synchronyRate)
							getNeuron(ub->getNeuronIndex(j))->setState((bool)((j+1)%2));
			}
		}
		
		else if(ub->getUpdateMethods() == RANDOMLY){
			for(int j=0; j < ub->getSize(); j++){
				rnd = Random::Uniform();
				if(rnd <= synchronyRate){
					if(Random::Uniform()<0.5)
							getNeuron(ub->getNeuronIndex(j))->setState(false);
					else
							getNeuron(ub->getNeuronIndex(j))->setState(true);
				}
			}
		}	
	}
}
*/
/*
 * This methods calculate the new global stat of the network using a block parallel update
 
void Network::computeBP(UpdateSchedulingPlan* sp, int nb_iterations, double synchronyRate){
	int nb_blocks = sp->getNb_blocks();
	UpdateBlock* ub;
	double rnd;
	for(int i=0;i< nb_iterations;i++){
			// All neuron compute their new state using a parallele scheme
			for(int j=0; j < nb_blocks; j++){
				ub = sp->getUpdateBlock(j);
				rnd = (double)rand() / ((double)RAND_MAX+1.0);
				if(rnd <= synchronyRate){
					if(ub->getUpdateMethods() == COMPUTE){
						if(not uniformalTemperature) getNeuron(ub->getNeuronIndex(i%ub->getSize()))->compute(getNeuron(ub->getNeuronIndex(i%ub->getSize()))->getTemperature());
						else getNeuron(ub->getNeuronIndex(i%ub->getSize()))->compute(temperature);
					}
					else if(ub->getUpdateMethods() == FIXE_1){
							getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState(true);
					}
					
					else if(ub->getUpdateMethods() == FIXE_0){
						getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState(false);
					}
					
					else if(ub->getUpdateMethods() == FIXE_01){
						getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState((bool)(i%2));
					}
					
					else if(ub->getUpdateMethods() == FIXE_10){
						getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState((bool)((i+1)%2));
					}
					
					else if(ub->getUpdateMethods() == RANDOMLY){
						if(Random::Uniform()<0.5)
							getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState(false);
						else
							getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState(true);
		
					}	
						
				}
			} 
			// When all is done theNewState is set
			for(int j=0; j < nb_blocks; j++){
				ub = sp->getUpdateBlock(j);
				getNeuron(ub->getNeuronIndex(i%ub->getSize()))->substitute();	
			}
			
	}
}

*/
/*
 * The neuron is added to the network and indexed by its creation date
 */
void Network::addNeuron(Neuron* neuron){
	neurons.push_back(neuron);
	neuron->setIndex(nb_neurons);
	nb_neurons++;
}

/*
 * Routine for suppression of a neuron from the network
 */
void Network::delNeuron(int index){
	Neuron * neuron = getNeuron(index);
	if(neuron != NULL){

		// All synapses conncected to the neuron 'll be deleted
		for(int i = 0; i<nb_neurons; i++){
			if(getNeuron(i)->getSynapseByNeighborIndex(index) < getNeuron(i)->getNb_neighbors()){
				getNeuron(i)->delSynapseBySynapseIndex(getNeuron(i)->getSynapseByNeighborIndex(index));
				i--;
			}
		}

		// All the other neuron decrement their indexs
		for(int i = index+1; i<nb_neurons; i++){
			getNeuron(i)->setIndex(i-1);
		}
		
		// Finally the pointer of the neuron is deleted from the vector 
		neurons.erase((vector<Neuron*>::iterator)&neurons[index]);
		delete neuron;
				
		// Decrement the numberr of a neurons
		nb_neurons--;
	}	
}

/*
 * Get the states of all the neurons of the network
 * and return their number
 */
int Network::getState(bool* states) const{
	int i = 0;
	for(vector<Neuron*>::const_iterator iter = neurons.begin(); iter != neurons.end(); iter++){
		states[i] = (*iter)->getState();
		i++;
	}
	return i;
}


/*
 * Draw the synapses then the neurons
 */
void Network::drawMe(QPainter* painter, float scale, int transX, int transY){
	vector<Neuron*>::iterator iter = neurons.begin();
	for(int i=0; i<nb_neurons;i++){
			(*iter)->drawSynapses(painter, scale, transX, transY);
			iter++;
	}
	
	iter = neurons.begin();
	for(int i=0; i<nb_neurons;i++){
		(*iter)->drawMe(painter, scale, transX, transY);
		iter++;
	}
}
