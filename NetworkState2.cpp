/*
 * NetworkState2.cpp
 *
 *  Created on: 09-Jul-2008
 *      Author: hbenamor
 */

#include "NetworkState2.h"

NetworkState2::NetworkState2(void):NetworkState(){
	initialize();
}

NetworkState2::NetworkState2(const NetworkState2& networkState2):NetworkState((NetworkState&)networkState2){
	initialize();
}

NetworkState2::NetworkState2(Network* network, NetworkState * nbStates):NetworkState(network, nbStates){
	initialize();
}

NetworkState2::NetworkState2(int size, NetworkState * nbStates):NetworkState(size, nbStates){
	initialize();
}


NetworkState2::~NetworkState2(){
	states.clear();
	nbStates = NULL;
}

void NetworkState2::initialize(){
	visited = false;
	nextOne = -1;
	isAttractor = false;
	attractorNumber = -1;
}

void NetworkState2::setVisited(bool visited){
	this->visited = visited;
}

void NetworkState2::setNextOne(int nextOne){
	this->nextOne = nextOne;
}

void NetworkState2::setIsAttractor(bool isAttractor){
	this->isAttractor = isAttractor;
}

void NetworkState2::setAttractorNumber(int attractorNumber){
	this->attractorNumber = attractorNumber;
}

bool NetworkState2::isVisited() const{
	return visited;
}

bool NetworkState2::isIsAttractor() const{
	return isAttractor;
}

int NetworkState2::getNextOne() const{
	return nextOne;
}

int NetworkState2::getAttractorNumber() const{
	return attractorNumber;
}
