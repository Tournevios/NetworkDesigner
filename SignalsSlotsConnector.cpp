#include "SignalsSlotsConnector.h"


SignalsSlotsConnector::SignalsSlotsConnector(EvenementHandler * evenementHandler, Ui::MainWindow * ui, QMainWindow * theWindow)
{
	    connect(ui->actionO_pen, SIGNAL(triggered(bool)), evenementHandler, SLOT(actionO_pen_Clicked(bool)));
	    connect(ui->action_New, SIGNAL(triggered(bool)), evenementHandler, SLOT(action_New_Clicked(bool)));
	    connect(ui->action_Save, SIGNAL(triggered(bool)), evenementHandler, SLOT(action_Save_Clicked(bool)));
	    connect(ui->actionSave_As, SIGNAL(triggered(bool)), evenementHandler, SLOT(actionSave_As_Clicked(bool)));

	    connect(ui->actionExport_To_GNBox_Premodel, SIGNAL(triggered(bool)), evenementHandler, SLOT(action_Export_Premodel(bool)));

	    connect(ui->action_Quit, SIGNAL(triggered(bool)), evenementHandler, SLOT(action_Quit(bool)));

	    connect((QWidget*)evenementHandler, SIGNAL(quitApplication()), theWindow, SLOT(close()));
	    connect((QWidget*)evenementHandler, SIGNAL(updateTitle(QString)), theWindow, SLOT(setWindowTitle(QString)));

	    connect(ui->actionCut, SIGNAL(triggered(bool)), evenementHandler, SLOT(actionCut_Clicked(bool)));
	    connect(ui->actionCopy, SIGNAL(triggered(bool)), evenementHandler, SLOT(actionCopy_Clicked(bool)));
	    connect(ui->actionPaste, SIGNAL(triggered(bool)), evenementHandler, SLOT(actionPaste_Clicked(bool)));
	    connect(ui->actionDelete, SIGNAL(triggered(bool)), evenementHandler, SLOT(actionDelete_Clicked(bool)));
	    connect(ui->actionSelect_All, SIGNAL(triggered(bool)), evenementHandler, SLOT(actionSelect_all_Clicked(bool)));

	    connect(ui->dsbTemperature, SIGNAL(valueChanged(double)), evenementHandler, SLOT(dsbTemperature_Changed(double)));
	    connect(evenementHandler, SIGNAL(temperatureChanged(double)), ui->dsbTemperature,SLOT(setValue(double)));

	    connect(ui->pbStart, SIGNAL(clicked(bool)), evenementHandler,SLOT(pbStart_click(bool)));
	    connect(ui->pbStop, SIGNAL(clicked(bool)), evenementHandler,SLOT(pbStop_click(bool)));

	    connect(ui->dsbThreshold, SIGNAL(valueChanged(double)), evenementHandler, SLOT(dsbThreshold_Changed(double)));
		connect(ui->sbNbStates, SIGNAL(valueChanged(int)), evenementHandler, SLOT(sbNbStates_Changed(int)));

	    connect(ui->cmbState, SIGNAL(currentIndexChanged(int)), evenementHandler, SLOT(cmbState_Changed(int)));
	    connect(ui->frmDesign, SIGNAL(neuronSelectedChanged(Neuron*)), evenementHandler, SLOT(neuronSelected(Neuron*)));
	    connect(evenementHandler, SIGNAL(thresholdChanged(double)), ui->dsbThreshold,SLOT(setValue(double)));
	    connect(evenementHandler, SIGNAL(nbStatesChanged(int)), ui->sbNbStates,SLOT(setValue(int)));
    	connect(evenementHandler, SIGNAL(stateChanged(int)), ui->cmbState,SLOT(setCurrentIndex(int)));

	    connect(ui->cmbUpdateBlock, SIGNAL(activated(int)), evenementHandler,SLOT(cmbUpdateBlock_Changed(int)));
	    connect(evenementHandler, SIGNAL(currentBlockChanged(int)), ui->cmbUpdateBlock,SLOT(setCurrentIndex(int)));

	    connect(ui->toolBox, SIGNAL(currentChanged(int)), evenementHandler, SLOT(toolBox_currentChanged(int)));
	    connect(ui->frmDesign, SIGNAL(blockModeInvoked(int)), ui->toolBox, SLOT(setCurrentIndex(int)));

		connect(ui->cmbUpdateMethods, SIGNAL(activated(int)), evenementHandler,SLOT(cmbUpdateMethods_Changed(int)));
	    connect(evenementHandler, SIGNAL(currentUpdateMethodsChanged(int)), ui->cmbUpdateMethods,SLOT(setCurrentIndex(int)));

		connect(ui->dsbWeight, SIGNAL(valueChanged(double)), evenementHandler, SLOT(dsbWeight_Changed(double)));
	    connect(ui->frmDesign, SIGNAL(synapseSelectedChanged(Synapse*)), evenementHandler, SLOT(synapseSelected(Synapse*)));
	    connect(evenementHandler, SIGNAL(weightChanged(double)), ui->dsbWeight,SLOT(setValue(double)));

		connect(ui->frmDesign, SIGNAL(updateCalled()), evenementHandler, SLOT(updateMeSlot()));
		connect(ui->frmDesign, SIGNAL(networkIsModified()), evenementHandler, SLOT(fileWasModified()));
		connect(ui->chkUniformalTemperature, SIGNAL(stateChanged(int)), evenementHandler, SLOT(chkUniformalTemperature_Changed(int)));

		connect(ui->dsbNeuronTemperature, SIGNAL(valueChanged(double)), evenementHandler, SLOT(dsbNeuronTemperature_Changed(double)));
	    connect(evenementHandler, SIGNAL(neuronTemperatureChanged(double)), ui->dsbNeuronTemperature,SLOT(setValue(double)));

		connect(ui->sbDelay, SIGNAL(valueChanged(int)), evenementHandler, SLOT(sbDelay_Changed(int)));
	    connect(evenementHandler, SIGNAL(delayChanged(int)), ui->sbDelay,SLOT(setValue(int)));

	    // 12th of April 2010
	    connect(ui->txtIndex, SIGNAL(textChanged(QString)), evenementHandler, SLOT(txtIndex_Changed(QString)));
	    connect(evenementHandler, SIGNAL(nodeIndexChanged(QString)), ui->txtIndex,SLOT(setText(QString)));

	    connect(ui->txtNodeID, SIGNAL(textChanged(QString)), evenementHandler, SLOT(txtNodeID_Changed(QString)));
	    connect(evenementHandler, SIGNAL(nodeIDChanged(QString)), ui->txtNodeID,SLOT(setText(QString)));
	    /////////
}

SignalsSlotsConnector::~SignalsSlotsConnector()
{
}
