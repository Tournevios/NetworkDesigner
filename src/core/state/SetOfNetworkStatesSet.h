#ifndef SETOFNETWORKSTATESSET_H_
#define SETOFNETWORKSTATESSET_H_

#include "NetworkStatesSet.h"

class SetOfNetworkStatesSet
{
public:
	SetOfNetworkStatesSet();
	SetOfNetworkStatesSet(int nbSets = 2);
	virtual ~SetOfNetworkStatesSet();
	NetworkStatesSet* getNetworkStatesSet(int index) const;
	void addNetworkState(int index, const NetworkState& networkState);
	void deleteOneElement(int index);
private:
	std::vector<NetworkStatesSet*> sets;
	int nbSets;	
};

#endif /*SETOFNETWORKSTATESSET_H_*/
