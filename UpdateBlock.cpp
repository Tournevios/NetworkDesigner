#include "UpdateBlock.h"
using namespace std;

UpdateBlock::UpdateBlock()
{
	size = 0;
	UpdateMethods = COMPUTE;
}

UpdateBlock::~UpdateBlock()
{
	neuronsIndexs.clear();
}

UpdateBlock::UpdateBlock(const UpdateBlock& updateBlock){
	size = 0;
	UpdateMethods = COMPUTE;

	this->UpdateMethods = updateBlock.getUpdateMethods();

	for(int i=0; i<updateBlock.getSize();i++){
		addNeuronIndex(updateBlock.getNeuronIndex(i));
	}
}


/*
 * Add a neuron's index to the neuronIndexs
 */
void UpdateBlock::addNeuronIndex(int index){
	vector<int>::iterator vectIterator = neuronsIndexs.begin();
	if(size!= 0){
		while(vectIterator != neuronsIndexs.end()) {
			if(*vectIterator != index){
				vectIterator++;
			}
			else break;
		}
		if(vectIterator == neuronsIndexs.end()) {
			neuronsIndexs.push_back(index);
			size++;
		}
	}
	else{
		neuronsIndexs.push_back(index);
		size++;
	}
}

/*
 * Delete the neuron's index from the vector of the neuronIndexs
 */
void UpdateBlock::delNeuronIndex(int index){
	vector<int>::iterator vectIterator = neuronsIndexs.begin();
	while(vectIterator != neuronsIndexs.end()){
		if(*vectIterator != index)	vectIterator++;
		else break;
	}
	if(vectIterator != neuronsIndexs.end()) {
		neuronsIndexs.erase(vectIterator);
		size--;
	}
}

/*
 * Return the UpdateMethods
 */
int UpdateBlock::getUpdateMethods() const{
	return UpdateMethods;
}

/*
 *  Set the UpdateMethods
 */
void UpdateBlock::setUpdateMethods(int UpdateMethods){
	this->UpdateMethods = UpdateMethods;
}

/*
 * Return the index of the neuron at the ith place in the vector neuronsIndexs
 */
int UpdateBlock::getNeuronIndex(int i) const{
	return neuronsIndexs[i];
}

/*
 * Decrement the index of the neurons which their indexs is greater than i
 */
void UpdateBlock::decrementGreaterThan(int index){
	for(int i=0; i < size; i++){
		if(neuronsIndexs[i]>index) neuronsIndexs[i]--;
	}
}

/*
 * Return the size of the UpdateBlock
 */
int UpdateBlock::getSize() const{
	return size;
}

