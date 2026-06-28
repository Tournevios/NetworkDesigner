#include "Simulation.h"
using namespace std;

Simulation::Simulation(Computer * computer)
{
	this->computer = computer;
	connect(computer, SIGNAL(tick()), this, SLOT(tick()));
	name = "Simulation";
	filepath = "simulation.gnu";
	copyNetwork(computer->getNetwork());
	network = computer->getNetwork();
}

Network * Simulation::getInitialNetwork(){
	return initialNetwork;
}

Simulation::Simulation(){

}

Simulation::~Simulation()
{
}

/*
 * name's getter
 */
string Simulation::getName() const{
	return name;
}

/*
 * name's setter
 */
void Simulation::setName(string name){
	this->name = name;
}

/*
 * filepath's getter
 */
string Simulation::getFilepath() const{
	return filepath;
}

/*
 * filepath's setter
 */
void Simulation::setFilepath(string filepath){
	this->filepath = filepath;
}

/*
 * view's setter
 */
void Simulation::setView(UpdateBlock * view){
	this->view = view;
}

/*
 * View's getter
 */

UpdateBlock * Simulation::getView() const{
  	return view;
 }

/*
 * Copie a network to conserve the initial
 */
void Simulation::copyNetwork(Network * network){
	this->initialNetwork = new Network(*network);
}

Computer * Simulation::getComputer() const{
	return computer;
}
