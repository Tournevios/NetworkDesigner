/*
 * pCalculateTransitions.cpp
 *
 *  Created on: 18-Jul-2008
 *      Author: hbenamor
 */

#include "SimulationAttractorsAndBasinsOfAttraction2.h"

pCalculateTransitions::pCalculateTransitions() {
	// TODO Auto-generated constructor stub

}

pCalculateTransitions::~pCalculateTransitions() {
	// TODO Auto-generated destructor stub
}

pCalculateTransitions::pCalculateTransitions(SimulationAttractorsAndBasinsOfAttraction2 * simulation, Computer * computer, int startIndex, int endIndex){
	this->sim = simulation;
	this->computer = new Computer(*computer);
	this->startIndex = startIndex;
	this->endIndex = endIndex;
}

/*pCalculateTransitions::pCalculateTransitions(const pCalculateTransitions& pcalculateTransitions): QThread((const QThread&) *this){
	this->sim = pcalculateTransitions.sim;
	this->computer = new Computer(*pcalculateTransitions.computer);
	this->startIndex = pcalculateTransitions.startIndex;
	this->endIndex = pcalculateTransitions.endIndex;
}*/

void pCalculateTransitions::run(){
	int nextOne;
	int nextAttractor;
	int currentAttractor;
	NetworkState tmpNetworkState;
	for(int i=startIndex; i <= endIndex; i++){
		for(int j=0; j < sim->affinity; j++){
			currentAttractor = sim->allCombinations->getNetworkState(i)->getAttractorNumber();
			computer->setStateOfTheNetwork(sim->allCombinations->getNetworkState(i));
			if(sim->getUpdateType() == P)		computer->computeP();
			else if(sim->getUpdateType() == BS) computer->computeBS();
			else if(sim->getUpdateType() == S) computer->computeS();
			else if(sim->getUpdateType() == BP) computer->computeBP();
			tmpNetworkState = NetworkState(computer->getNetwork(), sim->nbStates);
			nextOne = sim->indexOf(&tmpNetworkState);
			nextAttractor = sim->allCombinations->getNetworkState(nextOne)->getAttractorNumber();
			mutexTrans.lock();
				sim->setMTransitions(currentAttractor, nextAttractor, sim->getMTransitions(currentAttractor, nextAttractor) + (1 / (double)(sim->affinity * sim->sizeOfBasins[currentAttractor])));
			mutexTrans.unlock();
		}
		//printf("My Starting index is: startIndex: %d\n", startIndex);
		mutexProg.lock();
			sim->setNumberOfVisitedStates(1 + sim->getNumberOfVisitedStates());
			//sim->setProgressBarValue((int)(100*((double)sim->numberOfVisitedStates/(sim->totalNumberOfStates))));
		mutexProg.unlock();
	}
}
