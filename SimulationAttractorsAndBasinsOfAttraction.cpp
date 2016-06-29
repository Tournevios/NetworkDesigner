#include "SimulationAttractorsAndBasinsOfAttraction.h"
#include "random-singleton.h"
using namespace std;

SimulationAttractorsAndBasinsOfAttraction::SimulationAttractorsAndBasinsOfAttraction(Computer * computer):Simulation(computer)
{
	name = "Attractor and basins of attractions";
	filepath = "simulation.gnu";
	affinity = computer->getNetwork()->getNbNeurons() * 2;
	numberOfIterations = computer->getNbIterations();
	//temperature = 0;
}

SimulationAttractorsAndBasinsOfAttraction::~SimulationAttractorsAndBasinsOfAttraction()
{
	for(int i=0; i<(int)basinsOfAttraction.size(); i++){
		delete basinsOfAttraction[i];
	}
	for(int i=0; i<(int)solutions.size(); i++){
		delete solutions[i];
	}
	for(int i=0; i<(int)attractors.size(); i++){
		delete attractors[i];
	}
	delete nbStates;
	solutions.clear();
	basinsOfAttraction.clear();
	attractors.clear();
	printf("deleted\n");
}

void SimulationAttractorsAndBasinsOfAttraction::run(){

	numberOfIterations = computer->getNbIterations();

	solutions.clear();
	totalNumberOfStates = 1;
	numberOfVisitedStates = 0;

	NetworkState * initialAllStates;
	nonExploredSpace = new NetworkStatesSet(-1);

	nbStates = new NetworkState(computer->getNetwork()->getNbNeurons(), NULL);

	for(int i=0; i< computer->getNetwork()->getNbNeurons(); i++){
		totalNumberOfStates = totalNumberOfStates * computer->getNetwork()->getNeuron(i)->getNbStates();
		solutions.push_back(new SetOfNetworkStatesSet(computer->getNetwork()->getNeuron(i)->getNbStates()));
		nbStates->setState(i, computer->getNetwork()->getNeuron(i)->getNbStates());
	}

	initialAllStates = new NetworkState(computer->getNetwork()->getNbNeurons(), nbStates);
	for(int i=0; i < initialAllStates->getSize(); i++){
		initialAllStates->setState(i, -((int)pow(2.0, (double)initialAllStates->getNbStates(i))-1));
	}
	nonExploredSpace->addNetworkState(*initialAllStates);

	attractors.clear();
	basinsOfAttraction.clear();

	//NetworkStatesSet * basin = new NetworkStatesSet(-1);
	//basinsOfAttraction.push_back(basin);

	//visitedStates = new NetworkStatesSet(-1);;
	stateBuffer = new NetworkStatesSet(affinity);
	stateBuffer->addNetworkState(NetworkState(computer->getNetwork(), nbStates));

	computer->setNetwork(new Network(*initialNetwork));
	computer->getNetwork()->setTemperature(temperature);
	computer->setProgressBarValue(0);
	printf("=============================================================================\n");

	NetworkState test1(3, nbStates);
	NetworkState test2(3, nbStates);
	NetworkState test3(3, nbStates);
	NetworkState test4(3, nbStates);

	NetworkStatesSet test5(-1);
	NetworkStatesSet test6(-1);
	NetworkStatesSet test7(-1);

	test1.setState(0 ,-3);
	test1.setState(1 ,0);
	test1.setState(2 ,1);

	test2.setState(0 ,-3);
	test2.setState(1 ,1);
	test2.setState(2 ,-3);

	test5.addNetworkState(test1);
	test5.addNetworkState(test2);

	/*
	printf("hey\n");
	test5.printMe();
	printf("sans dup\n");
	test5.removeDuplications();
	printf("you never get herer\n");
	test5.printMe();
	*/

	test3.setState(0 ,1);
	test3.setState(1 ,0);
	test3.setState(2 ,1);

	test4.setState(0 ,0);
	test4.setState(1 ,1);
	test4.setState(2 ,0);

	test6.addNetworkState(test3);
	test6.addNetworkState(test4);

	if(test3 < test4) printf("test false!!!\n");
	test7= test5 - test6;
	test5.printMe();
	printf("-\n");
	test6.printMe();
	printf("shouf\n");
	test7.printMe();
	printf("end result\n");
	int dfg;
	scanf("haha %d\n", &dfg);

	/*if(test3 < test1) printf("test3 < test1\n");
	else if(test1 < test3) printf("test3 > test1\n");
	else printf("NO Relation between test3 and test1\n");

	NetworkState testAtt(computer->getNetwork(), nbStates);
	testAtt.setState(0,0);
	testAtt.setState(1,0);
	testAtt.setState(2,0);
	testAtt.setState(3,0);
	testAtt.setState(4,0);
	testAtt.setState(5,0);
	testAtt.setState(6,0);
	testAtt.setState(7,0);
	testAtt.setState(8,1);
	testAtt.setState(9,0);
	testAtt.setState(10,0);
	testAtt.setState(11,0);

	printf("predecessorOf \n");
	testAtt.printMe();
	NetworkStatesSet * aLitt = new NetworkStatesSet(-1);
	aLitt->addNetworkState(&testAtt);
	NetworkStatesSet * testMeNow = pBackward(aLitt);
	*testMeNow -= *aLitt;
	testMeNow->printMe();
	printf("and in a compressed mode I am\n");
	testMeNow->compress();
	testMeNow->printMe();
	delete testMeNow;
	printf("))))))))))))))\n");
	//(test2-test3).printMe();
*/
	if(updateType == P)		computer->computeP();
	else if(updateType == BS) computer->computeBS();
	else if(updateType == S) computer->computeS();
	else if(updateType == BP) computer->computeBP();

	bool exist;
	while(numberOfVisitedStates != totalNumberOfStates){
		NetworkState * rndNetworkState = getRandomNetworkState();
		//NetworkState * rndNetworkState = getANonVisitedNetworkState();
		if(numberOfVisitedStates < totalNumberOfStates)
			computer->setProgressBarValue(100*((double)numberOfVisitedStates/(double)totalNumberOfStates));
		else computer->setProgressBarValue(100.0);
		//printf("total = %f", (100*((double)numberOfVisitedStates/(double)totalNumberOfStates)));

		exist = false;
		for(int i=0; i<(int)basinsOfAttraction.size(); i++){
			if(*rndNetworkState == *basinsOfAttraction[i]){
				exist = true;
				break;
			}
		}

		//int num;
		//scanf("help %d\n", &num);

		if(!exist){
			for(int i=0; i<computer->getNetwork()->getNbNeurons(); i++){
				computer->getNetwork()->getNeuron(i)->setState(rndNetworkState->getState(i));
			}

		//delete stateBuffer;

			stateBuffer = new NetworkStatesSet(affinity);

			stateBuffer->addNetworkState(NetworkState(computer->getNetwork(), nbStates));

			if(updateType == P)		computer->computeP();
			else if(updateType == BS) computer->computeBS();
			else if(updateType == S) computer->computeS();
			else if(updateType == BP) computer->computeBP();
		}
		delete rndNetworkState;

		//printf("condition %d < %d\n", numberOfVisitedStates, totalNumberOfStates);
	}
	/*for(int i=0; i<(int)basinsOfAttraction.size(); i++){
		printf("----------------attractor of cycle %d------------------------------------\n", attractors[i]->getCardinal());
		attractors[i]->printMe();
		printf("++++++++++++++++basins length %d+++++++++++++++++++++++++++++++++++++++++\n", basinsOfAttraction[i]->getCardinal());
		//basinsOfAttraction[i]->printMe();
	}*/
	printf("I visited = %d of a total = %d\n", numberOfVisitedStates,totalNumberOfStates);
	printf("=============================================================================\n");
	printf("occuped memory is %f\n", (double)(numberOfVisitedStates * initialNetwork->getNbNeurons() * 4) / (double)1024);
	computer->setProgressBarValue((double)100.0);
	computer->setNetwork(initialNetwork);
}

NetworkState * SimulationAttractorsAndBasinsOfAttraction::getRandomNetworkState(){
	NetworkState * randomNetworkState = new NetworkState(computer->getNetwork(), nbStates);
	double rnd;
	for(int i=0; i < randomNetworkState->getSize(); i++){
		rnd = Random::Uniform();
		//if(i == 3 or i==0) randomNetworkState->setState(i, 1);
		//else {
			for(int j=0; j<randomNetworkState->getNbStates(i); j++){
				if(rnd <= ((double)(j+1)/(double)(randomNetworkState->getNbStates(i)))){
					randomNetworkState->setState(i, j);
					break;
				}
			}
		//}
	}
	return randomNetworkState;
}

NetworkState * SimulationAttractorsAndBasinsOfAttraction::getANonVisitedNetworkState(){

	if(nonExploredSpace->getCardinal() == 0) return NULL;
	NetworkState * nonVisited = new NetworkState(*(nonExploredSpace->getNetworkState(0)));

	for(int i=0; i < nonVisited->getSize(); i++){
		if(nonVisited->getState(i) < -1){
			for(int j=0; j<nonVisited->getNbStates(i); j++){
				if(-(nonVisited->getState(i)) & (long int)pow(2.0, (double)j)){
					nonVisited->setState(i, j);
					break;
				}
			}
		}
	}
	return nonVisited;
}
/*
 * Affinity's setter
 */
void SimulationAttractorsAndBasinsOfAttraction::setAffinity(int affinity){
	this->affinity = affinity;
}

/*
 * Affinity's getter
 */
inline int SimulationAttractorsAndBasinsOfAttraction::getAffinity() const{
	return affinity;
}

/*
 * UpdateType's setter
 */
void SimulationAttractorsAndBasinsOfAttraction::setUpdateType(int updateType){
	this->updateType = updateType;
}

/*
 * UpdateType's getter
 */
inline int SimulationAttractorsAndBasinsOfAttraction::getUpdateType() const{
	return updateType;
}

void SimulationAttractorsAndBasinsOfAttraction::tick(){
	NetworkState networkState(computer->getNetwork(), nbStates);
	NetworkStatesSet * attractor;
	for(int i=0; i < stateBuffer->getCardinal(); i++){
		if(networkState == *(stateBuffer->getNetworkState(i))){
			attractor = new NetworkStatesSet(stateBuffer->getCardinal() - i);
			for(int j=i; j<stateBuffer->getCardinal(); j++){
				attractor->addNetworkState(*(stateBuffer->getNetworkState(j)));
			}
			computer->stopComputing();
			delete stateBuffer;
			printf("----------------attractor of cycle %d------------------------------------\n", attractor->getCardinal());
			attractor->printMe();
			attractors.push_back(attractor);
			basinsOfAttraction.push_back(pBackward(attractor));
			//printf("evaluating the nonExploredSpace\n");

			//*nonExploredSpace -= *(basinsOfAttraction[(int)basinsOfAttraction.size()-1]);
			//nonExploredSpace->compress();
			printf("*********************************************\n");
			//basinsOfAttraction[(int)basinsOfAttraction.size() - 1]->printMe();
			printf("++++++++++++++++basins length %d+++++++++++++++++++++++++++++++++++++++++\n", basinsOfAttraction[(int)basinsOfAttraction.size()-1]->getNbOfAllPossibleStates());
			//basinsOfAttraction[(int)basinsOfAttraction.size()-1]->printMe();
			//printf("the inexplored is\n");
			//nonExploredSpace->printMe();
			/*
			 * Garbage Collecting
			 */
			/*
			while(basinsOfAttraction[(int)basinsOfAttraction.size()-1]->getCardinal() > 0){
				//delete basinsOfAttraction[(int)basinsOfAttraction.size()-1]->getNetworkState(0);
				basinsOfAttraction[(int)basinsOfAttraction.size()-1]->removeNetworkState(0);
			}
			delete basinsOfAttraction[(int)basinsOfAttraction.size()-1];
			basinsOfAttraction.clear();
			*/
			return;
		}
	}
	/*if(stateBuffer->getMaxCardinal() == stateBuffer->getCardinal()){
		delete stateBuffer->getNetworkState(0);
	} */
	stateBuffer->addNetworkState(networkState);
	//basinsOfAttraction[(int)basinsOfAttraction.size()-1]->addNetworkState(networkState);
	//visitedStates->addNetworkState(networkState);

}

/*
 * This function return a set of the predecessors of X according to the update schedule
 */
NetworkStatesSet * SimulationAttractorsAndBasinsOfAttraction::predecessorOf(NetworkState* x){
	NetworkStatesSet * result = NULL;

	if(updateType == BS){
	}
	else if(updateType == P){
		result = pPredecessorOf(x);
	}
	else if(updateType == BP){
	}
	else if(updateType == S){
	}
	return result;

}


/*
 * This function return a set of the predecessors of X when using the parallele updating scheme
 */
NetworkStatesSet * SimulationAttractorsAndBasinsOfAttraction::pPredecessorOf(NetworkState* x){

	NetworkStatesSet * result = NULL;
	if(!(x->coherent())) return new NetworkStatesSet(-1);

	for(int i=0; i < computer->getNetwork()->getNbNeurons(); i++){
		if(x->getState(i) >= 0 ){				// Temporary we suppose that the state is >=0 or equal to the superposition of all possible states
			if(!(solutions[i]->getNetworkStatesSet(x->getState(i))->getFilled())){
				solveAndStore(i);
			}
			if(result==NULL){
				result = new NetworkStatesSet(*(solutions[i]->getNetworkStatesSet(x->getState(i))));
				if(!(result->coherent())){
					*result -= *result;
					return result;
				}
			}
			else{
				/*NetworkStatesSet * tmp = new NetworkStatesSet(*(solutions[i]->getNetworkStatesSet(x->getState(i))));
				 if(!(tmp->coherent())){
					delete tmp;
					*result -= *result;
					return result;
				 }*/

				 if(!(solutions[i]->getNetworkStatesSet(x->getState(i)))){
					*result -= *result;
					return result;
				 }
				 else{
				 	*result *= *solutions[i]->getNetworkStatesSet(x->getState(i));
				 }
				 /*else{
				 	*result *= *tmp;
				 	delete tmp;
				 }*/
			}
		}
	}
	int countMe = 0;
	while(countMe < result->getCardinal()){
		if(!(result->getNetworkState(countMe)->coherent())){
			//delete result->getNetworkState(countMe);
			result->removeNetworkState(countMe);
		}
		else countMe++;
	}
	result->compress();
	result->removeDuplications();
	return result;
}

/*
 * This function found all the solutions of the equation
 */
void SimulationAttractorsAndBasinsOfAttraction::solveAndStore(int neuronIndex){
	if(neuronIndex < computer->getNetwork()->getNbNeurons()){
		Neuron * tmpNeuron = new Neuron(*(computer->getNetwork()->getNeuron(neuronIndex)));
		tmpNeuron->setState(0);

		if(computer->getNetwork()->getNeuron(neuronIndex)->getNb_neighbors() == 0){
			// Eden garden
			NetworkState allStates(computer->getNetwork(), nbStates);
			NetworkState noStates(computer->getNetwork(), nbStates);
			for(int i=0; i<allStates.getSize(); i++){
				allStates.setState(i, -((long int)pow(2.0, (double)allStates.getNbStates(i))-1));
				noStates.setState(i, -1);
			}
			tmpNeuron->compute(temperature);
			tmpNeuron->substitute();
			int relaxState = tmpNeuron->getState();
			for(int i=0; i<computer->getNetwork()->getNeuron(neuronIndex)->getNbStates(); i++){
				if(i != relaxState)
					solutions[neuronIndex]->addNetworkState(i, noStates);
				else
					solutions[neuronIndex]->addNetworkState(i, allStates);

				solutions[neuronIndex]->getNetworkStatesSet(i)->setFilled(true);
			}
			delete tmpNeuron;
			return;
		}
		// begin by copying the neuron and all of his neighbors
		vector<Neuron*> neighbors;
		NetworkState tmpNetworkState(computer->getNetwork(), nbStates);
		NetworkStatesSet * allCombinations = new NetworkStatesSet(-1);
		for(int i=0; i< computer->getNetwork()->getNeuron(neuronIndex)->getNb_neighbors(); i++){
			neighbors.push_back(new Neuron(*(computer->getNetwork()->getNeuron(neuronIndex)->getSynapse(i)->getFinalNeuron())));
			neighbors[i]->setState(0);
			tmpNeuron->addSynapse(neighbors[i], computer->getNetwork()->getNeuron(neuronIndex)->getSynapse(i)->getWeight(), computer->getNetwork()->getNeuron(neuronIndex)->getSynapse(i)->getDelay());
		}

		for(int i=0; i<computer->getNetwork()->getNbNeurons(); i++){
			bool inNeighbors = false;
			int j=0;
			while(j<(int)neighbors.size()) {
				if(i == neighbors[j]->getIndex()){
					 inNeighbors = true;
					 break;
				}
				j++;
			}
			if(!inNeighbors)
				tmpNetworkState.setState(i, -((pow(2.0, (int)computer->getNetwork()->getNeuron(i)->getNbStates()))-1));
			else
				tmpNetworkState.setState(i, 0);
		}

		allCombinations->addNetworkState(tmpNetworkState);

		// Resoudre l'equation en generant toutes les possibilités.
		int numberOfStates;
		int l;
		for(int i=0; i<(int)neighbors.size(); i++) {
			numberOfStates = neighbors[i]->getNbStates();
			l = neighbors[i]->getIndex();
			NetworkStatesSet tmpComb(*allCombinations);
			for(int j=1; j < numberOfStates; j++){
				for(int k=0; k<tmpComb.getCardinal(); k++){
					tmpComb.getNetworkState(k)->setState(l, j);
					allCombinations->addNetworkState(*(tmpComb.getNetworkState(k)));
				}
				//*allCombinations += tmpComb;
			}
			/*while(tmpComb->getCardinal()>0){
				//delete tmpComb->getNetworkState(0);
				tmpComb->removeNetworkState(0);
			}*/
		}


		// Effectuer les test sur les etats generer
		for(int i=0; i<(int)allCombinations->getCardinal(); i++){
			for(int j=0; j< (int)neighbors.size(); j++){
				neighbors[j]->setState(allCombinations->getNetworkState(i)->getState(neighbors[j]->getIndex()));
			}
			tmpNeuron->setState(allCombinations->getNetworkState(i)->getState(neuronIndex));
			tmpNeuron->compute(temperature);
			tmpNeuron->substitute();
			solutions[neuronIndex]->addNetworkState(tmpNeuron->getState(), *allCombinations->getNetworkState(i));
		}
		for(int i=0; i < tmpNeuron->getNbStates(); i++){
						solutions[neuronIndex]->getNetworkStatesSet(tmpNeuron->getState())->setFilled(true);
						solutions[neuronIndex]->getNetworkStatesSet(tmpNeuron->getState())->compress();
						solutions[neuronIndex]->getNetworkStatesSet(tmpNeuron->getState())->removeDuplications();
		}
		delete tmpNeuron;
		for(int i=0; i < (int)neighbors.size(); i++) delete neighbors[i];
		neighbors.clear();
	}
}

/*
 * Backward till the stack a_traiter is empty or a allPossibleStates is riched
 *
 */
NetworkStatesSet * SimulationAttractorsAndBasinsOfAttraction::pBackward(NetworkStatesSet * attractor){
	NetworkStatesSet * a_traiter = new NetworkStatesSet(-1);
	vector<int> distances;
	NetworkState * currentNetworkState;
	NetworkStatesSet * result = new NetworkStatesSet(*attractor);
	result->setMaxCardinal(-1);
	/*numberOfVisitedStates += result->getNbOfAllPossibleStates();
	if(numberOfVisitedStates < totalNumberOfStates)
			computer->setProgressBarValue(100*((double)numberOfVisitedStates/(double)totalNumberOfStates));
	else computer->setProgressBarValue(100.0);*/
	NetworkStatesSet * tmpResult = NULL;

	int initNumber = numberOfVisitedStates;
	if(numberOfVisitedStates < totalNumberOfStates)
			computer->setProgressBarValue(100*((double)numberOfVisitedStates/(double)totalNumberOfStates));
	else computer->setProgressBarValue(100.0);

	*a_traiter += *attractor;
	for(int i=0; i < a_traiter->getNbOfAllPossibleStates(); i++) {
		distances.push_back(0);
	}
	int currentDistance =  0;
	int maxDistance = 0;
	int sumOfDistance = 0;
	double rateOfDistance = 0.0;

	while(a_traiter->getCardinal()>0){
		currentNetworkState = a_traiter->getNetworkState(0);
		if(a_traiter->getCardinal() != (int)distances.size()) printf("not equal\n");
		currentDistance = distances[0];
		if(currentDistance > maxDistance) maxDistance = currentDistance;
		sumOfDistance += (currentDistance * currentNetworkState->getNbOfAllPossibleStates());

		distances.erase(distances.begin());

		tmpResult = pPredecessorOf(currentNetworkState);
		*tmpResult -= *attractor;
		*tmpResult -= *currentNetworkState;
		*tmpResult -= *a_traiter;
		a_traiter->removeNetworkState(0);
		tmpResult->removeDuplications();
		for(int i=0; i < tmpResult->getCardinal(); i++){
				a_traiter->addNetworkState(*(tmpResult->getNetworkState(i)));
				distances.push_back(currentDistance + 1);
				//int initCardi = result->getCardinal();
				result->addAndCompress(*(tmpResult->getNetworkState(i)));
				result->removeDuplications();
				//numberOfVisitedStates += (result->getCardinal() - initCardi);
				numberOfVisitedStates = initNumber + result->getNbOfAllPossibleStates();
				//result->addNetworkState(new NetworkState(*(tmpResult->getNetworkState(i))));
				//numberOfVisitedStates += tmpResult->getNetworkState(i)->getNbOfAllPossibleStates();
				if(numberOfVisitedStates < totalNumberOfStates)
					computer->setProgressBarValue(100*((double)numberOfVisitedStates/(double)totalNumberOfStates));
				else computer->setProgressBarValue(100.0);
		}
		delete tmpResult;
	}
	delete a_traiter;
	//	numberOfVisitedStates = numberOfVisitedStates + result->getNbOfAllPossibleStates();
	rateOfDistance = sumOfDistance / (double) result->getNbOfAllPossibleStates();
	printf("MAX DISTANCE = %d, AVERAGE DISTANCE = %f\n",  maxDistance, rateOfDistance);
	result->removeDuplications();
	numberOfVisitedStates = initNumber + result->getNbOfAllPossibleStates();
	if(numberOfVisitedStates < totalNumberOfStates)
		computer->setProgressBarValue(100*((double)numberOfVisitedStates/(double)totalNumberOfStates));
	else computer->setProgressBarValue(100.0);
	return result;
}


