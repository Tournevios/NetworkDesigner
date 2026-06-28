/*
 * NetworkState2.h
 *
 *  Created on: 09-Jul-2008
 *      Author: hbenamor
 */

#ifndef NETWORKSTATE2_H_
#define NETWORKSTATE2_H_

#include "NetworkState.h"

class NetworkState2: public NetworkState {
public:
	NetworkState2(void);
	NetworkState2(const NetworkState2& networkState);
	NetworkState2(Network* network, NetworkState * nbStates);
	NetworkState2(int size, NetworkState * nbStates);
	virtual ~NetworkState2();

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
	bool visited;
	bool isAttractor;
	int attractorNumber;
	int nextOne;
};

#endif /* NETWORKSTATE2_H_ */
