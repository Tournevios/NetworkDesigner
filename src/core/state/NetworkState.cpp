#include "NetworkState.h"
#include <cmath>

NetworkState::NetworkState(void){
	duplicated = false;
	initialize();
}
NetworkState::~NetworkState(void){
	states.clear();
	nbStates = nullptr;
}

NetworkState::NetworkState(const NetworkState& networkState){
	duplicated = false;
	states.clear();
	for(int i=0; i<networkState.getSize(); i++){
		states.push_back(networkState.getState(i));
	}
	this->nbStates = networkState.getNbStatesNS();
	initialize();
}

NetworkState::NetworkState(int size, NetworkState * nbStates){
	duplicated = false;
	for(int i=0; i<size; i++){
		states.push_back(-1);
		//nbStates.push_back(0);
	}
	this->nbStates = nbStates;
	initialize();
}

NetworkState::NetworkState(Network* network, NetworkState * nbStates){
	duplicated = false;
	for(int i=0; i<network->getNbNeurons(); i++){
		states.push_back(network->getNeuron(i)->getState());
		//nbStates.push_back(network->getNeuron(i)->getNbStates());
	}
	this->nbStates = nbStates;
	initialize();
}


int NetworkState::getSize(void) const{
	return static_cast<int>(states.size());
}

long int NetworkState::getState(int neuronIndex) const{
	if(neuronIndex < static_cast<int>(states.size())) return states[neuronIndex];
	else return -1;
}

void NetworkState::setState(int neuronIndex, long int neuronState){
	if(neuronIndex < static_cast<int>(states.size())) states[neuronIndex] = (long int)neuronState;
}

int NetworkState::getNbStates(int neuronIndex) const{
	if(neuronIndex < nbStates->getSize()) return nbStates->getState(neuronIndex);
	else return 0;
}

void NetworkState::setNbStates(int neuronIndex, int neuronNbStates){
	if(neuronIndex < nbStates->getSize()) nbStates->setState(neuronIndex, neuronNbStates);
}

NetworkState operator+(const NetworkState& lv, const NetworkState& rv){
	long int myState, rState;
	bool modified = false;
	NetworkState result(lv.getSize(), lv.getNbStatesNS());
	//if(this->getSize() != rv.getSize()) return nullptr;;

	for(int i=0; i<lv.getSize(); i++){
		myState = lv.getState(i);
		rState = rv.getState(i);
		if(myState == rState) result.setState(i, rState);
		else if(!modified){
			modified = true;
			if((myState<0) && (rState<0)) result.setState(i,-((-myState)|(-rState)));
			else if((myState<0) && (rState>=0)) result.setState(i,-((-myState)|((long int)pow((double)2,(long int)rState))));
			else if((myState>=0) && (rState<0)) result.setState(i,-((-rState)|((long int)pow((double)2,(long int)myState))));
			else if((myState>=0) && (rState>=0)) result.setState(i,-(((long int)pow((double)2,(long int)rState))|((long int)pow((double)2,(long int)myState))));
		}
		else {
			//delete result;
			//return nullptr;
			for(int j=i; j<lv.getSize(); j++) result.setState(j, -1);
			return result;
		}
		//result->setNbStates(i,(this->getNbStates(i)));
	}
	return result;
}


/*
 * Intersection of two NetworkState NICKEL
 */
NetworkState operator*(const NetworkState& lv, const NetworkState& rv){
	NetworkState result(lv.getSize(), lv.getNbStatesNS());
	long int myState, rState;

	//if(this->getSize() != rv.getSize()) return nullptr;

	for(int i=0; i<lv.getSize(); i++){
		myState = lv.getState(i);
		rState = rv.getState(i);
		if(myState == rState) result.setState(i, rState);
		else{
			if((myState == -1) || (rState == -1)) result.setState(i, -1);
			else if((myState<0) && (rState<0)){
				long int newState = ((-myState)&(-rState));
				if(newState == 0) result.setState(i, -1);
				if(log((double)newState)/log(2.0) == floor(log((double)newState)/log(2.0)))
					result.setState(i, floor(log((double)newState)/log(2.0)));
				else
					result.setState(i, -newState);
			}
			else if((myState<0) && (rState>=0)) {
				long int newState = ((-myState)&((long int)pow((double)2,(long int)rState)));
				if(newState == 0) result.setState(i, -1);
				if(log((double)newState)/log(2.0) == floor(log((double)newState)/log(2.0)))
					result.setState(i, floor(log((double)newState)/log(2.0)));
				else
					result.setState(i, -newState);
			}
			else if((myState>=0) && (rState<0)){
				long int newState = ((-rState)&((long int)pow((double)2,(long int)myState)));
				if(newState == 0) result.setState(i, -1);
				if(log((double)newState)/log(2.0) == floor(log((double)newState)/log(2.0)))
					result.setState(i, floor(log((double)newState)/log(2.0)));
				else
					result.setState(i, -newState);
			}
			else if((myState>=0) && (rState>=0)) result.setState(i, -1);
		}
		//result->setNbStates(i,(this->getNbStates(i)));
	}
	//result->setNbStatesNS(this->getNbStatesNS());
	return result;
}


/*
 * If one of the states in included in the other return true
 *
 */
int operator==(const NetworkState& lv, const NetworkState& rv){
	long int rState;
	long int myState;

	for(int i=0; i<lv.getSize(); i++){
		myState = lv.getState(i);
		rState = rv.getState(i);
		if(myState != rState){
			if((myState>=0) && (rState>=0)) return false;
			else if((myState<0) && (rState>=0))
				if(!(((long int)pow((double)2,rState)) && (-myState))) return false;
			else if((myState>=0) && (rState<0))
				if(!(((long int)pow((double)2,myState)) && (-rState))) return false;
			else if((myState<0) && (rState<0))
				if(((-myState) && !(-rState)) || (!(-myState) && (-rState))) return false;
		}
	}
	return true;
}

int operator<(const NetworkState& lv, const NetworkState& rv){
	long int rState;
	long int myState;

	for(int i=0; i<lv.getSize(); i++){
		myState = lv.getState(i);
		rState = rv.getState(i);
		if(myState != rState){
			if((myState>=0) && (rState>=0)) return false;
			else if((myState<0) && (rState>=0)) return false;
			else if((myState>=0) && (rState<0))
				if(!(((long int)pow((double)2,myState)) & (-rState))) return false;
			else if((myState<0) && (rState<0))
				if(((-myState) & (-rState)) != -myState) return false;
		}
	}
	return true;
}

int operator>(const NetworkState& lv, const NetworkState& rv){
	return(rv < lv);
}

NetworkState& NetworkState::operator=(const NetworkState& rv){
	if(this != &rv){
		nbStates = rv.getNbStatesNS();
		states.clear();
		for(int i=0; i < rv.getSize(); i++){
			states.push_back(rv.getState(i));
		}
	}
	initialize();
	return *this;
}

/*
 * If a prohibited state is in the NetworkState then its considered as incoherent
 */
int NetworkState::coherent() const{
	for(int i=0; i<static_cast<int>(states.size()); i++){
		if(states[i] == -1) return false;
	}
	return true;
}

void NetworkState::printMe() const{
	for(int i=0; i<static_cast<int>(states.size()) - 1; i++){
		std::cout<< states[i] << " ** ";
	}
	std::cout << states[static_cast<int>(states.size()) - 1] << '\n';
}

void NetworkState::setDuplicated(bool duplicated){
	this->duplicated = duplicated;
}

bool NetworkState::getDuplicated(){
	return duplicated;
}

/*
 * Return if a state is **....*...* || no.
 */
int NetworkState::isAllPossibleStates() const{
	for(int i=0; i < static_cast<int>(states.size()); i++){
		if(states[i] != -(static_cast<int>(pow(2.0, nbStates->getState(i))) - 1)) return false;
	}
	return true;
}

NetworkState * NetworkState::getNbStatesNS() const{
	return nbStates;
}

void NetworkState::setNbStatesNS(NetworkState * nbStates){
	this->nbStates = nbStates;
}

/*
 * get the number of all possbile state which could be represented by this network state
 */
int NetworkState::getNbOfAllPossibleStates() const{
	int nbPossible=1;
	int possibleNeuronStates = 0;
	for(int i=0; i<this->getSize();i++){
		if(this->getState(i)<0){
			possibleNeuronStates = 0;
			if(this->getState(i) == -1)	return 0;
			for(int j=0; j < this->getNbStates(i);j++){
				if((static_cast<int>(pow(2.0, static_cast<double>(j)))) & (-this->getState(i))){
					possibleNeuronStates++;
				}
			}
			nbPossible *= possibleNeuronStates;
		}
	}
	return nbPossible;
}

void NetworkState::initialize(){
	visited = false;
	nextOne = -1;
	isAttractor = false;
	attractorNumber = -1;
}

void NetworkState::setVisited(bool visited){
	this->visited = visited;
}

void NetworkState::setNextOne(int nextOne){
	this->nextOne = nextOne;
}

void NetworkState::setIsAttractor(bool isAttractor){
	this->isAttractor = isAttractor;
}

void NetworkState::setAttractorNumber(int attractorNumber){
	this->attractorNumber = attractorNumber;
}

bool NetworkState::isVisited() const{
	return visited;
}

bool NetworkState::isIsAttractor() const{
	return isAttractor;
}

int NetworkState::getNextOne() const{
	return nextOne;
}

int NetworkState::getAttractorNumber() const{
	return attractorNumber;
}
