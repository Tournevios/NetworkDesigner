#pragma once

#include "NetworkStatesSet.h"
#include <memory>

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
	std::vector<std::unique_ptr<NetworkStatesSet>> sets;
	int nbSets;	
};
