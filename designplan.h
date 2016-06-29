/*****************************************************************************/
/* This widget is a child of the QFrame and it permits the designing of a 	 */
/* neural network by handling events coming from the mouse and the keyboard	 */
/*																			 */
/* 																			 */
/*****************************************************************************/
#ifndef DESINGPLAN_H_
#define DESINGPLAN_H_
#include <QtGui/QFrame>
#include<QtGui/QMouseEvent>
#include<QtGui/QWidget>
#include "Network.h"
#include "NetworkDesignerParser.h"
#include <vector>

class DesignPlan : public QFrame
{
	Q_OBJECT
public:

	NetworkDesignerParser * ndp;

	DesignPlan(QWidget* parent);
	~DesignPlan();

	Network * getNetwork() const;
	void setNetwork(Network* network);

	UpdateSchedulingPlan * getUpdateSchedulingPlan() const;
	void setUpdateSchedulingPlan(UpdateSchedulingPlan * updateSchedulingPlan);


	void save(QString path);
	void load(QString path);
	void exportToGNBoxPremodel(QString path);


	void setDefaultThresholds(const Neuron * neuron);
	void setDefaultThreshold(int stateIndex, double threshold);
	void setDefaultNbStates(int nbStates);
	void setDefaultState(int state);
	void setDefaultWeight(double weight);

	void setDefaultTemperature(double temeperature);
	void setDefaultDelay(int delay);

	void setCurrentUpdateBlock(int currentUpdateBlock);

signals:
	void neuronSelectedChanged(Neuron* neuron);
	void synapseSelectedChanged(Synapse* synapse);
	void blockModeInvoked(int index);
	void updateCalled();
	void networkIsModified();

protected:

	void paintEvent(QPaintEvent * event);

	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void wheelEvent(QWheelEvent * event);
	void keyPressEvent( QKeyEvent * event );

private:

	double scale;
	double transX, transY;
	double midX, midY;
	Neuron* currentNeuron;
	Synapse * currentSynapse;
	Neuron* synapseBaseNeuron;
	double mouseX, mouseY;

	int currentUpdateBlock;

	std::vector<double> defaultThreshold;
	int defaultNbStates;
	int defaultState;
	double defaultWeight;
	double defaultTemperature;
	int defaultDelay;

	bool loaded;

	Network * network;
	UpdateSchedulingPlan * updateSchedulingPlan;
};


#endif /*DESINGPLAN_H_*/
