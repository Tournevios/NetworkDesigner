#include "SetOfNetworkStatesSet.h"
using namespace std;

SetOfNetworkStatesSet::SetOfNetworkStatesSet()
{
	nbSets = -1;
}

SetOfNetworkStatesSet::~SetOfNetworkStatesSet()
{
	while(nbSets > 0)	deleteOneElement(0);
}

void SetOfNetworkStatesSet::deleteOneElement(int index){
	//vector<NetworkStatesSet *> tmpVector;
	if(index < nbSets){
		/*if(index == nbSets - 1) {
			delete sets[index];
			sets.erase(sets.begin() + index);
		}
		else{
			tmpVector.clear();
			for(int i=index+1; i < nbSets; i++){
				tmpVector.push_back(sets[i]);
			}*/
			delete sets[index];
			sets.erase(sets.begin()+index); 
			/*sets.erase(sets.begin()+index, sets.end());
			for(int i=index+1; i < nbSets; i++){
				sets.push_back(tmpVector[i - (index + 1)]);
			}
		}*/
		nbSets--;
	}
}

SetOfNetworkStatesSet::SetOfNetworkStatesSet(int nbSets){
	sets.clear();
	this->nbSets = nbSets;
	for(int i=0; i<nbSets; i++){
		sets.push_back(new NetworkStatesSet(-1));
	}
}

NetworkStatesSet * SetOfNetworkStatesSet::getNetworkStatesSet(int index) const{
	if(index < nbSets){
		return sets[index];
	}
	return NULL;
}

void SetOfNetworkStatesSet::addNetworkState(int index, const NetworkState& networkState){
	if(index>=nbSets) return;
	sets[index]->addNetworkState(networkState);
}
