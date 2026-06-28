#include "SignalsSlotsConnector.h"


SignalsSlotsConnector::SignalsSlotsConnector(EvenementHandler * evenementHandler, Ui::MainWindow * ui, QMainWindow * theWindow)
{
	    connect(ui->actionO_pen, &QAction::triggered, evenementHandler, &EvenementHandler::actionO_pen_Clicked);
	    connect(ui->action_New, &QAction::triggered, evenementHandler, &EvenementHandler::action_New_Clicked);
	    connect(ui->action_Save, &QAction::triggered, evenementHandler, &EvenementHandler::action_Save_Clicked);
	    connect(ui->actionSave_As, &QAction::triggered, evenementHandler, &EvenementHandler::actionSave_As_Clicked);

	    connect(ui->actionExport_To_GNBox_Premodel, &QAction::triggered, evenementHandler, &EvenementHandler::action_Export_Premodel);

	    connect(ui->action_Quit, &QAction::triggered, evenementHandler, &EvenementHandler::action_Quit);

	    connect(evenementHandler, &EvenementHandler::quitApplication, theWindow, &QMainWindow::close);
	    connect(evenementHandler, &EvenementHandler::updateTitle, theWindow, &QMainWindow::setWindowTitle);

	    connect(ui->actionCut, &QAction::triggered, evenementHandler, &EvenementHandler::actionCut_Clicked);
	    connect(ui->actionCopy, &QAction::triggered, evenementHandler, &EvenementHandler::actionCopy_Clicked);
	    connect(ui->actionPaste, &QAction::triggered, evenementHandler, &EvenementHandler::actionPaste_Clicked);
	    connect(ui->actionDelete, &QAction::triggered, evenementHandler, &EvenementHandler::actionDelete_Clicked);
	    connect(ui->actionSelect_All, &QAction::triggered, evenementHandler, &EvenementHandler::actionSelect_all_Clicked);

	    connect(ui->dsbTemperature, QOverload<double>::of(&QDoubleSpinBox::valueChanged), evenementHandler, &EvenementHandler::dsbTemperature_Changed);
	    connect(evenementHandler, &EvenementHandler::temperatureChanged, ui->dsbTemperature, &QDoubleSpinBox::setValue);

	    connect(ui->pbStart, &QPushButton::clicked, evenementHandler, &EvenementHandler::pbStart_click);
	    connect(ui->pbStop, &QPushButton::clicked, evenementHandler, &EvenementHandler::pbStop_click);

	    connect(ui->dsbThreshold, QOverload<double>::of(&QDoubleSpinBox::valueChanged), evenementHandler, &EvenementHandler::dsbThreshold_Changed);
		connect(ui->sbNbStates, QOverload<int>::of(&QSpinBox::valueChanged), evenementHandler, &EvenementHandler::sbNbStates_Changed);

	    connect(ui->cmbState, QOverload<int>::of(&QComboBox::currentIndexChanged), evenementHandler, &EvenementHandler::cmbState_Changed);
	    connect(ui->frmDesign, &DesignPlan::neuronSelectedChanged, evenementHandler, &EvenementHandler::neuronSelected);
	    connect(evenementHandler, &EvenementHandler::thresholdChanged, ui->dsbThreshold, &QDoubleSpinBox::setValue);
	    connect(evenementHandler, &EvenementHandler::nbStatesChanged, ui->sbNbStates, &QSpinBox::setValue);
    	connect(evenementHandler, &EvenementHandler::stateChanged, ui->cmbState, &QComboBox::setCurrentIndex);

	    connect(ui->cmbUpdateBlock, QOverload<int>::of(&QComboBox::activated), evenementHandler, &EvenementHandler::cmbUpdateBlock_Changed);
	    connect(evenementHandler, &EvenementHandler::currentBlockChanged, ui->cmbUpdateBlock, &QComboBox::setCurrentIndex);

	    connect(ui->toolBox, &QToolBox::currentChanged, evenementHandler, &EvenementHandler::toolBox_currentChanged);
	    connect(ui->frmDesign, &DesignPlan::blockModeInvoked, ui->toolBox, &QToolBox::setCurrentIndex);

		connect(ui->cmbUpdateMethods, QOverload<int>::of(&QComboBox::activated), evenementHandler, &EvenementHandler::cmbUpdateMethods_Changed);
	    connect(evenementHandler, &EvenementHandler::currentUpdateMethodsChanged, ui->cmbUpdateMethods, &QComboBox::setCurrentIndex);

		connect(ui->dsbWeight, QOverload<double>::of(&QDoubleSpinBox::valueChanged), evenementHandler, &EvenementHandler::dsbWeight_Changed);
	    connect(ui->frmDesign, &DesignPlan::synapseSelectedChanged, evenementHandler, &EvenementHandler::synapseSelected);
	    connect(evenementHandler, &EvenementHandler::weightChanged, ui->dsbWeight, &QDoubleSpinBox::setValue);

		connect(ui->frmDesign, &DesignPlan::updateCalled, evenementHandler, &EvenementHandler::updateMeSlot);
		connect(ui->frmDesign, &DesignPlan::networkIsModified, evenementHandler, &EvenementHandler::fileWasModified);
		connect(ui->chkUniformalTemperature, &QCheckBox::stateChanged, evenementHandler, &EvenementHandler::chkUniformalTemperature_Changed);

		connect(ui->dsbNeuronTemperature, QOverload<double>::of(&QDoubleSpinBox::valueChanged), evenementHandler, &EvenementHandler::dsbNeuronTemperature_Changed);
	    connect(evenementHandler, &EvenementHandler::neuronTemperatureChanged, ui->dsbNeuronTemperature, &QDoubleSpinBox::setValue);

		connect(ui->sbDelay, QOverload<int>::of(&QSpinBox::valueChanged), evenementHandler, &EvenementHandler::sbDelay_Changed);
	    connect(evenementHandler, &EvenementHandler::delayChanged, ui->sbDelay, &QSpinBox::setValue);

	    // 12th of April 2010
	    connect(ui->txtIndex, &QLineEdit::textChanged, evenementHandler, &EvenementHandler::txtIndex_Changed);
	    connect(evenementHandler, &EvenementHandler::nodeIndexChanged, ui->txtIndex, &QLineEdit::setText);

	    connect(ui->txtNodeID, &QLineEdit::textChanged, evenementHandler, &EvenementHandler::txtNodeID_Changed);
	    connect(evenementHandler, &EvenementHandler::nodeIDChanged, ui->txtNodeID, &QLineEdit::setText);
	    /////////
}

SignalsSlotsConnector::~SignalsSlotsConnector()
{
}
