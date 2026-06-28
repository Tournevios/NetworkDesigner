#include "UpdateBlock.h"
#include <algorithm>

UpdateBlock::UpdateBlock() : size(0), UpdateMethods(COMPUTE) {}
UpdateBlock::~UpdateBlock() = default;

UpdateBlock::UpdateBlock(const UpdateBlock& other)
    : neuronsIndexs(other.neuronsIndexs)
    , size(other.size)
    , UpdateMethods(other.UpdateMethods)
{}

void UpdateBlock::addNeuronIndex(int index) {
    auto it = std::find(neuronsIndexs.begin(), neuronsIndexs.end(), index);
    if (it == neuronsIndexs.end()) {
        neuronsIndexs.push_back(index);
        ++size;
    }
}

void UpdateBlock::delNeuronIndex(int index) {
    auto it = std::find(neuronsIndexs.begin(), neuronsIndexs.end(), index);
    if (it != neuronsIndexs.end()) {
        neuronsIndexs.erase(it);
        --size;
    }
}

void UpdateBlock::decrementGreaterThan(int index) {
    for (int& idx : neuronsIndexs)
        if (idx > index) --idx;
}

int  UpdateBlock::getUpdateMethods() const        { return UpdateMethods; }
void UpdateBlock::setUpdateMethods(int m)          { UpdateMethods = m; }
int  UpdateBlock::getNeuronIndex(int i) const      { return neuronsIndexs[i]; }
int  UpdateBlock::getSize() const                  { return size; }
