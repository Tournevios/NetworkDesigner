#include "BlockSelector.h"


BlockSelector::BlockSelector()
{
}

BlockSelector::BlockSelector(const BlockSelector& blockSelector):QObject(){
	this->updateBlock = new UpdateBlock(*(blockSelector.getUpdateBlock()));
	this->l1_distance = blockSelector.getL1_distance();
	this->network = blockSelector.getNetwork();
}


BlockSelector::~BlockSelector()
{
	this->disconnect();
	delete updateBlock;
}

/*
 * Block selector's constructor
 */
BlockSelector::BlockSelector(Network * network, int l1_distance, int firstIndex){
	this->network = network;
	updateBlock = new UpdateBlock();
	updateBlock->addNeuronIndex(firstIndex);
	this->l1_distance = l1_distance;
}

/*
 * Select all the neuron in the update block
 */
void BlockSelector::select(bool checked){
	network->deselectAll();
	for(int i=0; i<updateBlock->getSize(); i++){
		network->getNeuron(updateBlock->getNeuronIndex(i))->setSelected(true);
	}
	emit repaintPlease();
}

/*
 * Add an index to the neuron
 */
void BlockSelector::addNeuronIndex(int index){
	updateBlock->addNeuronIndex(index);
}

/*
 * L1_distance's getter
 */
int BlockSelector::getL1_distance() const{
	return l1_distance;
}

/*
 * Operator < 's overloading
 */
int operator<(const BlockSelector& left, const BlockSelector& right){
	return left.getL1_distance() < right.getL1_distance();
}

/*
 * UpdateBlock's getter
 */
UpdateBlock * BlockSelector::getUpdateBlock() const{
	return updateBlock;
}

/*
 * Network's getter
 */
Network * BlockSelector::getNetwork() const{
	return network;
}

/*
 * Operator = overloading
 */
BlockSelector& BlockSelector::operator=(const BlockSelector& blockSelector){
	l1_distance = blockSelector.getL1_distance();
	network = blockSelector.getNetwork();
	updateBlock = new UpdateBlock(*(blockSelector.getUpdateBlock()));
	return *this;
}
