#include<QtGui/QMouseEvent>
#include<QtGui/QFileDialog>
#include<QtGui/QMessageBox>

#include "EvenementHandler.h"
#include <iostream>
#include <vector>
#include <stack>
#include "SimulationActivity.h"
#include "SimulationChangement.h"
#include "SimulationDiffusion.h"
#include "SimulationAttractorsAndBasinsOfAttraction.h"
#include "SimulationAttractorsAndBasinsOfAttraction2.h"
#include "Computer.h"
#include <algorithm>
#include <stdlib.h>
//#include <QtGui/QAction>

using namespace std;

EvenementHandler::EvenementHandler(Ui::MainWindow *parent, Network *network, UpdateSchedulingPlan * updateSchedulingPlan)
{
	this->parent = parent;
	this->network = network;
	this->updateSchedulingPlan = updateSchedulingPlan;
	scale = 1.0f;
	transX = transY = 0;
	currentNeuron = NULL;
	synapseBaseNeuron = NULL;
	mouseX = mouseY = 0;
	midX = midY = -1;
	fileName = "";
	fileUpdated();
	aSimpleCopy = new Network();
	networkLayersMenu = parent->menuEdit->addMenu("Select layer");
	connect(networkLayersMenu, SIGNAL(aboutToShow()), this, SLOT(networkLayersMenu_aboutToShow()));

	//this->networkDesigner = networkDesigner;
	//QAction * hahi = new QAction("hahi", parent->menuSelect_layer);
	//networkLayersMenu->addAction(hahi);

}

EvenementHandler::~EvenementHandler()
{
}

/*
 * Slot for updateMe
 */
void EvenementHandler::updateMeSlot(){
	updateMe();
}

/*
 * I m an Updating Methods so...
 * Call me every time you want!!!!
 */
void EvenementHandler::updateMe(){
		//std::cout << "hedi " << fileName.toStdString() << " hah" << endl;
		//std::cout << parent->frmDesign->parentWidget()->windowTitle().toStdString() << endl;
	if (saved) {
		fileUpdated();
	}
	else{
		fileWasModified();
	}
	emit temperatureChanged(network->getTemperature());
	emit neuronTemperatureChanged(network->getTemperature());
	parent->frmDesign->parentWidget()->repaint();
	if(network->getUniformalTemperature())	parent->chkUniformalTemperature->setCheckState(Qt::Checked);
	else					parent->chkUniformalTemperature->setCheckState(Qt::Unchecked);
	// Refreshing the cmbUpdateBlock
	while(parent->cmbUpdateBlock->count()>0) parent->cmbUpdateBlock->removeItem(0);

	parent->cmbUpdateBlock->addItem("Add a new block...");
	char blockName[20];

	for(int i=0; i < updateSchedulingPlan->getNb_blocks(); i++) {
		sprintf(blockName, "Block %d",i);
		parent->cmbUpdateBlock->addItem(blockName);
	}
}

/*
 * When the item open in the menu file is selected
 */
void EvenementHandler::actionO_pen_Clicked(bool checked){
	if(!saved){
		int ret = QMessageBox::warning(this, tr("Network Designer"),
                   tr("The document has been modified.\n"
                      "Do you want to save your changes?"),
                   QMessageBox::Save | QMessageBox::Discard
                   | QMessageBox::Cancel,
                   QMessageBox::Save);
        if(ret==QMessageBox::Cancel) return;
        if(ret==QMessageBox::Save) action_Save_Clicked(checked);

	}
		fileName = QFileDialog::getOpenFileName(this,
	     tr("Open Network Designer File"), "./", tr("Network Designer File (*.nml)"));
	     if(fileName.size()!=0){
	     	parent->frmDesign->load(fileName);
	     	network = parent->frmDesign->getNetwork();
	     	updateSchedulingPlan = parent->frmDesign->getUpdateSchedulingPlan();
	     	parent->frmDesign->repaint();
	     }
	     updateMe();
	     fileUpdated();
}

/*
 * When the item save as in the menu file is selected
 */
void EvenementHandler::actionSave_As_Clicked(bool checked){
		fileName = QFileDialog::getSaveFileName(this,
			     tr("Save Network Designer File"), "./", tr("Network Designer File (*.nml)"));
	     if(fileName.size()!=0) {
	     	parent->frmDesign->save(fileName);
	     	parent->frmDesign->repaint();
	    	updateMe();
	     	fileUpdated();
	     }
	     else {
	    	updateMe();
	    	fileWasModified();
	     }
}

/*
 * When the item save in the menu file is selected
 */
void EvenementHandler::action_Save_Clicked(bool checked){
	if(fileName.size()==0) actionSave_As_Clicked(checked);
	else{
		parent->frmDesign->save(fileName);
		parent->frmDesign->repaint();
		updateMe();
		fileUpdated();
	}
}

/*
 * When the item save as in the menu file is selected
 */
void EvenementHandler::action_Export_Premodel(bool checked){

		QString exportFileName = QFileDialog::getSaveFileName(this,
			     tr("Save GNBox Premodel"), "./", tr("GNBox Premodel File (*.gpf)"));
	     if(exportFileName.size()!=0) {
	     	parent->frmDesign->exportToGNBoxPremodel(exportFileName);
	     	//premodelfileUpdated();
	     	parent->frmDesign->repaint();
	     }
	     //else premodelfileWasModified();
	updateMe();
}


/*
 * When the item new in the menu file is selected
 */
void EvenementHandler::action_New_Clicked(bool checked){
	     if(!saved){
				int ret = QMessageBox::warning(this, tr("Network Designer"),
		                   tr("The document has been modified.\n"
		                      "Do you want to save your changes?"),
		                   QMessageBox::Save | QMessageBox::Discard
		                   | QMessageBox::Cancel,
		                   QMessageBox::Save);
		        if(ret==QMessageBox::Cancel) return;
		        if(ret==QMessageBox::Save) action_Save_Clicked(checked);
	     }
	     parent->frmDesign->setNetwork(new Network(1));
	     parent->frmDesign->setUpdateSchedulingPlan(new UpdateSchedulingPlan());
	     fileName = "";
	     fileWasModified();
	     network = parent->frmDesign->getNetwork();
	     updateSchedulingPlan = parent->frmDesign->getUpdateSchedulingPlan();
	     parent->chkUniformalTemperature->setCheckState(Qt::Checked);
	     parent->frmDesign->repaint();

	     updateMe();
}

/*
 * When the item save as in the menu file is selected
 */
void EvenementHandler::action_Quit(bool checked){
    if(!saved){
			int ret = QMessageBox::warning(this, tr("Network Designer"),
	                   tr("The document has been modified.\n"
	                      "Do you want to save your changes?"),
	                   QMessageBox::Save | QMessageBox::Discard
	                   | QMessageBox::Cancel,
	                   QMessageBox::Save);
	        if(ret==QMessageBox::Cancel) return;
	        if(ret==QMessageBox::Save) {
	        	if(fileName.size()==0) actionSave_As_Clicked(checked);
	        	else action_Save_Clicked(checked);
	        }
    }
	emit quitApplication();
}


/*
 * dsbTemeprature Changed evenementHandler
 */
void EvenementHandler::dsbTemperature_Changed(double temperature){
	network->setTemperature(temperature);
	parent->chkUniformalTemperature->setCheckState(Qt::Checked);
	parent->frmDesign->setDefaultTemperature(temperature);
	fileWasModified();
}

/*
 * Modify all the temperature of the selected neurons
 */
void EvenementHandler::dsbNeuronTemperature_Changed(double temperature){
	if(parent->dsbNeuronTemperature->hasFocus()){
		parent->frmDesign->setDefaultTemperature(temperature);
		for(int i=0; i<network->getNbNeurons();i++){
			if(network->getNeuron(i)->getSelected()){
				network->getNeuron(i)->setTemperature(temperature);
			}
		}
		fileWasModified();
	}
}


/*
 * Modify the Threshold of all the selected neurons.
 */
void EvenementHandler::dsbThreshold_Changed(double threshold){
	if(parent->dsbThreshold->hasFocus()){
		parent->frmDesign->setDefaultThreshold(parent->cmbState->currentIndex(), threshold);
		for(int i=0; i<network->getNbNeurons();i++){
			if(network->getNeuron(i)->getSelected()){
				if(parent->cmbState->currentIndex() < network->getNeuron(i)->getNbStates()){
					network->getNeuron(i)->setTemperature(parent->dsbNeuronTemperature->value());
					network->getNeuron(i)->setState(parent->cmbState->currentIndex());
					network->getNeuron(i)->setThreshold(parent->cmbState->currentIndex(), threshold);
				}
			}
		}
		fileWasModified();
	}
}

/*
 * Modify the weight of all the selected neurons.
 */
void EvenementHandler::dsbWeight_Changed(double weight){
	if(parent->dsbWeight->hasFocus()){
		parent->frmDesign->setDefaultWeight(weight);
		for(int i=0; i<network->getNbNeurons();i++){
			for(int j=0; j<network->getNeuron(i)->getNb_neighbors(); j++){
				if(network->getNeuron(i)->getSynapse(j)->getSelected()){
					network->getNeuron(i)->getSynapse(j)->setWeight(weight);
					network->getNeuron(i)->getSynapse(j)->setDelay(parent->sbDelay->value());
				}
			}
		}
	}
	fileWasModified();
}


/*
 * Executed when the start button is pushed
 */
void EvenementHandler::pbStart_click(bool checked){
	Computer * computer = new Computer(network, updateSchedulingPlan, parent);

	if(parent->cmbSimulationType->currentText() == "No simulation"){
		if(parent->cmbScheduleType->currentText() == "Parallel"){
			computer->computeP();
		}
		else if(parent->cmbScheduleType->currentText() == "Block Parallel"){
			computer->computeBP();
		}

		else if(parent->cmbScheduleType->currentText() == "Block Sequential"){
			computer->computeBS();
		}
		else if(parent->cmbScheduleType->currentText() == "Sequential"){
			computer->computeS();
		}
		//parent->frmDesign->repaint();
	}
	else if(parent->cmbSimulationType->currentText() == "Activity analysis"){
		SimulationActivity * simulationActivity = new SimulationActivity(computer);
		UpdateBlock * myView = new UpdateBlock();
		for(int i=0; i < network->getNbNeurons();i++){
			myView->addNeuronIndex(network->getNeuron(i)->getIndex());
		}
		simulationActivity->setView(myView);
		if(parent->cmbScheduleType->currentText() == "Parallel"){
			simulationActivity->setUpdateType(P);
			simulationActivity->run();
		}

		else if(parent->cmbScheduleType->currentText() == "Block Parallel"){
			simulationActivity->setUpdateType(BP);
			simulationActivity->run();
		}

		else if(parent->cmbScheduleType->currentText() == "Block Sequential"){
			simulationActivity->setUpdateType(BS);
			simulationActivity->run();
		}
		else if(parent->cmbScheduleType->currentText() == "Sequential"){
			simulationActivity->setUpdateType(S);
			simulationActivity->run();
		}
		delete simulationActivity;
		delete myView;
	}

	else if(parent->cmbSimulationType->currentText() == "Changement analysis"){
		SimulationChangement * simulationChangement = new SimulationChangement(computer);
		UpdateBlock * myView = new UpdateBlock();
		for(int i=0; i < network->getNbNeurons();i++){
			myView->addNeuronIndex(network->getNeuron(i)->getIndex());
		}
		simulationChangement->setView(myView);
		if(parent->cmbScheduleType->currentText() == "Parallel"){
			simulationChangement->setUpdateType(P);
			simulationChangement->run();
		}

		else if(parent->cmbScheduleType->currentText() == "Block Parallel"){
			simulationChangement->setUpdateType(BP);
			simulationChangement->run();
		}

		else if(parent->cmbScheduleType->currentText() == "Block Sequential"){
			simulationChangement->setUpdateType(BS);
			simulationChangement->run();
		}
		else if(parent->cmbScheduleType->currentText() == "Sequential"){
			simulationChangement->setUpdateType(S);
			simulationChangement->run();
		}
		delete simulationChangement;
		delete myView;
	}

	else if(parent->cmbSimulationType->currentText() == "Attractor and basins"){
		SimulationAttractorsAndBasinsOfAttraction * simulationAttractorsAndBasinsOfAttraction = new SimulationAttractorsAndBasinsOfAttraction(computer);
		UpdateBlock * myView = new UpdateBlock();
		for(int i=0; i < network->getNbNeurons();i++){
			myView->addNeuronIndex(network->getNeuron(i)->getIndex());
		}
		simulationAttractorsAndBasinsOfAttraction->setView(myView);
		if(parent->cmbScheduleType->currentText() == "Parallel"){
			simulationAttractorsAndBasinsOfAttraction->setUpdateType(P);
			simulationAttractorsAndBasinsOfAttraction->run();
		}
		else if(parent->cmbScheduleType->currentText() == "Block Parallel"){
			simulationAttractorsAndBasinsOfAttraction->setUpdateType(BP);
			simulationAttractorsAndBasinsOfAttraction->run();
		}

		else if(parent->cmbScheduleType->currentText() == "Block Sequential"){
			simulationAttractorsAndBasinsOfAttraction->setUpdateType(BS);
			simulationAttractorsAndBasinsOfAttraction->run();
		}
		else if(parent->cmbScheduleType->currentText() == "Sequential"){
			simulationAttractorsAndBasinsOfAttraction->setUpdateType(S);
			simulationAttractorsAndBasinsOfAttraction->run();
		}
		delete simulationAttractorsAndBasinsOfAttraction;
		delete myView;
	}
	else if(parent->cmbSimulationType->currentText() == "Attractor and basins V2"){
			SimulationAttractorsAndBasinsOfAttraction2 * simulationAttractorsAndBasinsOfAttraction2 = new SimulationAttractorsAndBasinsOfAttraction2(computer);
			UpdateBlock * myView = new UpdateBlock();
			for(int i=0; i < network->getNbNeurons();i++){
				myView->addNeuronIndex(network->getNeuron(i)->getIndex());
			}
			simulationAttractorsAndBasinsOfAttraction2->setView(myView);
			if(parent->cmbScheduleType->currentText() == "Parallel"){
				simulationAttractorsAndBasinsOfAttraction2->setUpdateType(P);
				simulationAttractorsAndBasinsOfAttraction2->run();
			}
			else if(parent->cmbScheduleType->currentText() == "Block Parallel"){
				simulationAttractorsAndBasinsOfAttraction2->setUpdateType(BP);
				simulationAttractorsAndBasinsOfAttraction2->run();
			}

			else if(parent->cmbScheduleType->currentText() == "Block Sequential"){
				simulationAttractorsAndBasinsOfAttraction2->setUpdateType(BS);
				simulationAttractorsAndBasinsOfAttraction2->run();
			}
			else if(parent->cmbScheduleType->currentText() == "Sequential"){
				simulationAttractorsAndBasinsOfAttraction2->setUpdateType(S);
				simulationAttractorsAndBasinsOfAttraction2->run();
			}
			delete simulationAttractorsAndBasinsOfAttraction2;
			delete myView;
	}
	else if(parent->cmbSimulationType->currentText() == "Diffusion analysis"){
		SimulationDiffusion * simulationDiffusion = new SimulationDiffusion(computer);
		UpdateBlock * myView = new UpdateBlock();
		for(int i=0; i < network->getNbNeurons();i++){
			myView->addNeuronIndex(network->getNeuron(i)->getIndex());
		}
		simulationDiffusion->setView(myView);
		simulationDiffusion->setUpdateType(P);
		simulationDiffusion->run();
		delete simulationDiffusion;
		delete myView;
	}

	delete computer;
}

/*
 * Stop the simulation and referesh the synapses
 */
void EvenementHandler::pbStop_click(bool checked){
	for(int i=0; i < network->getNbNeurons(); i++){
		network->getNeuron(i)->refreshSynapses();
	}
}

/*
 * The value of the state radio button and the threshold are those of the latest selected neuron
 * And set the default state and threshold's value for futur neuron.
 */
void EvenementHandler::neuronSelected(Neuron* neuron){

	parent->frmDesign->setDefaultTemperature(neuron->getTemperature());
	parent->frmDesign->setDefaultState(neuron->getState());
	parent->frmDesign->setDefaultThresholds(neuron);
	cmbState_Updating(neuron->getNbStates());

	emit nbStatesChanged(neuron->getNbStates());
	emit stateChanged(neuron->getState());

	// April 12th 2010
	QString tmpQStr1(neuron->getNodeID());
	emit nodeIDChanged(tmpQStr1);
	char tmpStr[10];
	sprintf(tmpStr,"%d", neuron->getIndex());
	QString tmpQStr2(tmpStr);
	emit nodeIndexChanged(tmpQStr2);
	//


	if(neuron->getState() == 0){
		parent->dsbThreshold->setEnabled(false);
	}
	else{
		parent->dsbThreshold->setEnabled(true);
		emit thresholdChanged(neuron->getThreshold(neuron->getState()));
	}
	emit neuronTemperatureChanged(neuron->getTemperature());

}

/*
 * The value of the sbWeight is the value of the latest selected synapse
 * And set the default weight and delay values for the futur synapse.
 */
void EvenementHandler::synapseSelected(Synapse* synapse){

	emit weightChanged(synapse->getWeight());
	emit delayChanged(synapse->getDelay());
	parent->frmDesign->setDefaultWeight(synapse->getWeight());
	parent->frmDesign->setDefaultDelay(synapse->getDelay());
}

/*
 * Handle the cmbUpdateBlock index changed's signal
 */
void EvenementHandler::cmbUpdateBlock_Changed(int index){
	 int blockNum;
	 char blockName[20] = "Block 0";
	 UpdateBlock * currentUpdateBlock = NULL;

	 if(index == 0){
		blockNum = parent->cmbUpdateBlock->count();
		sprintf(blockName, "Block %d",blockNum);
	 	parent->cmbUpdateBlock->addItem(blockName);
	 	currentUpdateBlock = new UpdateBlock();
	 	updateSchedulingPlan->addUpdateBlock(currentUpdateBlock);
	 	parent->frmDesign->setCurrentUpdateBlock(blockNum - 1);
	 	emit currentBlockChanged(blockNum);
	 	emit currentUpdateMethodsChanged(COMPUTE);
	 }
	 else{
	 	parent->frmDesign->setCurrentUpdateBlock(index - 1);
	 	emit currentUpdateMethodsChanged(updateSchedulingPlan->getUpdateBlock(index-1)->getUpdateMethods());
	 	//emit currentBlockChanged(index);
	 }
	fileWasModified();
}

/*
 * Handle the cmbUpdateMethods index changed's signal
 */
void EvenementHandler::cmbUpdateMethods_Changed(int index){
	if(parent->cmbUpdateBlock->currentIndex()>0)
		updateSchedulingPlan->getUpdateBlock(parent->cmbUpdateBlock->currentIndex() - 1)->setUpdateMethods(index);
	fileWasModified();
}

/*
 * FileName's setter
 */
void EvenementHandler::setFileName(QString fileName){
	this->fileName = fileName;
}

/*
 * Filename's getter
 */
QString EvenementHandler::getFileName() const{
	return fileName;
}

/*
 * Switch between the network and the schedule design modes.
 */
void EvenementHandler::toolBox_currentChanged(int index){
	if(parent->toolBox->itemText(index) == "Update schedule"){
		if(parent->cmbUpdateBlock->currentIndex() > 0){
			parent->frmDesign->setCurrentUpdateBlock(parent->cmbUpdateBlock->currentIndex()-1);
		}
	}
	else{
		for(int i = 0; i < network->getNbNeurons(); i++) network->getNeuron(i)->setYellowMe(false);
	}
	parent->frmDesign->repaint();
}

/*
 * Check box state Changed
 */
void EvenementHandler::chkUniformalTemperature_Changed(int state){
	if(state==Qt::Checked)			network->setUniformalTemperature(true);
	else if(state==Qt::Unchecked)		network->setUniformalTemperature(false);
	fileWasModified();
}

/*
 * When performing a change on the delay the focus must be on the sbDelay widget
 */
void EvenementHandler::sbDelay_Changed(int delay){
	if(parent->sbDelay->hasFocus()){
		parent->frmDesign->setDefaultDelay(delay);
		for(int i=0; i<network->getNbNeurons();i++){
			for(int j=0; j<network->getNeuron(i)->getNb_neighbors(); j++){
				if(network->getNeuron(i)->getSynapse(j)->getSelected()){
					network->getNeuron(i)->getSynapse(j)->setDelay(delay);
					network->getNeuron(i)->getSynapse(j)->setWeight(parent->dsbWeight->value());
				}
			}
		}
		fileWasModified();
	}
	parent->frmDesign->repaint();
}

void EvenementHandler::actionCut_Clicked(bool checked){
	actionCopy_Clicked(checked);
	actionDelete_Clicked(checked);
	fileWasModified();
}

void EvenementHandler::actionCopy_Clicked(bool checked){
	delete aSimpleCopy;
	bool okki;
	int v=0, w=0;
	aSimpleCopy = new Network();
	for(int i=0; i<network->getNbNeurons(); i++){
		if(network->getNeuron(i)->getSelected()){
			aSimpleCopy->addNeuron(new Neuron(*(network->getNeuron(i))));
		}
	}

	for(int i=0; i<network->getNbNeurons(); i++){
		if(network->getNeuron(i)->getSelected()){
			for(int j=0; j< network->getNeuron(i)->getNb_neighbors();j++){
				if(network->getNeuron(i)->getSynapse(j)->getFinalNeuron()->getSelected()){
					// Seek the index of the neighbor on the aSimpleCopy network
					w = 0;
					for(int l=0; l<network->getNeuron(i)->getSynapse(j)->getFinalNeuron()->getIndex();l++){
						if(network->getNeuron(l)->getSelected()) w++;
					}
					okki = aSimpleCopy->getNeuron(v)->addSynapse(aSimpleCopy->getNeuron(w), network->getNeuron(i)->getSynapse(j)->getWeight(), network->getNeuron(i)->getSynapse(j)->getDelay());
					aSimpleCopy->getNeuron(v)->getSynapse(aSimpleCopy->getNeuron(v)->getNb_neighbors()-1)->setCX(network->getNeuron(i)->getSynapse(j)->getCX());
					aSimpleCopy->getNeuron(v)->getSynapse(aSimpleCopy->getNeuron(v)->getNb_neighbors()-1)->setCY(network->getNeuron(i)->getSynapse(j)->getCY());
				}
			}
			v++;
		}
	}
}

void EvenementHandler::actionPaste_Clicked(bool checked){
	if(aSimpleCopy != NULL){
		if(aSimpleCopy->getNbNeurons()!=0){
			network->addNetwork(aSimpleCopy);
		}
		fileWasModified();
	}
}

void EvenementHandler::actionDelete_Clicked(bool checked){
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
	fileWasModified();
	parent->frmDesign->repaint();

}

void EvenementHandler::actionSelect_all_Clicked(bool checked){
	for(int i=0; i < network->getNbNeurons(); i++){
		network->getNeuron(i)->setSelected(true);
	}
	parent->frmDesign->repaint();
}

void EvenementHandler::networkLayersMenu_aboutToShow(){
	// A deep seeking algorithm'll do the job
	stack<Neuron*>  destination;
	stack<int> distance;
	vector<int> minimals;
	vector<int> l1_distances;
	Neuron* tmpNeuron;
	int tmpDistance;
	//int center_l1_distance = network->getNbNeurons();
	minimals.reserve(network->getNbNeurons());
	l1_distances.reserve(network->getNbNeurons());

	network->deselectAll();

	if(layerActions.size() != 0){
		for(int i=0; i<(int)layerActions.size(); i++){
			layerActions[i]->disconnect();
			delete layerActions[i];
		}
	}
	layers.clear();
	layerActions.clear();
	networkLayersMenu->clear();

	for(int i=0; i<network->getNbNeurons(); i++){
		l1_distances[i]= -1;
	}

	for(int i=0; i< network->getNbNeurons(); i++){
		// Initialization
			for(int p=0; p < network->getNbNeurons(); p++){
				for(int j=0; j < network->getNeuron(p)->getNb_neighbors(); j++){
					/*if(i==p){
						if(network->getNeuron(i)->getSynapse(j)->getFinalNeuron()->getIndex() != i){
							destination.push(network->getNeuron(i)->getSynapse(j)->getFinalNeuron());
							distance.push(1);
						}
					}*/
					//else{
					if(i!=p){
						if(network->getNeuron(p)->getSynapse(j)->getFinalNeuron()->getIndex() == i){
							destination.push(network->getNeuron(p));
							distance.push(1);
						}
					}
				}
			}

			for(int j=0; j < network->getNbNeurons(); j++) 	minimals[j] = network->getNbNeurons();

			// l1-distance calculus
			while(distance.size()>0){
				if(destination.top()->getIndex() != i){
					if(distance.top()<network->getNbNeurons()){
						if(distance.top() < minimals[destination.top()->getIndex()]){
							minimals[destination.top()->getIndex()] = distance.top();
						}
						tmpNeuron = destination.top();
						tmpDistance = distance.top();
						destination.pop();
						distance.pop();
						for(int p=0; p < network->getNbNeurons(); p++){
							for(int j=0; j < network->getNeuron(p)->getNb_neighbors(); j++){
								/*if(p==tmpNeuron->getIndex()){
									if(network->getNeuron(p)->getSynapse(j)->getFinalNeuron()->getIndex() != tmpNeuron->getIndex()){
										if(tmpDistance+1 < minimals[tmpNeuron->getSynapse(j)->getFinalNeuron()->getIndex()]){
											destination.push(tmpNeuron->getSynapse(j)->getFinalNeuron());
											distance.push(tmpDistance + 1);
										}
									}
								}*/
								//else{
								if(p!=tmpNeuron->getIndex()){
									if(network->getNeuron(p)->getSynapse(j)->getFinalNeuron()->getIndex() == tmpNeuron->getIndex()){
										if(tmpDistance+1 < minimals[p]){
											destination.push(network->getNeuron(p));
											distance.push(tmpDistance + 1);
										}
									}
								}
							}
						}
					}
					else{
						destination.pop();
						distance.pop();
					}
				}
				else{
					// Added on 12 may
					/*if(distance.top()<network->getNbNeurons()){
						if(distance.top() < minimals[destination.top()->getIndex()]){
							minimals[destination.top()->getIndex()] = distance.top();
						}
					}*/
					///////////////////////////
					destination.pop();
					distance.pop();
				}
			}
			for(int j = 0; j< network->getNbNeurons(); j++){
				if((minimals[j]>l1_distances[i]) and (minimals[j]!=network->getNbNeurons()))l1_distances[i] = minimals[j];
			}
	}

	BlockSelector centersBlockSelector(network,l1_distances[0],0);
	layers.push_back(centersBlockSelector);

	for(int i=1; i < network->getNbNeurons(); i++){
		int j=0;
		while(j<(int)layers.size()){
			if(layers[j].getL1_distance() == l1_distances[i]){
				layers[j].addNeuronIndex(i);
				break;
			}
			j++;
		}
		if(j == (int)layers.size()){
			BlockSelector newLayerBlockSelector(network,l1_distances[i], i);
			layers.push_back(newLayerBlockSelector);
		}
	}

	sort(layers.begin(), layers.end());

	// Connecting Slots

	char centerText[30];
	sprintf(centerText, "Centers -- Excentricity = %d", layers[0].getL1_distance());

	layerActions.push_back(new QAction(centerText, networkLayersMenu));
	connect(layerActions[0], SIGNAL(triggered(bool)), &layers[0], SLOT(select(bool)));
	connect(&layers[0], SIGNAL(repaintPlease()), this, SLOT(repaintPlease()));
	for(int i=1; i < (int)layers.size() - 1;i++){
		char layerText[30];
		sprintf(layerText, "Layer %d -- Excentricity = %d", i, layers[i].getL1_distance());
		layerActions.push_back(new QAction(layerText, networkLayersMenu));
		connect(layerActions[i], SIGNAL(triggered(bool)), &layers[i], SLOT(select(bool)));
		connect(&layers[i], SIGNAL(repaintPlease()), this, SLOT(repaintPlease()));
	}

	char borderText[30];
	sprintf(borderText, "Borders -- Excentricity = %d", layers[(int)layers.size()-1].getL1_distance());
	layerActions.push_back(new QAction(borderText, networkLayersMenu));
	connect(layerActions[layerActions.size() - 1], SIGNAL(triggered(bool)), &layers[layers.size() - 1], SLOT(select(bool)));
	connect(&layers[layers.size() - 1], SIGNAL(repaintPlease()), this, SLOT(repaintPlease()));

	for(int i=0; i<(int)layerActions.size(); i++) networkLayersMenu->addAction(layerActions[i]);
}

/*
 * A slot for repainting
 */
void EvenementHandler::repaintPlease(){
	parent->frmDesign->repaint();
}

/*
 * cmbState_value changed 's slot
 */
void EvenementHandler::cmbState_Changed(int index){
	if(parent->cmbState->hasFocus()){
		parent->frmDesign->setDefaultState(index);
		for(int i=0; i<network->getNbNeurons();i++){
			if(network->getNeuron(i)->getSelected()){
				if(index < network->getNeuron(i)->getNbStates()){
					network->getNeuron(i)->setState(index);
					if(index == 0){
						parent->dsbThreshold->setEnabled(false);
					}
					else{
						parent->dsbThreshold->setEnabled(true);
						emit thresholdChanged(network->getNeuron(i)->getThreshold(index));
					}
				}
			}
			fileWasModified();
		}
		parent->frmDesign->repaint();
	}
}

/*
 *  sbNbStates Evenement Handler
 */
void EvenementHandler::sbNbStates_Changed(int nbStates){
	//printf("nbstates = %d\n", nbStates);
	if(parent->sbNbStates->hasFocus()){
		parent->frmDesign->setDefaultNbStates(nbStates);
		cmbState_Updating(nbStates);
		for(int i=0; i<network->getNbNeurons();i++){
			if(network->getNeuron(i)->getSelected()){
				network->getNeuron(i)->setNbStates(nbStates);
				if(network->getNeuron(i)->getState() > nbStates - 1) network->getNeuron(i)->setState(nbStates - 1);
				emit stateChanged(network->getNeuron(i)->getState());
				if(network->getNeuron(i)->getState() == 0){
					parent->dsbThreshold->setEnabled(false);
				}
				else{
					parent->dsbThreshold->setEnabled(true);
					emit thresholdChanged(network->getNeuron(i)->getState());
				}

			}
			fileWasModified();
		}
		parent->frmDesign->repaint();
	}
}

/*
 * cmbStates updating
*/

void EvenementHandler::cmbState_Updating(int nbStates){
	char layerText[30];
	QStringList qslStates;
	string stateStr;
	if(nbStates > 0){
		for(int i=0; i<nbStates; i++){
			sprintf(layerText, "State %d", i);
			qslStates << layerText;
		}
		while(parent->cmbState->count() != 0) parent->cmbState->removeItem(0);
		parent->cmbState->insertItems((int)-1, qslStates);
	}
	fileWasModified();
}

/*
* This procedure is called every time the network is modified
*/
void EvenementHandler::fileWasModified(){
	saved = false;
	QString justFileName = fileName.section("/",-1,-1);

	if(fileName.size()==0)
		emit updateTitle(tr("%1 - %2 *").arg(tr("NetworkDesigner")).arg(tr("Untitled.nml")));
	else
		emit updateTitle(tr("%1 - %2 *").arg(tr("NetworkDesigner")).arg(justFileName));
}

/*
 * This procedure is called every time the network is updated
 */
void EvenementHandler::fileUpdated(){
	saved = true;
	QString justFileName = fileName.section("/",-1,-1);

	if(fileName.size()==0)
		emit updateTitle(tr("%1 - %2").arg(tr("NetworkDesigner")).arg(tr("Untitled.nml")));
	else
		emit updateTitle(tr("%1 - %2").arg(tr("NetworkDesigner")).arg(justFileName));
}

void EvenementHandler::txtIndex_Changed(QString index){
// nothing to do
}

void EvenementHandler::txtNodeID_Changed(QString nodeID){
	if(parent->txtNodeID->hasFocus()){
		for(int i=0; i<network->getNbNeurons();i++){
			if(network->getNeuron(i)->getSelected()){
				network->getNeuron(i)->setNodeID((char*)(nodeID.toStdString().c_str()));
			}
		}
		parent->frmDesign->repaint();
	}
}
