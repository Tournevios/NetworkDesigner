#include "SimulationActivity.h"
using namespace std;

SimulationActivity::SimulationActivity(Computer * computer):Simulation(computer)
{
	name = "Activity";
	filepath = "simulation.gnu";
	affinity = 100;
	totalNumberOfOne = 0;
	totalNumberOfZero = 0;
	oneRating = 0;
	zeroRating = 0;
	numberOfIterations = 0;
}

SimulationActivity::~SimulationActivity()
{
}

void SimulationActivity::run(){

	double temperature = initialNetwork->getTemperature();
	numberOfIterations = computer->getNbIterations();

	//string datfile = getFilepath() +".dat";
	//ofstream out("simulation.dat");
	countIter = 0;
	datfile = getFilepath() +".dat";
	out = new ofstream("simulation.dat");
	computer->setNetwork(new Network(*initialNetwork));
	computer->getNetwork()->setTemperature(temperature);

/*	for(int i=0; i < affinity; i++){
		if(i!=affinity-1) temperature -= (double)(initialNetwork->getTemperature()/affinity);
		else temperature = 0;
		computer->setNetwork(new Network(*initialNetwork));
		computer->getNetwork()->setTemperature(temperature);
		if(updateType == P)		computer->computeP();
		else if(updateType == BS) computer->computeBS();
		else if(updateType == S) computer->computeS();
		else if(updateType == BP) computer->computeBP();
		out << temperature << " " << ((double)oneRating/numberOfIterations)*100 << "\n";
		totalNumberOfOne = 0;
		totalNumberOfZero = 0;
		zeroRating = 0;
		oneRating = 0;
		computer->setProgressBarValue((int)((i+1)*100/(double)affinity));
		delete computer->getNetwork();
	}
	*/
	if(updateType == BS) computer->computeBS();
	ofstream outMe("simulation.gp");
	outMe << "set title \"Histogramme\"\n";
	outMe << "set xlabel \"Iteration\"\n";
	outMe << "set ylabel \"State\"\n";
	outMe << "set yrange [0:3]\n";
/*	outMe << "set title \"Activity of the network\"\n";
	outMe << "set xlabel \"Temperature\"\n";
	outMe << "set ylabel \"Percentage %\"\n";
	outMe << "set yrange [0:101]\n";*/
	//outMe << "set xtic auto\n";
	//outMe << "set ytic auto\n";
	//outMe << "plot \"simulation.dat\" title 'Activity rate' with lines\n";
	outMe << "plot \"simulation.dat\" using 1:2 title 'Pulmonary' with lines, \"simulation.dat\" using 1:3 title 'Heart' with lines\n";
	computer->setNetwork(new Network(*initialNetwork));
	delete out;
}

/*
 * Affinity's setter
 */
void SimulationActivity::setAffinity(int affinity){
	this->affinity = affinity;
}

/*
 * Affinity's getter
 */
inline int SimulationActivity::getAffinity() const{
	return affinity;
}

/*
 * UpdateType's setter
 */
void SimulationActivity::setUpdateType(int updateType){
	this->updateType = updateType;
}

/*
 * UpdateType's getter
 */
inline int SimulationActivity::getUpdateType() const{
	return updateType;
}

void SimulationActivity::tick(){
	/*int numberOfOne = 0;
	int numberOfZero = 0;

	for(int i=0; i<view->getSize(); i++){
		if(computer->getNetwork()->getNeuron(view->getNeuronIndex(i))->getState())
			numberOfOne++;
		else
			numberOfZero++;
	}
	totalNumberOfOne += numberOfOne;
	totalNumberOfZero += numberOfZero;
	oneRating += ((double)numberOfOne / view->getSize());
	zeroRating += ((double)numberOfZero / view->getSize());*/
	*out << countIter << " " << (int)(computer->getNetwork()->getNeuron(0)->getState() * 2) << " " << computer->getNetwork()->getNeuron(2)->getState()<< "\n";
	countIter++;
}
