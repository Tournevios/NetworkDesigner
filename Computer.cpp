#include "Computer.h"

Computer::Computer()
{
}

Computer::Computer(const Computer& computer){
		network = new Network(*(computer.getNetwork()));
		updateSchedulingPlan = new UpdateSchedulingPlan(*(computer.getUpdateSchedulingPlan()));
		nb_iterations = computer.getNbIterations();
		Random::Uniform<double>(0.0,1.0);
		ui = computer.getUi();
		connect(this, SIGNAL(setProgressBarValue_Signal(int)), ui->progressBar, SLOT(setValue(int)));
		repaintDesign = false;
}

Computer::Computer(Network * network, UpdateSchedulingPlan * updateSchedulingPlan, Ui::MainWindow * ui){
	this->network = network;
	this->updateSchedulingPlan = updateSchedulingPlan;
	//this->simulation = simulation;
	nb_iterations = ui->sbUpdatesNumber->value();
	this->ui = ui;
	Random::Uniform<double>(0.0,1.0);
	connect(this, SIGNAL(setProgressBarValue_Signal(int)), ui->progressBar, SLOT(setValue(int)));
	repaintDesign = true;
}

Computer::~Computer()
{
}

Ui::MainWindow * Computer::getUi() const{
	return ui;
}

/*
 * Network's setter
 */
void Computer::setNetwork(Network * network){
	this->network = network;
}

/*
 * Network's getter
 */
Network * Computer::getNetwork() const{
	return network;
}

/*
 * Simulation's setter

void Computer::setSimulation(Simulation * simulation){
	this->simulation = simulation;
}
*/
/*
 * Simulation's getter

Simulation * Computer::getSimulation() const{
	return simulation;
}
*/

/*
 * UpdateScheduler's setter
 */
void Computer::setUpdateSchedulingPlan(UpdateSchedulingPlan * updateSchedulingPlan){
	this->updateSchedulingPlan = updateSchedulingPlan;
}

/*
 * UpdateScheduler's getter
 */
UpdateSchedulingPlan * Computer::getUpdateSchedulingPlan() const{
	return updateSchedulingPlan;
}

/*
 * Compute using block sequential scheduling
 */
void Computer::computeBS(){
	stop = false;
	double synchronyRate = ui->dsbSynchronyRate->value();

	//network->setUniformalTemperature(true);
	int nb_blocks = updateSchedulingPlan->getNb_blocks();
	double rnd;
	for(int k=0; k < nb_iterations; k++){
		if(stop) return;
		for(int i=0;i< nb_blocks;i++){

			UpdateBlock* ub = updateSchedulingPlan->getUpdateBlock(i);
			if(ub->getUpdateMethods() == COMPUTE){
				// All neuron compute their new state using a parallele scheme
				for(int j=0; j < ub->getSize(); j++){
					rnd = Random::Uniform();//(double)rand() / ((double)RAND_MAX+1.0);
					if(rnd <= synchronyRate){
						if(not network->getUniformalTemperature()) network->getNeuron(ub->getNeuronIndex(j))->compute(network->getNeuron(ub->getNeuronIndex(j))->getTemperature());
						else network->getNeuron(ub->getNeuronIndex(j))->compute(network->getTemperature());
					}
				}
				// When all is done theNewState is set
				for(int j=0; j < ub->getSize(); j++)
								network->getNeuron(ub->getNeuronIndex(j))->substitute();
			}

			else if(ub->getUpdateMethods() == FIXE_1){
				for(int j=0; j < ub->getSize(); j++){
					rnd = Random::Uniform();
					if(rnd <= synchronyRate)
								network->getNeuron(ub->getNeuronIndex(j))->setState(true);
				}
			}

			else if(ub->getUpdateMethods() == FIXE_0){
				for(int j=0; j < ub->getSize(); j++){
					rnd = Random::Uniform();
					if(rnd <= synchronyRate)
								network->getNeuron(ub->getNeuronIndex(j))->setState(false);
				}
			}

			else if(ub->getUpdateMethods() == FIXE_01){
				for(int j=0; j < ub->getSize(); j++){
					rnd = Random::Uniform();
					if(rnd <= synchronyRate)
								network->getNeuron(ub->getNeuronIndex(j))->setState((bool)(j%2));
				}
			}

			else if(ub->getUpdateMethods() == FIXE_10){
				for(int j=0; j < ub->getSize(); j++){
					rnd = Random::Uniform();
					if(rnd <= synchronyRate)
								network->getNeuron(ub->getNeuronIndex(j))->setState((bool)((j+1)%2));
				}
			}

			else if(ub->getUpdateMethods() == RANDOMLY){
				for(int j=0; j < ub->getSize(); j++){
					rnd = Random::Uniform();
					if(rnd <= synchronyRate){
						if(Random::Uniform()<0.5)
								network->getNeuron(ub->getNeuronIndex(j))->setState(false);
						else
								network->getNeuron(ub->getNeuronIndex(j))->setState(true);
					}
				}
			}
			emit tick();
		}
		// Mark a little pause and refresh th designPlan widget
		if(repaintDesign){
			if(ui->sbSpeedPercent->value()!=100){
				ui->frmDesign->repaint();
				usleep(100000-ui->sbSpeedPercent->value()*1000);
			}
		}
		//emit tick();
	}
	if(repaintDesign)	ui->frmDesign->repaint();
}

/*
 * Compute using block parallel scheduling
 */
void Computer::computeBP(){
	stop = false;
	double synchronyRate = ui->dsbSynchronyRate->value();

	network->setUniformalTemperature(true);
	int nb_blocks = updateSchedulingPlan->getNb_blocks();
	UpdateBlock* ub;
	double rnd;
	for(int i=0;i< nb_iterations;i++){
		if(stop) return;
			// All neuron compute their new state using a parallele scheme
			for(int j=0; j < nb_blocks; j++){
				ub = updateSchedulingPlan->getUpdateBlock(j);
				rnd = Random::Uniform();
				if(rnd <= synchronyRate){
					if(ub->getUpdateMethods() == COMPUTE){
						if(not network->getUniformalTemperature()) network->getNeuron(ub->getNeuronIndex(i%ub->getSize()))->compute(network->getNeuron(ub->getNeuronIndex(i%ub->getSize()))->getTemperature());
						else network->getNeuron(ub->getNeuronIndex(i%ub->getSize()))->compute(network->getTemperature());
					}
					else if(ub->getUpdateMethods() == FIXE_1){
							network->getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState(true);
					}

					else if(ub->getUpdateMethods() == FIXE_0){
						network->getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState(false);
					}

					else if(ub->getUpdateMethods() == FIXE_01){
						network->getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState((bool)(i%2));
					}

					else if(ub->getUpdateMethods() == FIXE_10){
						network->getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState((bool)((i+1)%2));
					}

					else if(ub->getUpdateMethods() == RANDOMLY){
						if(Random::Uniform()<0.5)
							network->getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState(false);
						else
							network->getNeuron(ub->getNeuronIndex(i%ub->getSize()))->setTheNewState(true);

					}
				}
			}
			// When all is done theNewState is set
			for(int j=0; j < nb_blocks; j++){
				ub = updateSchedulingPlan->getUpdateBlock(j);
				network->getNeuron(ub->getNeuronIndex(i%ub->getSize()))->substitute();
			}
		if(repaintDesign){
			if(ui->sbSpeedPercent->value()!=100){
						ui->frmDesign->repaint();
						usleep(100000-ui->sbSpeedPercent->value()*1000);
			}
		}
			emit tick();
	}
	if(repaintDesign) ui->frmDesign->repaint();
}

/*
 * Compute using parallel scheduling
 */
void Computer::computeP(){
	stop = false;
	double synchronyRate = ui->dsbSynchronyRate->value();
	double rnd;

	for(int i=0;i< nb_iterations;i++){
		if(stop) return;
		// All neuron compute their new state using a parallele scheme
		for(int j=0; j < network->getNbNeurons(); j++){
			rnd = Random::Uniform();//(double)rand() / ((double)RAND_MAX+1.0);
			if(rnd <= synchronyRate){
				if(not network->getUniformalTemperature()) network->getNeuron(j)->compute(network->getNeuron(j)->getTemperature());
				else network->getNeuron(j)->compute(network->getTemperature());
			}
		}
		// When all is done theNewState is set
		for(int j=0; j < network->getNbNeurons(); j++){
							network->getNeuron(j)->substitute();
		}
		if(repaintDesign){
			if(ui->sbSpeedPercent->value()!=100){
				ui->frmDesign->repaint();
				usleep(100000-ui->sbSpeedPercent->value()*1000);
			}
		}
		emit tick();
	}
	if(repaintDesign) ui->frmDesign->repaint();
}

/*
 * Compute using sequential scheduling
 */
void Computer::computeS(){
	stop = false;
	double synchronyRate = ui->dsbSynchronyRate->value();
	double rnd;
	for(int i=0;i< nb_iterations;i++){
		if(stop) return;
		// All neuron compute their new state using a parallele scheme
		for(int j=0; j < network->getNbNeurons(); j++){
			rnd = Random::Uniform();//(double)rand() / ((double)RAND_MAX+1.0);
			if(rnd <= synchronyRate){
				if(not network->getUniformalTemperature()){
					 network->getNeuron(j)->compute(network->getNeuron(j)->getTemperature());
					 network->getNeuron(j)->substitute();
				}
				else {
					network->getNeuron(j)->compute(network->getTemperature());
					network->getNeuron(j)->substitute();
				}
			}
			if(repaintDesign){
				if(ui->sbSpeedPercent->value()!=100){
					ui->frmDesign->repaint();
					usleep(100000-ui->sbSpeedPercent->value()*1000);
				}
			}
		}
		emit tick();
	}
	if(repaintDesign) ui->frmDesign->repaint();
}

int Computer::getNbIterations() const{
	return nb_iterations;
}

void Computer::setNbIterations(int nb_iterations){
	this->nb_iterations = nb_iterations;
}

/*
 * Change the value of the progress bar
 */
void Computer::setProgressBarValue(int value){
	emit setProgressBarValue_Signal(value);
}

void Computer::stopComputing(){
	stop = true;
}

void Computer::setStateOfTheNetwork(NetworkState* networkState){
	for(int i=0; i<this->getNetwork()->getNbNeurons(); i++){
		this->getNetwork()->getNeuron(i)->setState(networkState->getState(i));
	}
}
