#include "UpdateSchedulingPlan.h"
#include <algorithm>

UpdateSchedulingPlan::UpdateSchedulingPlan() : nb_blocks(0) {}

UpdateSchedulingPlan::UpdateSchedulingPlan(const UpdateSchedulingPlan& other) : nb_blocks(0) {
    for (int i = 0; i < other.getNb_blocks(); ++i)
        addUpdateBlock(new UpdateBlock(*other.getUpdateBlock(i)));
}

UpdateSchedulingPlan::~UpdateSchedulingPlan() {
    for (UpdateBlock* ub : sequentialblocks) delete ub;
}

int UpdateSchedulingPlan::getNb_blocks() const { return nb_blocks; }

void UpdateSchedulingPlan::addUpdateBlock(UpdateBlock* ub) {
    sequentialblocks.push_back(ub);
    ++nb_blocks;
}

void UpdateSchedulingPlan::delUpdateBlock(int index) {
    delete sequentialblocks[index];
    sequentialblocks.erase(sequentialblocks.begin() + index);
    --nb_blocks;
}

UpdateBlock* UpdateSchedulingPlan::getUpdateBlock(int index) const {
    return sequentialblocks[index];
}
