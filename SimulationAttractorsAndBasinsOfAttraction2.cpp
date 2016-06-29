/*
 * SimulationAttractorsAndBasinsOfAttraction2.cpp
 *
 *  Created on: 09-Jul-2008
 *      Author: hbenamor
 */

#include "SimulationAttractorsAndBasinsOfAttraction2.h"
#include "random-singleton.h"
#include <cmath>
using namespace std;

SimulationAttractorsAndBasinsOfAttraction2::SimulationAttractorsAndBasinsOfAttraction2() {
	// TODO Auto-generated constructor stub

}

SimulationAttractorsAndBasinsOfAttraction2::SimulationAttractorsAndBasinsOfAttraction2(Computer * computer):Simulation(computer){
	// TODO Auto-generated constructor stub
	name = "Attractor and basins of attractions V2";
	filepath = "simulation.gnu";
	affinity = 100000;
	computer->setNbIterations(1);
	numberOfIterations = 1;
}

/*
 * Generate all the possible states of the network
 *
 */
void SimulationAttractorsAndBasinsOfAttraction2::generateAllStates(){
	// Generation de tout les états possibles du réseau.
	int numberOfStates;
	totalNumberOfStates = 1;
	nbStates = new NetworkState(computer->getNetwork()->getNbNeurons(), NULL);
	for(int i=0; i< computer->getNetwork()->getNbNeurons(); i++){
		totalNumberOfStates = totalNumberOfStates * computer->getNetwork()->getNeuron(i)->getNbStates();
		nbStates->setState(i, computer->getNetwork()->getNeuron(i)->getNbStates());
	}

	NetworkState tmpNetworkState(computer->getNetwork(), nbStates);
	for(int i=0; i<computer->getNetwork()->getNbNeurons(); i++){
		tmpNetworkState.setState(i, 0);
	}
	allCombinations = new NetworkStatesSet(-1);

	allCombinations->addNetworkState(tmpNetworkState);
	computer->setProgressBarValue(0);
	for(int i=0; i<computer->getNetwork()->getNbNeurons(); i++) {
		numberOfStates = tmpNetworkState.getNbStates(i);
		NetworkStatesSet tmpComb(*allCombinations);
		for(int j=1; j < numberOfStates; j++){
			for(int k=0; k<tmpComb.getCardinal(); k++){
				tmpComb.getNetworkState(k)->setState(i, j);
				allCombinations->addNetworkState(*(tmpComb.getNetworkState(k)));
			}
			//*allCombinations += tmpComb;
		}
	}
}

void SimulationAttractorsAndBasinsOfAttraction2::initializeMTransitions(){
	numberOfVisitedStates = 0;
	for(int i=0; i < attractors.size(); i++)
		for(int j=0; j < attractors.size(); j++)
			mTransitions[i][j] = 0;
}

void SimulationAttractorsAndBasinsOfAttraction2::calculateTransitions(int numberOfThreads){
	pCalculateTransitions * myThreads[100];
	int startIndex;
	int endIndex;
	startIndex = 0;
	endIndex = (totalNumberOfStates / numberOfThreads);
	initializeMTransitions();
	//bool doit = true;
	for(int i=0; i < numberOfThreads; i++){
		myThreads[i] = new pCalculateTransitions(this, computer, startIndex, endIndex);
	    //connect(this, SIGNAL(startThreads(QThread::Priority)), myThreads, SLOT(start(QThread::Priority)));
		startIndex = endIndex+1;
		endIndex = startIndex + (totalNumberOfStates / numberOfThreads);
		if(endIndex > totalNumberOfStates-1) endIndex = totalNumberOfStates - 1;
	}
	for(int i=0; i < numberOfThreads; i++) 	myThreads[i]->start();
	computer->setProgressBarValue(0);
	/*while(doit){
		mutexProg.lock();
			if(numberOfVisitedStates >= totalNumberOfStates) doit = false;
			else
				computer->setProgressBarValue((int)(100*((double)numberOfVisitedStates/(totalNumberOfStates))));
		mutexProg.unlock();
	}*/
	for(int i=0; i < numberOfThreads; i++){
		myThreads[i]->wait();
		myThreads[i]->quit();
		//computer->setProgressBarValue((int)(100*((double)i/numberOfThreads)));
	}
	computer->setProgressBarValue(100);
}

void SimulationAttractorsAndBasinsOfAttraction2::calculateTransitions(){
	int nextOne;
	int nextAttractor;
	int currentAttractor;
	NetworkState tmpNetworkState;
	initializeMTransitions();
	for(int i=0; i < allCombinations->getCardinal(); i++){
		for(int j=0; j < affinity; j++){
			currentAttractor = allCombinations->getNetworkState(i)->getAttractorNumber();
			computer->setStateOfTheNetwork(allCombinations->getNetworkState(i));
			if(updateType == P)		computer->computeP();
			else if(updateType == BS) computer->computeBS();
			else if(updateType == S) computer->computeS();
			else if(updateType == BP) computer->computeBP();
			tmpNetworkState = NetworkState(computer->getNetwork(), nbStates);
			nextOne = indexOf(&tmpNetworkState);
			nextAttractor = allCombinations->getNetworkState(nextOne)->getAttractorNumber();
			mTransitions[currentAttractor][nextAttractor] += 1 / (double)(affinity * sizeOfBasins[currentAttractor]);
		}
		computer->setProgressBarValue((int)(100*((double)i/(totalNumberOfStates))));
	}
	computer->setProgressBarValue(100);
}

void SimulationAttractorsAndBasinsOfAttraction2::run(){
	/*pthread_t threads[100];
	int rcs[100];
	int * parts[100];
*/
	printf("generating\n");
	generateAllStates();
	Network * net = new Network(*(computer->getNetwork()));
	temperature = net->getTemperature();
	Network * tmpNetwork = computer->getNetwork();
	computer->setNetwork(net);
	computer->getNetwork()->setTemperature(0.00);
	numberOfVisitedStates = 0;
	printf("looking for attractors\n");
	for(int i = 0; i < allCombinations->getCardinal();i++){
		if(!(allCombinations->getNetworkState(i)->isVisited())) linkThem(i);
	}
	calculateSizeOfBasins();
	calculateActivityPerBasins();
	for(int i=0; i < attractors.size(); i++){
		printf("size of attractor %d: %d \t \t activity %f%%\n", i+1, sizeOfBasins[i], percentageOfActivity[i] * 100);
	}
	printf("Total states are: %d\n", totalNumberOfStates);
	computer->getNetwork()->setTemperature(temperature);
	/*for(int i=0; i < 10; i++){
		parts[i][0] = i * (totalNumberOfStates/10);
		parts[i][1] = (i+1) * (totalNumberOfStates/10) - 1;
		if((rc1=pthread_create( &threads[i], NULL, &pcalculateTransitions, (void*)parts[i])) )
		 {
		     printf("Thread creation failed: %d\n", rc1);
		 }
	}
	for(int i=0; i < 10; i++){
		pthread_join( threads[i], NULL);
	}*/
	//calculateTransitions();
	computer->setProgressBarValue(0);
	/////////////////////// Le 20/11/2008
	if(temperature > 0){
		calculateTransitions(100);
		for(int i = 0; i < attractors.size(); i++){
			printf("From Attractor %d to: \n",i+1);
			for(int j = 0; j < attractors.size(); j++){
				printf("\t \t \t %d : %f%%\n",j+1, mTransitions[i][j] * 100);
				//printf("\t \t \t %d K: %f\n",j+1, log(pow(pow((double)(mTransitions[i][j] * totalNumberOfStates / (sizeOfBasins[i]*sizeOfBasins[j])), -1/(double)computer->getNetwork()->getNbNeurons()) - 1, - computer->getNetwork()->getTemperature())));
			}
		}
	}

	/////////////////////////////
	computer->setNetwork(tmpNetwork);
/*	for(int j=0; j < attractors.size(); j++){
		attractors[j]->printMe();
	}*/
}

int SimulationAttractorsAndBasinsOfAttraction2::indexOf(NetworkState* networkState){
	int i=0;
	for(int j=0; j<networkState->getSize();j++){
		if(networkState->getState(j)==1) i = i + (int)pow(2.0, j);
	}
	/*while(i < allCombinations->getCardinal()){
			if(*allCombinations->getNetworkState(i) == *networkState) break;
			i++;
	}*/
	return i;
}

void SimulationAttractorsAndBasinsOfAttraction2::attractorFound(int startingIndex){
	NetworkStatesSet * attractor;
	int nextOne;

	attractor = new NetworkStatesSet(-1);
	attractor->addNetworkState(*(allCombinations->getNetworkState(startingIndex)));

	allCombinations->getNetworkState(startingIndex)->setAttractorNumber(attractors.size());
	allCombinations->getNetworkState(startingIndex)->setIsAttractor(true);

	nextOne = allCombinations->getNetworkState(startingIndex)->getNextOne();
	if(nextOne != startingIndex) numberOfCycles++;
	else numberOfFixedPoints++;
	while(nextOne != startingIndex){
		attractor->addNetworkState(*(allCombinations->getNetworkState(nextOne)));
		allCombinations->getNetworkState(nextOne)->setAttractorNumber(attractors.size());
		allCombinations->getNetworkState(nextOne)->setIsAttractor(true);
		nextOne = allCombinations->getNetworkState(nextOne)->getNextOne();
	}
	attractors.push_back(attractor);
	attractor->printMe();
}

void SimulationAttractorsAndBasinsOfAttraction2::setAttractorNumber(int startingIndex, int attractorNumber){
	int index = startingIndex;
	while(allCombinations->getNetworkState(index)->getAttractorNumber()==-1){
		allCombinations->getNetworkState(index)->setAttractorNumber(attractorNumber);
		index = allCombinations->getNetworkState(index)->getNextOne();
	}
}

void SimulationAttractorsAndBasinsOfAttraction2::linkThem(int startingIndex){
	NetworkState * currentState = allCombinations->getNetworkState(startingIndex);
	NetworkState * nextState;
	NetworkState tmpNetworkState;
	while(!(currentState->isVisited())){
		currentState->setVisited(true);
		computer->setStateOfTheNetwork(currentState);

		if(updateType == P)		computer->computeP();
		else if(updateType == BS) computer->computeBS();
		else if(updateType == S) computer->computeS();
		else if(updateType == BP) computer->computeBP();

		tmpNetworkState = NetworkState(computer->getNetwork(), nbStates);
		nextState = allCombinations->getNetworkState(indexOf(&tmpNetworkState));
		numberOfVisitedStates++;
		computer->setProgressBarValue((int)(100*((double)numberOfVisitedStates/(double)totalNumberOfStates)));
		currentState->setNextOne(indexOf(nextState));

		if((nextState->isVisited()) and !(nextState->isIsAttractor()) and (nextState->getAttractorNumber()==-1)){
			attractorFound(indexOf(nextState));
			setAttractorNumber(startingIndex, attractors.size() - 1);
		}
		else if(nextState->isVisited()){
			setAttractorNumber(startingIndex, nextState->getAttractorNumber());
		}
		currentState = nextState;
	}
}

void SimulationAttractorsAndBasinsOfAttraction2::calculateSizeOfBasins(){
	for(int i=0; i < attractors.size(); i++) sizeOfBasins[i]=0;
	for(int i= 0; i < allCombinations->getCardinal(); i++){
		sizeOfBasins[allCombinations->getNetworkState(i)->getAttractorNumber()] ++;
	}
}

SimulationAttractorsAndBasinsOfAttraction2::~SimulationAttractorsAndBasinsOfAttraction2() {
	// TODO Auto-generated destructor stub
}

/*
 * Affinity's setter
 */
void SimulationAttractorsAndBasinsOfAttraction2::setAffinity(int affinity){
	this->affinity = affinity;
}

/*
 * Affinity's getter
 */
int SimulationAttractorsAndBasinsOfAttraction2::getAffinity() const{
	return affinity;
}

/*
 * UpdateType's setter
 */
void SimulationAttractorsAndBasinsOfAttraction2::setUpdateType(int updateType){
	this->updateType = updateType;
}

void SimulationAttractorsAndBasinsOfAttraction2::calculateActivityPerBasins(){
	int attNumber;
	for(int i=0; i < attractors.size(); i++) percentageOfActivity[i]=0;
	for(int i= 0; i < allCombinations->getCardinal(); i++){
		attNumber = allCombinations->getNetworkState(i)->getAttractorNumber();
		percentageOfActivity[attNumber] += numberOfOne(allCombinations->getNetworkState(i)) / (double)(nbStates->getSize() * (sizeOfBasins[attNumber]));
	}
}

int SimulationAttractorsAndBasinsOfAttraction2::numberOfOne(NetworkState * networkState){
	int totalOfOne = 0;
	for(int i=0; i < networkState->getSize(); i++){
		if(networkState->getState(i)==1)totalOfOne++;
	}
	return totalOfOne;
}

/*
 * UpdateType's getter
 */
int SimulationAttractorsAndBasinsOfAttraction2::getUpdateType() const{
	return updateType;
}

void SimulationAttractorsAndBasinsOfAttraction2::tick(){

}

NetworkStatesSet * SimulationAttractorsAndBasinsOfAttraction2::getAllCombinations() const{
	return allCombinations;
}

NetworkState * SimulationAttractorsAndBasinsOfAttraction2::getNbStates() const{
	return nbStates;
}

int * SimulationAttractorsAndBasinsOfAttraction2::getSizeOfBasins() const{

}

double** SimulationAttractorsAndBasinsOfAttraction2::getMTransitions() const{
	return (double**)mTransitions;
}

int SimulationAttractorsAndBasinsOfAttraction2::getTotalNumberOfStates() const{
	return totalNumberOfStates;
}

void SimulationAttractorsAndBasinsOfAttraction2::setNumberOfVisitedStates(int numberOfVisitedStates) {
	this->numberOfVisitedStates = numberOfVisitedStates;
}

int SimulationAttractorsAndBasinsOfAttraction2::getNumberOfVisitedStates() const{
	return numberOfVisitedStates;
}

Computer * SimulationAttractorsAndBasinsOfAttraction2::getComputer() const{
	return computer;
}

void SimulationAttractorsAndBasinsOfAttraction2::setProgressBarValue(int value){
	computer->setProgressBarValue(value);
}

void SimulationAttractorsAndBasinsOfAttraction2::setMTransitions(int i, int j, double value){
	mTransitions[i][j] = value;
}

double SimulationAttractorsAndBasinsOfAttraction2::getMTransitions(int i, int j) const{
	return mTransitions[i][j];
}
