#include "SimulationDiffusion.h"
#include <QtCore/QCoreApplication>

SimulationDiffusion::SimulationDiffusion(Computer * computer):Simulation(computer)
{
	name = "Activity";
	filepath = "simulation.gnu";
	affinity = 1000;
	numberOfIterations = 0;
}

SimulationDiffusion::~SimulationDiffusion()
{
}

void SimulationDiffusion::run(){

	int nbStates = initialNetwork->getNeuron(0)->getNbStates();
	numberOfIterations = computer->getNbIterations();
	double temperature = initialNetwork->getTemperature();

	frequencyOfStates.clear();
	while(static_cast<int>(frequencyOfStates.size())<computer->getNetwork()->getNeuron(0)->getNbStates()){
		frequencyOfStates.push_back(0);
	}


	std::string datfile = getFilepath() +".dat";
	std::ofstream out("simulation.dat");
	int tmpRt;
	for(int j=1; j < 1000; j++){
		// This sweep runs ~10^6 network updates synchronously: keep the UI
		// alive and honor the Stop button between temperature steps.
		QCoreApplication::processEvents();
		if(computer->isAbortRequested()) break;
		if(j!=1000-1) temperature -= static_cast<double>(initialNetwork->getTemperature()/1000);
		else temperature = 0;
		computer->setNetwork(new Network(*initialNetwork));
		computer->getNetwork()->setTemperature(temperature);
		frequencyOfJumps.clear();
		while(static_cast<int>(frequencyOfJumps.size())<computer->getNetwork()->getNeuron(0)->getNbStates()){
			frequencyOfJumps.push_back(0);
		}
		for(int i=0; i < affinity; i++){
			computer->getNetwork()->getNeuron(0)->setState(static_cast<int>(nbStates/2));
			if(updateType == UpdateType::P)		computer->computeP();
			tmpRt = computer->getNetwork()->getNeuron(0)->getState() - (static_cast<int>(nbStates/2));
			frequencyOfJumps[tmpRt + (static_cast<int>(nbStates/2))] ++;
			computer->setProgressBarValue(static_cast<int>((j*affinity+(i+1))*100/(static_cast<double>(affinity)*1000)));
		}
		delete computer->getNetwork();
		for(int i=-(static_cast<int>(nbStates/2)); i <= nbStates - 1 -(static_cast<int>(nbStates/2)); i++){
			out << i << " " << temperature << " " << frequencyOfJumps[i+(static_cast<int>(nbStates/2))] << "\n";
		}
	}

	//out << temperature << " " << ((double)oneRating/numberOfIterations)*100 << "\n";
	std::ofstream outMe("simulation.gp");
	outMe << "set output \"./sim/" << initialNetwork->getTemperature() << ".ps\"\n";
	outMe << "set terminal postscript\n";
	outMe << "set title \"Diffusion at T = "<< initialNetwork->getTemperature() << " to 0\"\n";
	outMe << "set xlabel \"Rt\"\n";
	outMe << "set ylabel \"Temperature\"\n";
	outMe << "set zlabel \"Number of particles\"\n";
	outMe << "set autoscale y\n";
	//outMe << "set yrange [0:101]\n";
	//outMe << "set xtic auto\n";
	//outMe << "set ytic auto\n";
	outMe << "splot \"simulation.dat\" title 'Diffusion particles' with lines\n";
	computer->setNetwork(new Network(*initialNetwork));
}

/*
 * Affinity's setter
 */
void SimulationDiffusion::setAffinity(int affinity){
	this->affinity = affinity;
}

/*
 * Affinity's getter
 */
inline int SimulationDiffusion::getAffinity() const{
	return affinity;
}

/*
 * UpdateType's setter
 */
void SimulationDiffusion::setUpdateType(UpdateType updateType){
	this->updateType = updateType;
}

/*
 * UpdateType's getter
 */
inline UpdateType SimulationDiffusion::getUpdateType() const{
	return updateType;
}

void SimulationDiffusion::tick(){


}

SimulationDiffusion::SimulationDiffusion()
{
}
