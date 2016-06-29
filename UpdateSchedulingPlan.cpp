#include "UpdateSchedulingPlan.h"
using namespace std;

UpdateSchedulingPlan::UpdateSchedulingPlan()
{
	nb_blocks=0;
}

UpdateSchedulingPlan::UpdateSchedulingPlan(const UpdateSchedulingPlan& updateSchedulingPlan){
	nb_blocks = 0;
	for(int i=0; i < updateSchedulingPlan.getNb_blocks(); i++){
		addUpdateBlock(new UpdateBlock(*(updateSchedulingPlan.getUpdateBlock(i))));
	}
}

/*
 * Deleting all the element in the vector
 */
UpdateSchedulingPlan::~UpdateSchedulingPlan()
{
	//if(sequentialblocks.size()>0)
		//sequentialblocks.erase(sequentialblocks.begin(), sequentialblocks.begin()+nb_blocks);

}

/*
 * Return the number of blocks
 */
int UpdateSchedulingPlan::getNb_blocks() const{
	return nb_blocks;
}

/*
 * Add a new block to the update plan
 */
void UpdateSchedulingPlan::addUpdateBlock(UpdateBlock* ub){
	sequentialblocks.push_back(ub);
	nb_blocks++;
}

/*
 * Delete a block from the update plan
 */
void UpdateSchedulingPlan::delUpdateBlock(int index){

	sequentialblocks.erase((vector<UpdateBlock*>::iterator)&sequentialblocks[index]);
	nb_blocks--;
}


/*
 * Return the UpdateBlock from its index
 */
UpdateBlock* UpdateSchedulingPlan::getUpdateBlock(int index) const{
	return sequentialblocks[index];
}
