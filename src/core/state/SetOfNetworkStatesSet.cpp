#include "SetOfNetworkStatesSet.h"

SetOfNetworkStatesSet::SetOfNetworkStatesSet()
{
	nbSets = -1;
}

SetOfNetworkStatesSet::~SetOfNetworkStatesSet()
{
	// unique_ptr elements are destroyed automatically
}

void SetOfNetworkStatesSet::deleteOneElement(int index){
	//vector<NetworkStatesSet *> tmpVector;
	if(index < nbSets){
			sets.erase(sets.begin()+index);
		nbSets--;
	}
}

SetOfNetworkStatesSet::SetOfNetworkStatesSet(int nbSets){
	sets.clear();
	this->nbSets = nbSets;
	for(int i=0; i<nbSets; i++){
		sets.push_back(std::make_unique<NetworkStatesSet>(-1));
	}
}

NetworkStatesSet * SetOfNetworkStatesSet::getNetworkStatesSet(int index) const{
	if(index < nbSets){
		return sets[index].get();
	}
	return NULL;
}

void SetOfNetworkStatesSet::addNetworkState(int index, const NetworkState& networkState){
	if(index>=nbSets) return;
	sets[index]->addNetworkState(networkState);
}
