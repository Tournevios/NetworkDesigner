/*
 * pCalculateTransitions.h
 *
 *  Created on: 18-Jul-2008
 *      Author: hbenamor
 */
/*
#ifndef PCALCULATETRANSITIONS_H_
#define PCALCULATETRANSITIONS_H_
#include <QThread>
#include <QMutex>
#include "SimulationAttractorsAndBasinsOfAttraction2.h"
#include "Computer.h"

class pCalculateTransitions: public QThread {
private:
	SimulationAttractorsAndBasinsOfAttraction2* sim;
	Computer * computer;
	int startIndex;
	int endIndex;
	QMutex mutexProg;
	QMutex mutexTrans;
public:
	pCalculateTransitions();
	pCalculateTransitions(SimulationAttractorsAndBasinsOfAttraction2 * simulation, Computer* computer, int startIndex, int endIndex);
	void run();
	virtual ~pCalculateTransitions();
};
*/
//#endif /* PCALCULATETRANSITIONS_H_ */
