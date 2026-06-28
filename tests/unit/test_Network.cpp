#include <gtest/gtest.h>
#include "Network.h"

// ── Construction ──────────────────────────────────────────────────────────────

TEST(NetworkTest, DefaultConstructorTemperature) {
    Network net;
    EXPECT_DOUBLE_EQ(net.getTemperature(), 0.001);
}

TEST(NetworkTest, ConstructorWithTemperature) {
    Network net(0.5);
    EXPECT_DOUBLE_EQ(net.getTemperature(), 0.5);
}

TEST(NetworkTest, DefaultConstructorEmpty) {
    Network net;
    EXPECT_EQ(net.getNbNeurons(), 0);
}

TEST(NetworkTest, DefaultUniformalTemperatureIsTrue) {
    Network net;
    EXPECT_TRUE(net.getUniformalTemperature());
}

// ── Neurons add / get ─────────────────────────────────────────────────────────

TEST(NetworkTest, AddNeuronIncreasesCount) {
    Network net;
    net.addNeuron(new Neuron());
    EXPECT_EQ(net.getNbNeurons(), 1);
}

TEST(NetworkTest, AddMultipleNeurons) {
    Network net;
    net.addNeuron(new Neuron());
    net.addNeuron(new Neuron());
    net.addNeuron(new Neuron());
    EXPECT_EQ(net.getNbNeurons(), 3);
}

TEST(NetworkTest, GetNeuronByIndexReturnsCorrectPointer) {
    Network net;
    net.addNeuron(new Neuron());
    net.addNeuron(new Neuron());
    Neuron* n = net.getNeuron(1);
    ASSERT_NE(n, nullptr);
    EXPECT_EQ(n->getIndex(), 1);
}

TEST(NetworkTest, GetNeuronOutOfBoundsReturnsNull) {
    Network net;
    net.addNeuron(new Neuron());
    EXPECT_EQ(net.getNeuron(5), nullptr);
    EXPECT_EQ(net.getNeuron(-1), nullptr);
}

TEST(NetworkTest, AddNeuronSetsIndexAutomatically) {
    Network net;
    net.addNeuron(new Neuron(99)); // raw index ignored — Network reassigns it
    net.addNeuron(new Neuron(99));
    EXPECT_EQ(net.getNeuron(0)->getIndex(), 0);
    EXPECT_EQ(net.getNeuron(1)->getIndex(), 1);
}

// ── Delete neuron ─────────────────────────────────────────────────────────────

TEST(NetworkTest, DelNeuronDecreasesCount) {
    Network net;
    net.addNeuron(new Neuron());
    net.addNeuron(new Neuron());
    net.delNeuron(0);
    EXPECT_EQ(net.getNbNeurons(), 1);
}

TEST(NetworkTest, DelNeuronShiftsIndices) {
    Network net;
    net.addNeuron(new Neuron());
    net.addNeuron(new Neuron());
    net.addNeuron(new Neuron());
    net.delNeuron(0);
    // Remaining neurons should have indices 0 and 1
    EXPECT_EQ(net.getNeuron(0)->getIndex(), 0);
    EXPECT_EQ(net.getNeuron(1)->getIndex(), 1);
}

TEST(NetworkTest, DelNeuronInvalidIndexDoesNothing) {
    Network net;
    net.addNeuron(new Neuron());
    net.delNeuron(99);
    EXPECT_EQ(net.getNbNeurons(), 1);
}

// ── Temperature ───────────────────────────────────────────────────────────────

TEST(NetworkTest, SetTemperature) {
    Network net;
    net.setTemperature(0.9);
    EXPECT_DOUBLE_EQ(net.getTemperature(), 0.9);
}

TEST(NetworkTest, SetUniformalTemperature) {
    Network net;
    net.setUniformalTemperature(false);
    EXPECT_FALSE(net.getUniformalTemperature());
}

// ── Copy constructor ──────────────────────────────────────────────────────────

TEST(NetworkTest, CopyConstructorPreservesNeuronCount) {
    Network original;
    original.addNeuron(new Neuron());
    original.addNeuron(new Neuron());

    Network copy(original);
    EXPECT_EQ(copy.getNbNeurons(), 2);
}

TEST(NetworkTest, CopyConstructorIsDeep) {
    Network original;
    original.addNeuron(new Neuron());

    Network copy(original);
    EXPECT_NE(copy.getNeuron(0), original.getNeuron(0));
}

TEST(NetworkTest, CopyConstructorPreservesTemperature) {
    Network original(0.42);
    Network copy(original);
    EXPECT_DOUBLE_EQ(copy.getTemperature(), 0.42);
}
