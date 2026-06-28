#ifndef UPDATEBLOCK_H_
#define UPDATEBLOCK_H_

#include <vector>
#include "constFile.h"

class UpdateBlock
{
public:
	// Constructor and destructor
	UpdateBlock();
	UpdateBlock(const UpdateBlock& updateBlock);
	virtual ~UpdateBlock();

	// Class's methods
	void addNeuronIndex(int index);
	void delNeuronIndex(int index);
	void decrementGreaterThan(int index);

	// Getters and setters
	int getUpdateMethods() const;
	void setUpdateMethods(int UpdateMethods);
	int getSize() const;
	int getNeuronIndex(int i) const;


private:
	std::vector<int> neuronsIndexs;
	int size;
	int UpdateMethods;				// The way that this block on neurons is updated

};

#endif /*UPDATEBLOCK_H_*/
