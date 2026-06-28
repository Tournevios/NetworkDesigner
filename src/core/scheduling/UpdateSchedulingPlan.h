#pragma once

#include "UpdateBlock.h"
#include <memory>

class UpdateSchedulingPlan
{

public:

	// Constructors && destuctors
	UpdateSchedulingPlan();
	UpdateSchedulingPlan(const UpdateSchedulingPlan& updateSchedulingPlan);
	virtual ~UpdateSchedulingPlan();

	// Getters && setters
	int getNb_blocks() const;
	void addUpdateBlock(std::unique_ptr<UpdateBlock> ub);
	void delUpdateBlock(int index);
	UpdateBlock* getUpdateBlock(int index) const;

private:
	int nb_blocks;
	std::vector<std::unique_ptr<UpdateBlock>> sequentialblocks;


};
