#ifndef NETWORKSTATE_H_
#define NETWORKSTATE_H_

#include <vector>
#include <cmath>
#include  "Network.h"
#include <iostream>

class NetworkState
{
public:
	NetworkState(void);
	NetworkState(const NetworkState& networkState);
	NetworkState(Network* network, NetworkState * nbStates);
	NetworkState(int size, NetworkState * nbStates);
	virtual ~NetworkState(void);
	long int getState(int neuronIndex) const;
	void setState(int neuronIndex, long int neuronState);
	int getNbStates(int neuronIndex) const;
	void setNbStates(int neuronIndex, int neuronNbStates);
	void setDuplicated(bool duplicated);
	bool getDuplicated();
	NetworkState * getNbStatesNS() const;
	void setNbStatesNS(NetworkState * nbStates);

	friend NetworkState operator*(const NetworkState& lv, const NetworkState& rv);
	friend NetworkState operator+(const NetworkState& lv, const NetworkState& rv);
	friend int operator==(const NetworkState& lState, const NetworkState& rState);
	friend int operator<(const NetworkState& lv, const NetworkState& rv);
	friend int operator>(const NetworkState& lv, const NetworkState& rv);
	NetworkState& operator=(const NetworkState& rv);

	int getSize(void) const;
	void printMe() const;
	int coherent() const;
	int isAllPossibleStates() const;
	int getNbOfAllPossibleStates() const;

	void initialize();

	bool isVisited() const;
	void setVisited(bool visited);

	bool isIsAttractor() const;
	void setIsAttractor(bool isAttractor);

	int getAttractorNumber() const;
	void setAttractorNumber(int attractorNumber);

	int getNextOne() const;
	void setNextOne(int nextOne);

protected:
	std::vector<long int> states;
	NetworkState * nbStates;
	//std::vector<int> nbStates;
	bool duplicated;
	bool visited;
	bool isAttractor;
	int attractorNumber;
	int nextOne;

};

#endif /*NETWORKSTATE_H_*/
