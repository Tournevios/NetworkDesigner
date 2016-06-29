#ifndef NETWORKSTATESSET_H_
#define NETWORKSTATESSET_H_

#include "NetworkState.h"
#include <vector>
#include <iostream>
 
class NetworkStatesSet
{
public:
	NetworkStatesSet();
	NetworkStatesSet(int maxCardinal=-1);
	NetworkStatesSet(const NetworkStatesSet& cp);
	virtual ~NetworkStatesSet();
	NetworkState* getNetworkState(int index) const;
	void addNetworkState(const NetworkState& networkState);
	void removeNetworkState(int index);
	void addNetworkStatesSet(const NetworkStatesSet& networkStatesSet);
	int getCardinal() const;
	int getMaxCardinal() const;
	void setMaxCardinal(int maxCardinal);
	void setFilled(bool filled);
	bool getFilled() const;
	int getNbOfAllPossibleStates() const;
	int coherent() const;
	
		
	void addAndCompress(const NetworkState& networkState);
	
	void printMe() const;
	
	void deleteOneElement(int index);
	void compress();
	void removeDuplications();
	
	// Extern and friendly Opertators
	friend const NetworkStatesSet operator+(const NetworkStatesSet& lv, const NetworkStatesSet& rv);
	friend const NetworkStatesSet operator*(const NetworkStatesSet& lv, const NetworkStatesSet& rv);

	friend const NetworkStatesSet operator-(const NetworkStatesSet& lv, const NetworkStatesSet& rv);
	friend const NetworkStatesSet operator-(const NetworkStatesSet& lv, const NetworkState& rv);	
	
	// Member increment and decrement operators
	void operator+=(const NetworkState& rv);
	void operator-=(const NetworkState& rv);
	void operator*=(const NetworkState& rv);
		
	void operator+=(const NetworkStatesSet& rv);
	void operator-=(const NetworkStatesSet& rv);
	void operator*=(const NetworkStatesSet& rv);
	
	NetworkStatesSet& operator=(const NetworkStatesSet& rv);
	
	// Extern Boolean operators
	friend int operator==(const NetworkStatesSet& lv, const NetworkStatesSet& rv);
	friend int operator==(const NetworkState& lv, const NetworkStatesSet& rv);
		
private:
	std::vector<NetworkState*> set;
	int cardinal;
	int maxCardinal;
	bool filled;

};
const NetworkStatesSet operator-(const NetworkState& lv, const NetworkState& rv);
#endif /*NETWORKSTATESSET_H_*/
