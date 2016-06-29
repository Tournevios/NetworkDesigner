#include "SimulationChangement.h"
using namespace std;

SimulationChangement::SimulationChangement(Computer * computer):Simulation(computer)
{
	name = "Activity";
	filepath = "simulation.gnu";
	affinity = 100;

	totalNumberOfChangeToOne = 0;
	totalNumberOfChangeToZero = 0;
	totalNumberOfZeroHolding = 0;
	totalNumberOfOneHolding = 0;
	numberOfIterations = 0;
	changeToOneRating = 0;
	changeToZeroRating = 0;
	holdingInOneRating = 0;
	holdingInTwoRating = 0;
	changeRating = 0;

	numberOfIterations = 0;
}

SimulationChangement::~SimulationChangement()
{
}

void SimulationChangement::run(){

	double temperature = initialNetwork->getTemperature();
	numberOfIterations = computer->getNbIterations();

	string datfile = getFilepath() +".dat";
	ofstream out("simulation.dat");

	for(int i=0; i < affinity; i++){
		if(i!=affinity-1) temperature -= (double)(initialNetwork->getTemperature()/affinity);
		else temperature = 0;
		computer->setNetwork(new Network(*initialNetwork));
		computer->getNetwork()->setTemperature(temperature);

		oldNetworkState.clear();
		for(int j=0; j<view->getSize(); j++){
			oldNetworkState.push_back(network->getNeuron(view->getNeuronIndex(j))->getState());
		}

		if(updateType == P)		computer->computeP();
		else if(updateType == BS) computer->computeBS();
		else if(updateType == S) computer->computeS();
		else if(updateType == BP) computer->computeBP();
		out << temperature << " " << ((double)changeToOneRating/numberOfIterations)*100 << "\n";

		/*
		totalNumberOfChangeToOne = 0;
		totalNumberOfChangeToZero = 0;
		totalNumberOfZeroHolding = 0;
		totalNumberOfOneHolding = 0;
		changeToOneRating = 0;
		changeToZeroRating = 0;
		holdingInOneRating = 0;
		holdingInTwoRating = 0;
		*/
		changeToOneRating = 0;
		changeToZeroRating = 0;
		changeRating = 0;
		holdingInOneRating = 0;
		holdingInTwoRating = 0;
		computer->setProgressBarValue((int)((i+1)*100/(double)affinity));
		delete computer->getNetwork();
	}
	ofstream outMe("simulation.gp");
	outMe << "set title \"Activity variation of the network\"\n";
	outMe << "set xlabel \"Temperature\"\n";
	outMe << "set ylabel \"Percentage %\"\n";
	outMe << "set yrange [0:101]\n";
	//outMe << "set xtic auto\n";
	//outMe << "set ytic auto\n";
	outMe << "plot \"simulation.dat\" title 'Change to one rate' with lines\n";
	computer->setNetwork(new Network(*initialNetwork));
}

/*
 * Affinity's setter
 */
void SimulationChangement::setAffinity(int affinity){
	this->affinity = affinity;
}

/*
 * Affinity's getter
 */
inline int SimulationChangement::getAffinity() const{
	return affinity;
}

/*
 * UpdateType's setter
 */
void SimulationChangement::setUpdateType(int updateType){
	this->updateType = updateType;
}

/*
 * UpdateType's getter
 */
inline int SimulationChangement::getUpdateType() const{
	return updateType;
}

void SimulationChangement::tick(){

	int numberOfChangeToOne = 0;
	int numberOfChangeToZero = 0;

	int numberOfOneHolding = 0;
	int numberOfZeroHolding = 0;

	for(int i=0; i<view->getSize(); i++){
		if((computer->getNetwork()->getNeuron(view->getNeuronIndex(i))->getState()) != oldNetworkState[i]){
			if(oldNetworkState[i] == 1) numberOfChangeToZero++;
			else numberOfChangeToOne++;
		}
		else{
			if(oldNetworkState[i] == 1) numberOfOneHolding++;
			else numberOfZeroHolding++;
		}
		oldNetworkState[i] = computer->getNetwork()->getNeuron(view->getNeuronIndex(i))->getState();
	}

	changeToOneRating += ((double)numberOfChangeToOne / view->getSize());
	changeToZeroRating += ((double)numberOfChangeToZero / view->getSize());
	changeRating += changeToZeroRating + changeToOneRating;
	holdingInOneRating += ((double)numberOfOneHolding / view->getSize());
	holdingInTwoRating += ((double)numberOfZeroHolding / view->getSize());


}
