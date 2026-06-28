#ifndef UPDATESCHEDULINGPLAN_H_
#define UPDATESCHEDULINGPLAN_H_

#include "UpdateBlock.h"

class UpdateSchedulingPlan
{

public:

	// Constructors and destuctors
	UpdateSchedulingPlan();
	UpdateSchedulingPlan(const UpdateSchedulingPlan& updateSchedulingPlan);
	virtual ~UpdateSchedulingPlan();

	// Getters and setters
	int getNb_blocks() const;
	void addUpdateBlock(UpdateBlock* ub);
	void delUpdateBlock(int index);
	UpdateBlock* getUpdateBlock(int index) const;

private:
	int nb_blocks;
	std::vector<UpdateBlock*> sequentialblocks;


};

#endif /*UPDATESCHEDULINGPLAN_H_*/
