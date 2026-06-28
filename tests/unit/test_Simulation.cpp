// Simulation is an abstract QThread subclass — it cannot be instantiated directly
// and requires a Qt event loop. Tests for concrete simulation subclasses
// (SimulationActivity, SimulationDiffusion, etc.) belong here once a lightweight
// QCoreApplication fixture is introduced.
//
// For now this file covers UpdateBlock, which underpins all simulation scheduling
// and has no Qt dependency.

#include <gtest/gtest.h>
#include "UpdateBlock.h"
#include "constFile.h"

// ── Construction ──────────────────────────────────────────────────────────────

TEST(UpdateBlockTest, DefaultConstructorEmptySize) {
    UpdateBlock ub;
    EXPECT_EQ(ub.getSize(), 0);
}

TEST(UpdateBlockTest, DefaultUpdateMethodIsCompute) {
    UpdateBlock ub;
    EXPECT_EQ(ub.getUpdateMethods(), COMPUTE);
}

// ── addNeuronIndex ────────────────────────────────────────────────────────────

TEST(UpdateBlockTest, AddNeuronIndexIncreasesSize) {
    UpdateBlock ub;
    ub.addNeuronIndex(0);
    EXPECT_EQ(ub.getSize(), 1);
}

TEST(UpdateBlockTest, AddMultipleNeuronIndices) {
    UpdateBlock ub;
    ub.addNeuronIndex(0);
    ub.addNeuronIndex(2);
    ub.addNeuronIndex(5);
    EXPECT_EQ(ub.getSize(), 3);
}

TEST(UpdateBlockTest, AddDuplicateIndexIgnored) {
    UpdateBlock ub;
    ub.addNeuronIndex(3);
    ub.addNeuronIndex(3);
    EXPECT_EQ(ub.getSize(), 1);
}

TEST(UpdateBlockTest, GetNeuronIndexReturnsCorrectValue) {
    UpdateBlock ub;
    ub.addNeuronIndex(7);
    ub.addNeuronIndex(2);
    EXPECT_EQ(ub.getNeuronIndex(0), 7);
    EXPECT_EQ(ub.getNeuronIndex(1), 2);
}

// ── delNeuronIndex ────────────────────────────────────────────────────────────

TEST(UpdateBlockTest, DelNeuronIndexDecreasesSize) {
    UpdateBlock ub;
    ub.addNeuronIndex(1);
    ub.addNeuronIndex(2);
    ub.delNeuronIndex(1);
    EXPECT_EQ(ub.getSize(), 1);
}

TEST(UpdateBlockTest, DelNonExistentIndexDoesNothing) {
    UpdateBlock ub;
    ub.addNeuronIndex(0);
    ub.delNeuronIndex(99);
    EXPECT_EQ(ub.getSize(), 1);
}

// ── decrementGreaterThan ──────────────────────────────────────────────────────

TEST(UpdateBlockTest, DecrementGreaterThanShiftsHigherIndices) {
    UpdateBlock ub;
    ub.addNeuronIndex(0);
    ub.addNeuronIndex(2);
    ub.addNeuronIndex(4);
    ub.decrementGreaterThan(1); // indices > 1 become index - 1
    EXPECT_EQ(ub.getNeuronIndex(0), 0);
    EXPECT_EQ(ub.getNeuronIndex(1), 1); // was 2
    EXPECT_EQ(ub.getNeuronIndex(2), 3); // was 4
}

TEST(UpdateBlockTest, DecrementGreaterThanDoesNotAffectLowerOrEqual) {
    UpdateBlock ub;
    ub.addNeuronIndex(0);
    ub.addNeuronIndex(1);
    ub.decrementGreaterThan(1); // only strictly greater than 1 affected
    EXPECT_EQ(ub.getNeuronIndex(0), 0);
    EXPECT_EQ(ub.getNeuronIndex(1), 1);
}

// ── setUpdateMethods ──────────────────────────────────────────────────────────

TEST(UpdateBlockTest, SetUpdateMethods) {
    UpdateBlock ub;
    ub.setUpdateMethods(FIXE);
    EXPECT_EQ(ub.getUpdateMethods(), FIXE);
}

TEST(UpdateBlockTest, SetUpdateMethodsRandomly) {
    UpdateBlock ub;
    ub.setUpdateMethods(RANDOMLY);
    EXPECT_EQ(ub.getUpdateMethods(), RANDOMLY);
}

// ── Copy constructor ──────────────────────────────────────────────────────────

TEST(UpdateBlockTest, CopyConstructorPreservesSize) {
    UpdateBlock ub;
    ub.addNeuronIndex(1);
    ub.addNeuronIndex(3);
    ub.setUpdateMethods(FIXE_1);

    UpdateBlock copy(ub);
    EXPECT_EQ(copy.getSize(), 2);
    EXPECT_EQ(copy.getUpdateMethods(), FIXE_1);
    EXPECT_EQ(copy.getNeuronIndex(0), 1);
    EXPECT_EQ(copy.getNeuronIndex(1), 3);
}

TEST(UpdateBlockTest, CopyConstructorIsIndependent) {
    UpdateBlock ub;
    ub.addNeuronIndex(0);
    UpdateBlock copy(ub);
    copy.addNeuronIndex(1);
    EXPECT_EQ(ub.getSize(), 1);   // original unchanged
    EXPECT_EQ(copy.getSize(), 2);
}
