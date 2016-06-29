#include "NeuronSynapse.h"
#include <cmath>
#include "constFile.h"
#include <iostream>
#include "random-singleton.h"


using namespace std;

void Neuron::init(){
	Random::Uniform<double>(0.0,1.0);
	nb_neighbors = 0;
	nbStates = 2;
	threshold.push_back(0.0001);
	hasANewState = false;
	x = 0;
	y = 0;
	selected = true;
	yellowMe = false;
	temperature = 0;
	state = true;
	strcpy(nodeID, "noname");
}

Neuron::Neuron()
{

	init();
/*	color[0] = 0;
	color[1] = 255;
	color[2] = 0;
*/

}

Neuron::Neuron(int index)
{
	init();
	this->index = index;
}

Neuron::Neuron(int index, int state, int nbStates, vector<double> threshold)
{
	init();
	this->index = index;
	this->state = state;
	this->nbStates = nbStates;
	(this->threshold).clear();
	if(nbStates >= 2) {
		for(int i=1; i<nbStates; i++)	(this->threshold).push_back(threshold[i-1]);
	}
}

Neuron::Neuron(const Neuron& neuron){

	nb_neighbors = 0;
	//Synapse * synapse;
	index = neuron.getIndex();
	temperature = neuron.getTemperature();
	state = neuron.getState();
	nbStates = neuron.getNbStates();
	threshold.clear();
	for(int i=1; i<nbStates; i++)	threshold.push_back(neuron.getThreshold(i));
	x = neuron.getX();
	y = neuron.getY();
	selected = true;
	yellowMe = false;
	hasANewState = false;
	theNewState = 0;
	strcpy(nodeID, neuron.getNodeID());

	/*
	 * for(int i=0; i<neuron->getNb_neighbors();i++){
		synapse = new Synapse(this, neuron->getSynapse(i)->getFinalNeuron(), neuron->getSynapse(i)->getWeight(), neuron->getSynapse(i)->getDelay());
		synapse->setCX(neuron->getSynapse(i)->getCX());
		synapse->setCY(neuron->getSynapse(i)->getCY());
		synapses.push(synapse);
		nb_neighbors++;
	}
	 */
}

Neuron::~Neuron()
{
	while(nb_neighbors!=0){
		delete synapses[0];
		//synapses.erase((vector<Synapse*>::iterator)&synapses[0]);
		synapses.erase(synapses.begin());
		nb_neighbors--;
	}
	threshold.clear();
}

int Neuron::getIndex() const{
	return index;
}

void Neuron::setIndex(int index){
	this->index = index;
}

int Neuron::getState() const{
	return state;
}

void Neuron::setState(int state){
	this->state = state;
}

double Neuron::getThreshold(int stateIndex) const{
	if(stateIndex <= 0)	return -1000.00;
	else return threshold[stateIndex - 1];
}

void Neuron::setThreshold(int stateIndex, double thresholdValue){
	if(stateIndex >  0)	threshold[stateIndex - 1] = thresholdValue;
}

int Neuron::getNb_neighbors() const{
	return nb_neighbors;
}

void Neuron::setNb_neighbors(int nb_neighbors){
	this->nb_neighbors = nb_neighbors;
}

void Neuron::setXY(double x, double y){
	setX(x);
	setY(y);
}

void Neuron::setX(double x){
	for(int i=0; i< nb_neighbors; i++){
		synapses[i]->setCX(synapses[i]->getCX() - (this->x - x));
	}
	this->x = x;

}

void Neuron::setY(double y){
	for(int i=0; i< nb_neighbors; i++){
		synapses[i]->setCY(synapses[i]->getCY() - (this->y - y));
	}
	this->y = y;
}

double Neuron::getX() const{
	return x;
}

double Neuron::getY() const{
	return y;
}
/*
void Neuron::setColor(int red, int green, int blue){
	this->color[0] = red;
	this->color[1] = green;
	this->color[2] = blue;
}

int* Neuron::getColor(){
	return color;
}
*/

Neuron * Neuron::getNeighbor(int synapseIndex) const{
	if(synapseIndex < nb_neighbors)
		return synapses[synapseIndex]->getFinalNeuron();
	return NULL;
}

/*
 * Selected's getter
 */
bool Neuron::getSelected() const{
	return selected;
}

/*
 * Selected's setter
 */
void Neuron::setSelected(bool selected){
	this->selected = selected;
}

/*
 * YellowMe's getter
 */
bool Neuron::getYellowMe() const{
	return yellowMe;
}

/*
 * YellowMe's setter
 */
void Neuron::setYellowMe(bool yellowMe){
	this->yellowMe = yellowMe;
}

/*
 * TheNewState's setters
 */
void Neuron::setTheNewState(int theNewState){
	this->theNewState = theNewState;
	hasANewState = true;
}

void Neuron::compute2(double temperature){
		vector<double> sum;
		vector<double> expoTerm;
		vector<double> boltzmannTerm;				// vector of boltzmann term associated to thresholds
		vector<double> transitionProbabilities;		// Transition probabilities from state to others

		sum.push_back(threshold[0] * state);
		sum.push_back(threshold[0] * not(state));
		for(int i = 0; i < nb_neighbors; i++){
			sum[0] += synapses[i]->getWeight() * (not synapses[i]->getStateOfTheFinalNeuron());
			sum[1] += synapses[i]->getWeight() * synapses[i]->getStateOfTheFinalNeuron();
		}

		if(sum[0] >= sum[1]) theNewState = 0;
		else theNewState = 1;

		hasANewState = true;
}

/*
 * Compute the new state of the neuron
 * using the Boltzman law
 */
void Neuron::compute(double temperature){

	vector<double> sum;
	vector<double> expoTerm;
	vector<double> boltzmannTerm;				// vector of boltzmann term associated to thresholds
	vector<double> transitionProbabilities;		// Transition probabilities from state to others
	double rd1; 			// Random number
	double max = 0.0;
	//temperature = temperature * K_BOLTZMANN;
	sum.push_back(0.00);
	for(int i = 0; i < nb_neighbors; i++){
			/*if(synapses[i]->getFinalNeuron()->getIndex() == 9)
				sum[0] += synapses[i]->getWeight();
			else*/
				sum[0] += synapses[i]->getWeight() * synapses[i]->getStateOfTheFinalNeuron();
	}

	for(int i=1; i < nbStates - 1; i++){
		sum.push_back(sum[0]);
		sum[i] -= threshold[i];
		if(sum[i]>max) max = sum[i] + 1;
	}
	sum[0] -= threshold[0];
	if(sum[0]>max) max = sum[0] + 1;


	if(temperature==0){
		//if(index == 3 or index == 0) {theNewState = 1; hasANewState = true; return;}
		theNewState = 0;
		double tmpSum = max;
		for(int i=0; i<nbStates - 1;i++){
			//printf("%d is %f >= %f and state is %d\n", index, tmpSum, sum[i], i+1);
			if((i+1) < (nbStates - 1)){
				if(sum[i] >= 0 and sum[i+1] < 0){
					theNewState = i+1;
					tmpSum = sum[i];
					break;
				}
			}
			else {
				if(sum[i] >= 0)	theNewState = i+1;
				else theNewState = i;
				break;
			}

			/*if((sum[i] <= tmpSum) and (sum[i]>=0)){
				theNewState = i+1;
				tmpSum = sum[i];
			}*/
			//printf("sum[%d] = %f\n", i, sum[i]);
		}

		/*if(state == 0){
			if(sum[0] >= 0)	theNewState = 1;
			else theNewState = 0;
		}
		else if(state < nbStates - 1){
			if(sum[state] >= 0) theNewState = state + 1;
			else if (sum[state - 1] >= 0) theNewState = state;
			else theNewState = state - 1;
		}
		else {
			if(sum[state - 1] >= 0) theNewState = state;
			else theNewState = state - 1;
		}
		*/
	}
	else{
		// Calculus of the boltzmann terms
		boltzmannTerm.push_back(1.0);			// Fictif boltzmann Term for calculus associated to the threshold of the 0 state
		for(int i=0; i < nbStates - 1;i++) {
			expoTerm.push_back(exp(-sum[i] / temperature));
		if(expoTerm[i] == HUGE_VAL) 	boltzmannTerm.push_back(0);
		else						boltzmannTerm.push_back(1/(1 + expoTerm[i]));
		}

		boltzmannTerm.push_back(0.0);			// Fictif boltzmann Term for calculus associated to the threshold of the N+1 state
		// If we are in the state 0 we move up
		// If we're in the last state we move down
		// Else we randomly choose between going up or down

		if(state == 0)	rd1 = 0.1;
		else if(state == nbStates - 1)	rd1 = 0.9;
		else rd1 = Random::Uniform();

		if(rd1 <= 0.5){

			transitionProbabilities.push_back(1-boltzmannTerm[state + 1]);
			for(int i=state+1; i < nbStates; i++){
				transitionProbabilities.push_back(transitionProbabilities[i-1-state]* boltzmannTerm[i - state] * (1 - boltzmannTerm[i+1-state])/(1 - boltzmannTerm[i-state]));
			}

			/*
			transitionProbabilities.push_back(1-boltzmannTerm[state+1]);
			for(int i=state+1; i<nbStates; i++){
				transitionProbabilities.push_back(1-boltzmannTerm[i+1]);
				if(transitionProbabilities[i - state] != 0){
					for(int j=state+1; j<=i; j++){
						transitionProbabilities[i - state] = transitionProbabilities[i - state] * boltzmannTerm[j];
					}
				}
			}
			*/
			// After calculating the probabilies of transitions
			// A new state is set by the lucky wheel's methods
			rd1 = Random::Uniform();
			double s = 0;
			int i = 0;
			while((rd1>s) and (i+state < nbStates)){
				s+=transitionProbabilities[i];
				i++;
			}
			theNewState = i - 1 + state;
			//printf("Hi I m %d my state is %d\n s=%f", index, theNewState, s);

		}
		else{
			while((int)transitionProbabilities.size() < state + 1) transitionProbabilities.push_back(1.00);
			transitionProbabilities[state] = boltzmannTerm[state];
			for(int i = state - 1; i >= 0; i--){
				if(boltzmannTerm[i + 1] == 0){
					transitionProbabilities[i] = boltzmannTerm[i];
					for(int j = i+1; j <= state; j++){
						transitionProbabilities[i] = transitionProbabilities[i] * (1 - boltzmannTerm[j]);
					}
				}
				else {
						transitionProbabilities[i] = transitionProbabilities[i+1] * (1 - boltzmannTerm[i+1]) *  boltzmannTerm[i] / boltzmannTerm[i+1];
				}
			}
			rd1 = Random::Uniform();
			double s = 0;
			int i = 0;
			while((rd1>s) and (i<=state)){
				s+=transitionProbabilities[i];
				i++;
			}
			theNewState = i-1;
		}

	}
		//	Moving up is prior but the probability of staying and moving to an upper state must be satisfied
	hasANewState = true;
}

/*
 * This methods will be called by the updateScheduler
 */
void Neuron::substitute(){
	if(hasANewState) state = theNewState;
	hasANewState = false;
}

/*
 * Add a synapse between two neurons. The synapse has a weight.
 */
bool Neuron::addSynapse(Neuron * neighbor, double weight, int delay){
	//addSynapse(neighbor, weight, 0, 0, 0);
	vector<Synapse*>::iterator iter = synapses.begin();
	while(iter != synapses.end()){
		if((*iter)->getFinalNeuron()->getIndex() == neighbor->getIndex())	return false;
		iter++;
	}

	synapses.push_back(new Synapse(this, neighbor, weight, delay));
	nb_neighbors++;
	return true;
}
/*
void Neuron::addSynapse(Neuron * neighbor, double weight, int red, int green, int blue){
	int color[3] = {red, green, blue};
	neighbors.push_back(neighbor);
	weights.push_back(weight);
	synapsesColor.push_back(color);
	nb_neighbors++;
}
*/

/*
 * Modify the color of the synapse
 */
/*
void Neuron::setSynapseColor(int synapseIndex, int red, int green, int blue){
	if(synapseIndex < nb_neighbors) {
		synapsesColor[synapseIndex][0] = red;
		synapsesColor[synapseIndex][1] = green;
		synapsesColor[synapseIndex][2] = blue;
	}
}
*/
/*
 * Return the color of the synapse
 */
/*
int* Neuron::getSynapseColor(int synapseIndex){
	if(synapseIndex < nb_neighbors) {
			return synapsesColor[synapseIndex];
	}
	return NULL;
}
*/

/*
 * Modify the weight of the synapse
 */
void Neuron::setSynapseWeight(int synapseIndex, double weight){
	if(synapseIndex < nb_neighbors) {
		synapses[synapseIndex]->setWeight(weight);
	}
}

/*
 * Return the weight of the synapse
 */
double Neuron::getSynapseWeight(int synapseIndex) const{
	if(synapseIndex < nb_neighbors) {
		return synapses[synapseIndex]->getWeight();
	}
	return 0;
}

/*
 * Return the synapse's delay
 */
int Neuron::getSynapseDelay(int synapseIndex) const{
	if(synapseIndex < nb_neighbors) {
		return synapses[synapseIndex]->getDelay();
	}
	return 0;
}

/*
 * Set the synapse's delay
 */
void Neuron::setSynapseDelay(int synapseIndex, int synapseDelay){
	if(synapseIndex < nb_neighbors) {
		synapses[synapseIndex]->setDelay(synapseDelay);
	}
}

/*
 * Modify the graphical excentricity of the synapse
 */
void Neuron::setSynapseGE(int synapseIndex, float gE){
	if(synapseIndex < nb_neighbors) {
		synapses[synapseIndex]->setGExcentricity(gE);
	}
}

/*
 * Return the graphical excentricity of the synapse
 */
float Neuron::getSynapseGE(int synapseIndex) const{
	if(synapseIndex < nb_neighbors) {
		return synapses[synapseIndex]->getGExcentricity();
	}
	return 0;
}

/*
 * Return the self synapse of the neuron otherwise return null
 */
Synapse * Neuron::getSelfSynapse() const{
	vector<Synapse*>::const_iterator iter = synapses.begin();
	while(iter!=synapses.end()){
		if((*iter)->getBaseNeuron()==(*iter)->getFinalNeuron()) return *iter;
		iter++;
	}
	return NULL;
}

/*
 * Delete a synapse between two neurons based on the index of the synapse
 */
void Neuron::delSynapseBySynapseIndex(int synapseIndex){
	if(synapseIndex < nb_neighbors)
	{
			synapses.erase((vector<Synapse*>::iterator)&synapses[synapseIndex]);
			nb_neighbors--;
	}
}

/*
 * Get synapse by synapse Index
 */
Synapse * Neuron::getSynapse(int synapseIndex) const{
	if(synapseIndex < nb_neighbors) {
		return synapses[synapseIndex];
	}
	return NULL;
}

/*
 * Get the index of a synapse using the index of the neighbor
 */
int Neuron::getSynapseByNeighborIndex(int neighborIndex) const{
	int synapseIndex = 0;
	vector<Synapse*>::const_iterator synapsesIterator = synapses.begin();

	while(synapsesIterator != synapses.end()){
		if((*synapsesIterator)->getFinalNeuron()->getIndex() != neighborIndex){
			synapsesIterator++;
			synapseIndex++;
		}
		else break;
	}
	return synapseIndex;
}

/*
 * Draw the neuron.
 */
void Neuron::drawMe(QPainter* painter, double scale, double transX, double transY){
	// Draw The Neuron
			painter->setRenderHint(QPainter::Antialiasing, true);
			if(yellowMe)
				painter->setPen(QPen(Qt::yellow, 3*scale, Qt::SolidLine, Qt::RoundCap));
			else if(!selected)
				painter->setPen(QPen(Qt::black, 3*scale, Qt::SolidLine, Qt::RoundCap));
			else
				painter->setPen(QPen(Qt::blue, 3*scale, Qt::SolidLine, Qt::RoundCap));

			if(state > 1)
				painter->setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
			else if(state == 1)
				painter->setBrush(QBrush(Qt::green, Qt::SolidPattern));
			else
				painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));

			painter->drawEllipse(QRectF((x-10)*scale + transX, (y-10)*scale + transY, 20*scale, 20*scale));
}

/*
 * Draw the synapses
 */
void Neuron::drawSynapses(QPainter* painter, double scale, double transX, double transY){
	vector<Synapse*>::iterator iter = synapses.begin();
	while(iter != synapses.end()){
		(*iter)->drawMe(painter, scale, transX, transY);
		 iter++;
	}
}

/*
 * Temeprature's getter
 */
double Neuron::getTemperature() const{
	return temperature;
}

/*
 * Temperature's setter
 */
void Neuron::setTemperature(double temperature){
	this->temperature = temperature;
}

/*
 * Refresh the synapse
 */
void Neuron::refreshSynapses(){
	vector<Synapse*>::iterator iter = synapses.begin();
	while(iter != synapses.end()){
		(*iter)->refreshMe();
		 iter++;
	}
}

/*
 * NbStates'setter
 */
void Neuron::setNbStates(int nbStates){
	if((int)threshold.size() > nbStates - 1){
		while((int)threshold.size() > nbStates - 1) threshold.pop_back();
	}
	else {
		if((int) threshold.size() < nbStates - 1){
			while((int)threshold.size() < nbStates - 1) {
				if((int)threshold.size() == 0) threshold.push_back(0);
				else	threshold.push_back(threshold[threshold.size() - 1]);
			}
		}
	}
	this->nbStates = nbStates;
}

/*
 * NbStates's getter
 */
int Neuron::getNbStates() const{
	return nbStates;
}

/*
 * Return all the thresholds
 */
vector<double> Neuron::getThresholds() const{
	return threshold;
}

/*
 * Setter of the nodeID
 */
void Neuron::setNodeID(char * nodeID){
	strcpy(this->nodeID, nodeID);
}

/*
 * Getter for the nodeID
 */
const char * Neuron::getNodeID() const{
	return nodeID;
}

