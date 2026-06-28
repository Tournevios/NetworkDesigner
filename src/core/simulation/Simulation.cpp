#include "Simulation.h"

Simulation::Simulation(Computer * computer)
{
	this->computer = computer;
	connect(computer, &Computer::tick, this, &Simulation::tick);
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
std::string Simulation::getName() const{
	return name;
}

/*
 * name's setter
 */
void Simulation::setName(std::string name){
	this->name = name;
}

/*
 * filepath's getter
 */
std::string Simulation::getFilepath() const{
	return filepath;
}

/*
 * filepath's setter
 */
void Simulation::setFilepath(std::string filepath){
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
