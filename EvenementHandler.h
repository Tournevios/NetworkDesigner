#ifndef EVENEMENTHANDLER_H_
#define EVENEMENTHANDLER_H_
#include "UpdateSchedulingPlan.h"
#include "ui_mainWindow.h"
#include "constFile.h"
#include "Network.h"
#include "Simulation.h"
#include "BlockSelector.h"


class EvenementHandler : public QWidget
{
	Q_OBJECT
public:
	EvenementHandler(Ui::MainWindow *parent, Network *network, UpdateSchedulingPlan * updateSchedulingPlan);
	virtual ~EvenementHandler();
	void updateMe();

	void setFileName(QString filename);
	QString getFileName() const;
	void cmbState_Updating(int nbStates);

signals:
	void temperatureChanged(double temperature);
	void neuronTemperatureChanged(double temperature);
	void thresholdChanged(double threshold);
	void weightChanged(double);
	void delayChanged(int);
	void currentBlockChanged(int index);
	void currentUpdateMethodsChanged(int index);

	// Introduced on April 12th 2010
	void nodeIDChanged(QString nodeID);
	void nodeIndexChanged(QString index);
	//

	void stateChanged(int);
	void nbStatesChanged(int);
	void quitApplication();
	void updateTitle(QString str);


public slots:
	void action_New_Clicked(bool checked);
	void actionO_pen_Clicked(bool checked);
	void action_Save_Clicked(bool checked);
	void actionSave_As_Clicked(bool checked);
	void action_Export_Premodel(bool checked);
	void action_Quit(bool checked);

	void actionCut_Clicked(bool checked);
	void actionCopy_Clicked(bool checked);
	void actionPaste_Clicked(bool checked);
	void actionDelete_Clicked(bool checked);
	void actionSelect_all_Clicked(bool checked);

	void dsbTemperature_Changed(double temperature);
	void dsbNeuronTemperature_Changed(double temperature);

	void dsbThreshold_Changed(double threshold);
	void sbNbStates_Changed(int nbStates);
	void dsbWeight_Changed(double);

	void cmbState_Changed(int index);

	void cmbUpdateBlock_Changed(int index);
	void cmbUpdateMethods_Changed(int index);

	void pbStart_click(bool checked);
	void pbStop_click(bool checked);

	void neuronSelected(Neuron* neuron);
	void synapseSelected(Synapse* synapse);

	void toolBox_currentChanged(int index);

	void updateMeSlot();
	void chkUniformalTemperature_Changed(int state);
	void sbDelay_Changed(int delay);

	// April 12th 2010
	void txtIndex_Changed(QString index);
	void txtNodeID_Changed(QString nodeID);
	//

	void networkLayersMenu_aboutToShow();

	void repaintPlease();
	void fileWasModified();
	void fileUpdated();
	//void lblLabel_textChanged(const QString & text);


private:
	Ui::MainWindow *parent;
	//QMainWindow* networkDesigner;

	Network * network;
	UpdateSchedulingPlan * updateSchedulingPlan;

	float scale;
	int transX, transY;
	int midX, midY;
	Neuron* currentNeuron;
	Neuron* synapseBaseNeuron;
	int mouseX, mouseY;
	bool saved;
	QString fileName;
	QMenu * networkLayersMenu;

	Network * aSimpleCopy;

	std::vector<BlockSelector> layers;
	std::vector<QAction *> layerActions;
};

#endif /*EVENEMENTHANDLER_H_*/
