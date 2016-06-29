#include "networkdesigner.h"
#include "NetworkDesignerParser.h"
#include <QtGui/QMainWindow>

NetworkDesigner::NetworkDesigner(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	//network = new Network(1);

	//ui.frmDesign->setNetwork(network);

	QStringList qslScheduleTypes;

    qslScheduleTypes << "Parallel" << "Block Parallel" << "Block Sequential" << "Sequential";
	ui.cmbScheduleType->insertItems((int)-1, qslScheduleTypes);
	ui.cmbUpdateBlock->insertItem((int)-1, "Add a new block...");

	QStringList qslUpdateMethods;
	qslUpdateMethods << "Compute" << "Fixe" << "All to 1"
		<< "All to 0" << "Alternate 01" << "Alternate 10" << "Randomly";
	ui.cmbUpdateMethods->insertItems((int)-1, qslUpdateMethods);

	QStringList qslSimulations;
	qslSimulations << "No simulation" << "Activity analysis" << "Changement analysis" << "Diffusion analysis" << "Attractor and basins" << "Attractor and basins V2";
	ui.cmbSimulationType->insertItems((int)-1, qslSimulations);

    /*
    ui.cmbScheduleType->insertItem((int)-1, "Block Parallel");
    ui.cmbScheduleType->insertItem((int)-1, "Block Sequential");
   	ui.cmbScheduleType->insertItem((int)-1, "Parallel");
    ui.cmbScheduleType->insertItem((int)-1, "Sequential");
    */
    setWindowTitle("Network Designer");

//	ui.frmDesign->load("test.nml");

	network = ui.frmDesign->getNetwork();
	updateSchedulingPlan = ui.frmDesign->getUpdateSchedulingPlan();

	evenementHandler = new EvenementHandler(&ui, network, updateSchedulingPlan);
	ssc = new SignalsSlotsConnector(evenementHandler, &ui, this);
//	evenementHandler->setFileName("test.nml");
	evenementHandler->updateMe();
	//evenementHandler->fileUpdated();
	//evenementHandler->fileWasModified();
}

Network * NetworkDesigner::getNetwork() const{
	return network;
}

void NetworkDesigner::setNetwork(Network* network){
	this->network = network;
	ui.frmDesign->setNetwork(network);
}

UpdateSchedulingPlan * NetworkDesigner::getUpdateSchedulingplan() const{
	return updateSchedulingPlan;
}

void NetworkDesigner::setUpdateSchedulingPlan(UpdateSchedulingPlan * updateSchedulingPlan){
	this->updateSchedulingPlan = updateSchedulingPlan;
	ui.frmDesign->setUpdateSchedulingPlan(updateSchedulingPlan);
}

NetworkDesigner::~NetworkDesigner(){
	for(int i=0; i<network->getNbNeurons();i++){
		network->delNeuron(i);
	}
	delete ssc;
	delete evenementHandler;
}
