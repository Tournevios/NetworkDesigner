#include "designplan.h"
#include <QtGui/QPainter>
#include "networkdesigner.h"
#include <iostream>
using namespace std;
/*
 * Initiat all the atributes
 */
DesignPlan::DesignPlan(QWidget* parent):QFrame(parent){
	scale = 1.0f;
	transX = transY = 0;
	currentNeuron = NULL;
	currentSynapse = NULL;
	synapseBaseNeuron = NULL;
	mouseX = mouseY = 0;
	midX = midY = -1;
	this->setMouseTracking(true);
	this->setFrameShape(DesignPlan::StyledPanel);
	ndp = new NetworkDesignerParser();

	defaultNbStates = 2;
	defaultThreshold.push_back(0.001);
	defaultState = 0;
	defaultWeight = 1;
	defaultTemperature = 1;
	defaultDelay = 0;
	loaded = false;
	currentUpdateBlock = -1;
	//load("test.nml");
	network = new Network(0);
	updateSchedulingPlan = new UpdateSchedulingPlan();
	network->deselectAll();
	loaded = true;

}

/*
 * A free area is an empty area such that the minimal distance between its points and all
 * the neurons of the network is larger than 2 times the radius of a neuron.
 * If left button pressed in a free area then create a neuron
 * else select a neuron: if ctrl is holded than add to selection else only this neuron is selected.
 *
 * If the center button is pressed begin calculating the translation parameters
 *
 * If the riight button is pressed begin calculating the representation ot
 * the under construction neurons's parameters
 *
 */
void DesignPlan::mousePressEvent(QMouseEvent * event){

	Neuron * n = NULL;
	Synapse * s = NULL;
	bool ctrl = event->modifiers() & Qt::ControlModifier;
	bool alt = event->modifiers() & Qt::AltModifier;
	bool shift = event->modifiers() & Qt::ShiftModifier;

	this->setFocus();
	if(event->button() & Qt::LeftButton){
		if (!(ctrl or alt or shift)){
			network->deselectAll();
			// Those 7 lines ensure that two neurons'll never be drawn on the same place
			// And select a neuron in case the cursor is on it.
			n = network->getNeuron((int)((event->x() - transX)/scale), (int)((event->y() - transY)/scale), (int)(10*2/scale)); // ensure that the maximum distance is 2 * radius
			if(n != NULL){
				n = network->getNeuron((int)((event->x() - transX)/scale), (int)((event->y() - transY)/scale), (int)(10/scale)); // Ensure that the maximum distance for selecting is radius
				if(n != NULL){
					n->setSelected(true);
					emit neuronSelectedChanged(n);
					s = n->getSelfSynapse();
					if(s!=NULL) emit synapseSelectedChanged(s);
					currentNeuron = n;
					currentSynapse = NULL;
				}
			}
			else{
				s = network->getSynapse(event->x(), event->y());
				if(s != NULL){
					s->setSelected(not s->getSelected());
					emit synapseSelectedChanged(s);
					currentSynapse = s;
				}
				else{
					//printf("actually %d %f\n", defaultState, defaultThreshold[defaultState]);
					n = new Neuron(0, defaultState, defaultNbStates, defaultThreshold);
					n->setTemperature(defaultTemperature);
					n->setSelected(true);
					emit neuronSelectedChanged(n);
					currentNeuron = n;
					currentSynapse = NULL;
					n->setXY((int)((event->x() - transX)/scale), (int)((event->y() - transY)/scale));
					network->addNeuron(n);
				}
			}
		}
		else if(ctrl and !(alt or shift)){
			Neuron * n;
			n = network->getNeuron((int)((event->x() - transX)/scale), (int)((event->y() - transY)/scale), (int)(10/scale));
			if(n != NULL){
				if(n->getSelected()){
					currentNeuron = NULL;
					n->setSelected(false);
				}
				else{
					n->setSelected(true);
					emit neuronSelectedChanged(n);
					currentNeuron = n;
				}
				currentSynapse = NULL;
			}
			else{
				Synapse * s = network->getSynapse(event->x(), event->y());
				if(s != NULL){
					s->setSelected(not s->getSelected());
					emit synapseSelectedChanged(s);
					currentSynapse = s;
				}
			}

		}
		else if(shift and !(alt or ctrl) and (currentUpdateBlock > -1)){
			Neuron * n;
			n = network->getNeuron((int)((event->x() - transX)/scale), (int)((event->y() - transY)/scale), (int)(10/scale));
			if(n != NULL){
				if(n->getYellowMe()){
					n->setYellowMe(false);
					updateSchedulingPlan->getUpdateBlock(currentUpdateBlock)->delNeuronIndex(n->getIndex());
				}
				else{
					n->setYellowMe(true);
					updateSchedulingPlan->getUpdateBlock(currentUpdateBlock)->addNeuronIndex(n->getIndex());
				}
			}
			emit blockModeInvoked(3); // Select the Update Scheduling panel
			emit networkIsModified();
		}
	}
	else if(event->button() & Qt::RightButton){
		network->deselectAll();
		currentNeuron = NULL;
		currentSynapse = NULL;
		synapseBaseNeuron = network->getNeuron((int)((event->x() - transX) / scale), (int)((event->y() - transY) / scale), (int)(10/scale));
		if(synapseBaseNeuron!=NULL){
			synapseBaseNeuron->setSelected(true);
			emit neuronSelectedChanged(synapseBaseNeuron);
		}
	}
	else if(event->button() & Qt::MidButton){
		midX = event->x();
		midY = event->y();
	}

	this->repaint();
}

/*
 * When mouse release the under construction synapse is validated if it reach an other neuron.
 */
void DesignPlan::mouseReleaseEvent(QMouseEvent * event){

	//bool ctrl = event->modifiers() & Qt::ControlModifier;
	//bool alt = event->modifiers() & Qt::AltModifier;
	//bool shift = event->modifiers() & Qt::ShiftModifier;

	if(event->button() & Qt::MidButton){
		midX = -1;
		midY = -1;
	}
	else if((event->button() & Qt::RightButton) and (synapseBaseNeuron != NULL)){
		Neuron * synapseFinalNeuron = network->getNeuron((int)((event->x() - transX)/scale), (int)((event->y()- transY) / scale), (int)(10/scale));
		if(synapseFinalNeuron != NULL){
			network->deselectAll();
			synapseBaseNeuron->setSelected(true);
			if(synapseFinalNeuron->addSynapse(synapseBaseNeuron, defaultWeight, defaultDelay)){
				synapseFinalNeuron->setSelected(true);
				emit neuronSelectedChanged(synapseFinalNeuron);
				emit networkIsModified();
			}
		}
	}
	synapseBaseNeuron = NULL;
	currentNeuron = NULL;
	currentSynapse = NULL;

	this->repaint();
}

/*
 * Upload the zoom parameters
 */
void DesignPlan::wheelEvent(QWheelEvent * event){
	if(event->delta()>0){
		if(scale + 0.05f <= 2) scale += 0.05f;
	}
	else{
		if(scale - 0.05f > 0) scale -= 0.05f;
	}
	this->repaint();
}

void DesignPlan::keyPressEvent( QKeyEvent * event ){

	// Delete behavior
	if(event->key() == Qt::Key_Delete){
		// Let's say we delete some selected neurons
		for(int i=0; i<network->getNbNeurons();i++){
			if(network->getNeuron(i)->getSelected()){
				for(int j=0;j<updateSchedulingPlan->getNb_blocks();j++){
					updateSchedulingPlan->getUpdateBlock(j)->delNeuronIndex(i);
				}
				network->delNeuron(i);
				// Never forget to decrement the index of the neuron which index is greater than i
				for(int j=0;j<updateSchedulingPlan->getNb_blocks();j++){
					updateSchedulingPlan->getUpdateBlock(j)->decrementGreaterThan(i);
				}
				i--;
			}
		}
		// Delete the update block if necessary
		for(int j=0;j<updateSchedulingPlan->getNb_blocks();j++){
			if(updateSchedulingPlan->getUpdateBlock(j)->getSize()==0){
				updateSchedulingPlan->delUpdateBlock(j);
				j--;
			}
		}
		// Erase all the selected synapses
		for(int i=0; i<network->getNbNeurons();i++){
			for(int j=0; j<network->getNeuron(i)->getNb_neighbors(); j++){
				if(network->getNeuron(i)->getSynapse(j)->getSelected())
					network->getNeuron(i)->delSynapseBySynapseIndex(j);
			}
		}
	}
	emit networkIsModified();
	emit updateCalled();
}

/*
 * upload the mouse coordinate and the parameters of the translation.
 */
void DesignPlan::mouseMoveEvent(QMouseEvent * event){
	mouseX = event->x();
	mouseY = event->y();
	double dX, dY;
	if(currentNeuron!=NULL){
		dX = currentNeuron->getX() - (event->x()- transX)/scale;
		dY = currentNeuron->getY() - (event->y()- transY)/scale;
		currentNeuron->setXY((event->x()- transX)/scale, (event->y() - transY)/scale);
		for(int i=0; i<network->getNbNeurons(); i++){
			if((network->getNeuron(i)->getIndex()!=currentNeuron->getIndex()) and (network->getNeuron(i)->getSelected()))
				network->getNeuron(i)->setXY(network->getNeuron(i)->getX()-dX ,network->getNeuron(i)->getY()-dY);
		}
		emit networkIsModified();
		this->repaint();
	} else if(currentSynapse != NULL){
		//dX = currentSynapse->getBaseNeuron()->getX() - (event->x()- transX)/scale;
		//currentSynapse->setGExcentricity(currentSynapse->getGExcentricity() - ((float)dX/50000));
		currentSynapse->setCX((event->x() - transX)/scale);
		currentSynapse->setCY((event->y() - transY)/scale);
		emit networkIsModified();
		this->repaint();
	}
	if(synapseBaseNeuron != NULL){
		this->repaint();
	}
	if((midX != -1)){
		transX += mouseX - midX;
		transY += mouseY - midY;
		midX = mouseX;
		midY = mouseY;
		this->repaint();
	}
}

/*
 * Paint the network and a representation of the synapse the user is currently building.
 */
void DesignPlan::paintEvent(QPaintEvent * event){
	QPainter painter(this);
	if(loaded){
		if(synapseBaseNeuron != NULL){
			painter.setPen(QPen(Qt::black, 2*scale, Qt::SolidLine, Qt::RoundCap));
			painter.drawLine((int)(synapseBaseNeuron->getX()*scale + transX),(int)(synapseBaseNeuron->getY() * scale + transY), mouseX, mouseY);
		}
		if (network != NULL)
			network->drawMe(&painter, scale, transX, transY);
	}

}

/*
 * Network getter
 */
Network * DesignPlan::getNetwork() const{
	return network;
}

/*
 * Network setter
 */
void DesignPlan::setNetwork(Network* network){
	this->network = network;
}

/*
 * UpdateSchedulingPlan getter
 */
UpdateSchedulingPlan * DesignPlan::getUpdateSchedulingPlan() const{
	return updateSchedulingPlan;
}

/*
 * UpdateSchedulingPlan setter
 */
void DesignPlan::setUpdateSchedulingPlan(UpdateSchedulingPlan* updateSchedulingPlan){
	this->updateSchedulingPlan = updateSchedulingPlan;
}


/*
 * Delete all the neuron when network is deleted
 */

DesignPlan::~DesignPlan(){

	for(int i=0; i<network->getNbNeurons();i++){
		network->delNeuron(i);
	}

}

/*
 * Call the saving methods of the parser ndp to save the network under nml format
 * Network Designer File
 */
void DesignPlan::save(QString path){
	ndp->setNetwork(network);
	ndp->setUpdateSchedulingPlan(updateSchedulingPlan);
	ndp->save(path);
}

/*
 * Call the load methods of ndp to load a network from an nml file
 */
void DesignPlan::load(QString path){
	loaded = false;
	ndp->load(path);
	network = ndp->getNetwork();
	updateSchedulingPlan = ndp->getUpdateSchedulingPlan();
	loaded = true;
}


/*
 *
 */
void DesignPlan::exportToGNBoxPremodel(QString path){
	ndp->setNetwork(network);
	ndp->setUpdateSchedulingPlan(updateSchedulingPlan);
	ndp->exportToGNBoxPremodel(path);
}


/*
 * Default State's setter
 */
void DesignPlan::setDefaultState(int state){
	defaultState = state;
}

/*
 * Default Thresholds's setter
 */
void DesignPlan::setDefaultThresholds(const Neuron* neuron){
	defaultThreshold.clear();
	defaultNbStates = neuron->getNbStates();
	for(int i=1; i<neuron->getNbStates(); i++){
		defaultThreshold.push_back(neuron->getThreshold(i));
	}
}

/*
 * Default threshold 's setter
 */

void DesignPlan::setDefaultThreshold(int stateIndex, double threshold){
	if((stateIndex <= (int)defaultThreshold.size()) and (stateIndex > 0)){
		defaultThreshold[stateIndex - 1] = threshold;
	}
}


/*
 * Default NbStates's setter
 */

 void DesignPlan::setDefaultNbStates(int nbStates){
	defaultNbStates = nbStates;
	printf("ABOUT TO DESTROY YOUR LIFE!!\n");
	if((int)defaultThreshold.size() <= nbStates - 1){
		double defThreshold = 0.00;
		if((int)defaultThreshold.size() > 0) defThreshold = defaultThreshold[defaultThreshold.size() - 1];
		while((int) defaultThreshold.size() != nbStates - 1)	defaultThreshold.push_back(defThreshold);
	}
	else {
		while((int)defaultThreshold.size() != nbStates - 1)	defaultThreshold.pop_back();
	}
 }

/*
 * Default Weight's setter
 */
void DesignPlan::setDefaultWeight(double weight){
	defaultWeight = weight;
}

/*
 * Default Temperature's setter
 */
void DesignPlan::setDefaultTemperature(double temperature){
	defaultTemperature = temperature;
}

/*
 * DefautlDelay's setter
 */
void DesignPlan::setDefaultDelay(int delay){
	defaultDelay = delay;
}

/*
 *	CurrentUpdateBlock's setter
 */
void DesignPlan::setCurrentUpdateBlock(int currentUpdateBlock){
	this->currentUpdateBlock = currentUpdateBlock;

	for(int i=0; i<network->getNbNeurons(); i++){
		network->getNeuron(i)->setYellowMe(false);
	}
	if(currentUpdateBlock > -1){
		UpdateBlock * updateBlock = updateSchedulingPlan->getUpdateBlock(currentUpdateBlock);
		for(int i=0; i<updateBlock->getSize(); i++){
			network->getNeuron(updateBlock->getNeuronIndex(i))->setYellowMe(true);
//			printf("Yellowed %d with neuron: %d\n", currentUpdateBlock, updateBlock->getNeuronIndex(i));
		}
	}
	this->repaint();
}
