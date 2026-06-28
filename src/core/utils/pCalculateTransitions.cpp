#include "pCalculateTransitions.h"

pCalculateTransitions::pCalculateTransitions() {}

pCalculateTransitions::~pCalculateTransitions() {
	delete computer;
}

pCalculateTransitions::pCalculateTransitions(SimulationAttractorsAndBasinsOfAttraction2* simulation,
                                              Computer* computer,
                                              int startIndex,
                                              int endIndex)
    : sim(simulation)
    , computer(new Computer(*computer))
    , startIndex(startIndex)
    , endIndex(endIndex)
{}

pCalculateTransitions::pCalculateTransitions(const pCalculateTransitions& other)
    : sim(other.sim)
    , computer(new Computer(*other.computer))
    , startIndex(other.startIndex)
    , endIndex(other.endIndex)
{}

void pCalculateTransitions::run(){
	NetworkState tmpNetworkState;
	for (int i = startIndex; i <= endIndex; ++i) {
		for (int j = 0; j < sim->affinity; ++j) {
			int currentAttractor = sim->allCombinations->getNetworkState(i)->getAttractorNumber();
			computer->setStateOfTheNetwork(sim->allCombinations->getNetworkState(i));

			if      (sim->getUpdateType() == UpdateType::P)  computer->computeP();
			else if (sim->getUpdateType() == UpdateType::BS) computer->computeBS();
			else if (sim->getUpdateType() == UpdateType::S)  computer->computeS();
			else if (sim->getUpdateType() == UpdateType::BP) computer->computeBP();

			tmpNetworkState = NetworkState(computer->getNetwork(), sim->nbStates);
			int nextOne = sim->indexOf(&tmpNetworkState);
			int nextAttractor = sim->allCombinations->getNetworkState(nextOne)->getAttractorNumber();

			mutexTrans.lock();
			sim->setMTransitions(currentAttractor, nextAttractor,
			    sim->getMTransitions(currentAttractor, nextAttractor)
			    + (1.0 / (sim->affinity * sim->sizeOfBasins[currentAttractor])));
			mutexTrans.unlock();
		}

		mutexProg.lock();
		sim->setNumberOfVisitedStates(1 + sim->getNumberOfVisitedStates());
		mutexProg.unlock();
	}
}
