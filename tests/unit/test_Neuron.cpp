#include <gtest/gtest.h>
#include "NeuronSynapse.h"

// ── Construction ──────────────────────────────────────────────────────────────

TEST(NeuronTest, DefaultConstructorState) {
    Neuron n;
    // init() sets state = true (1)
    EXPECT_EQ(n.getState(), 1);
}

TEST(NeuronTest, DefaultConstructorNbStates) {
    Neuron n;
    EXPECT_EQ(n.getNbStates(), 2);
}

TEST(NeuronTest, DefaultConstructorNoSynapses) {
    Neuron n;
    EXPECT_EQ(n.getNb_neighbors(), 0);
}

TEST(NeuronTest, IndexConstructor) {
    Neuron n(3);
    EXPECT_EQ(n.getIndex(), 3);
}

TEST(NeuronTest, FullConstructorSetsStateAndNbStates) {
    Neuron n(0, 0, 2, {0.5});
    EXPECT_EQ(n.getState(), 0);
    EXPECT_EQ(n.getNbStates(), 2);
}

TEST(NeuronTest, CopyConstructorPreservesState) {
    Neuron original(0, 1, 2, {0.5});
    Neuron copy(original);
    EXPECT_EQ(copy.getState(), original.getState());
    EXPECT_EQ(copy.getNbStates(), original.getNbStates());
    EXPECT_EQ(copy.getIndex(), original.getIndex());
}

// ── Getters / setters ─────────────────────────────────────────────────────────

TEST(NeuronTest, SetAndGetState) {
    Neuron n;
    n.setState(0);
    EXPECT_EQ(n.getState(), 0);
}

TEST(NeuronTest, SetAndGetIndex) {
    Neuron n;
    n.setIndex(7);
    EXPECT_EQ(n.getIndex(), 7);
}

TEST(NeuronTest, SetAndGetTemperature) {
    Neuron n;
    n.setTemperature(0.5);
    EXPECT_DOUBLE_EQ(n.getTemperature(), 0.5);
}

TEST(NeuronTest, SetAndGetNodeID) {
    Neuron n;
    n.setNodeID("A1");
    EXPECT_EQ(n.getNodeID(), "A1");
}

TEST(NeuronTest, SetNbStatesExpandsThresholds) {
    Neuron n;
    n.setNbStates(3); // adds one more threshold
    EXPECT_EQ(n.getNbStates(), 3);
}

TEST(NeuronTest, SetNbStatesTrimsThresholds) {
    Neuron n;
    n.setNbStates(3);
    n.setNbStates(2);
    EXPECT_EQ(n.getNbStates(), 2);
}

// ── Synapses ──────────────────────────────────────────────────────────────────

TEST(NeuronTest, AddSynapseIncreasesNeighborCount) {
    Neuron n0(0), n1(1);
    n0.addSynapse(&n1, 1.0);
    EXPECT_EQ(n0.getNb_neighbors(), 1);
}

TEST(NeuronTest, AddSynapseDuplicateReturnsFalse) {
    Neuron n0(0), n1(1);
    EXPECT_TRUE(n0.addSynapse(&n1, 1.0));
    EXPECT_FALSE(n0.addSynapse(&n1, 0.5)); // duplicate target
}

TEST(NeuronTest, AddSynapseWeightStored) {
    Neuron n0(0), n1(1);
    n0.addSynapse(&n1, 0.75);
    EXPECT_DOUBLE_EQ(n0.getSynapseWeight(0), 0.75);
}

TEST(NeuronTest, DelSynapseBySynapseIndex) {
    Neuron n0(0), n1(1), n2(2);
    n0.addSynapse(&n1, 1.0);
    n0.addSynapse(&n2, 0.5);
    n0.delSynapseBySynapseIndex(0);
    EXPECT_EQ(n0.getNb_neighbors(), 1);
}

TEST(NeuronTest, GetSynapseByNeighborIndex) {
    Neuron n0(0), n1(1), n2(2);
    n0.addSynapse(&n1, 1.0);
    n0.addSynapse(&n2, 0.5);
    EXPECT_EQ(n0.getSynapseByNeighborIndex(2), 1); // n2 is at synapse index 1
}

TEST(NeuronTest, GetSelfSynapseNullWhenNone) {
    Neuron n0(0), n1(1);
    n0.addSynapse(&n1, 1.0);
    EXPECT_EQ(n0.getSelfSynapse(), nullptr);
}

TEST(NeuronTest, GetSelfSynapseReturnsCorrectly) {
    Neuron n0(0);
    n0.addSynapse(&n0, 1.0); // self-loop
    EXPECT_NE(n0.getSelfSynapse(), nullptr);
}

// ── compute() at temperature=0 ────────────────────────────────────────────────
//
// Règle pour un neurone binaire (nbStates=2) :
//   sum = Σ weight_i * state(neighbor_i) − threshold[0]
//   sum >= 0  →  theNewState = 1
//   sum <  0  →  theNewState = 0
// L'état n'est appliqué qu'après substitute().

TEST(NeuronTest, ComputeNoSynapsesGoesToZero) {
    // Aucune synapse : sum = 0 - 0.0001 < 0 → 0
    Neuron n;
    n.compute(0.0);
    n.substitute();
    EXPECT_EQ(n.getState(), 0);
}

TEST(NeuronTest, ComputeExcitatoryInputKeepsActive) {
    // n0 (state=1) → n1 avec poids 1.0 ; sum = 1.0 - 0.0001 >= 0 → 1
    Neuron n0(0), n1(1);
    n0.setState(1);
    n1.addSynapse(&n0, 1.0);
    n1.compute(0.0);
    n1.substitute();
    EXPECT_EQ(n1.getState(), 1);
}

TEST(NeuronTest, ComputeInhibitoryInputSilences) {
    // n0 (state=1) → n1 avec poids -1.0 ; sum = -1.0 - 0.0001 < 0 → 0
    Neuron n0(0), n1(1);
    n0.setState(1);
    n1.addSynapse(&n0, -1.0);
    n1.compute(0.0);
    n1.substitute();
    EXPECT_EQ(n1.getState(), 0);
}

TEST(NeuronTest, ComputeInactiveSourceHasNoEffect) {
    // n0 (state=0) → n1 avec poids 1.0 ; sum = 1.0*0 - 0.0001 < 0 → 0
    Neuron n0(0), n1(1);
    n0.setState(0);
    n1.addSynapse(&n0, 1.0);
    n1.compute(0.0);
    n1.substitute();
    EXPECT_EQ(n1.getState(), 0);
}

TEST(NeuronTest, ComputeSelfExcitatoryLoopStaysActive) {
    // self-loop poids=1.0, state=1 : sum = 1.0 - 0.0001 >= 0 → 1
    Neuron n(0);
    n.setState(1);
    n.addSynapse(&n, 1.0);
    n.compute(0.0);
    n.substitute();
    EXPECT_EQ(n.getState(), 1);
}

TEST(NeuronTest, ComputeHighThresholdOverridesInput) {
    // n0(state=1) → n1, poids=1.0 mais threshold très élevé → sum < 0 → 0
    Neuron n0(0), n1(1, 0, 2, {10.0});
    n0.setState(1);
    n1.addSynapse(&n0, 1.0);
    n1.compute(0.0);
    n1.substitute();
    EXPECT_EQ(n1.getState(), 0);
}

TEST(NeuronTest, ComputeDoesNotApplyUntilSubstitute) {
    Neuron n0(0), n1(1);
    n0.setState(1);
    n1.addSynapse(&n0, 1.0);
    n1.compute(0.0);
    // Pas encore de substitute : l'état visible reste inchangé
    EXPECT_EQ(n1.getState(), 1); // état par défaut de init()
}

// ── substitute() ──────────────────────────────────────────────────────────────

TEST(NeuronTest, SubstituteWithoutComputeDoesNothing) {
    Neuron n;
    int stateBefore = n.getState();
    n.substitute();
    EXPECT_EQ(n.getState(), stateBefore);
}

TEST(NeuronTest, SubstituteAppliesComputedState) {
    Neuron n;
    n.compute(0.0);   // pas de synapses → theNewState=0
    n.substitute();
    EXPECT_EQ(n.getState(), 0);
}

TEST(NeuronTest, SubstituteCalledTwiceDoesNotReapply) {
    Neuron n0(0), n1(1);
    n0.setState(1);
    n1.addSynapse(&n0, 1.0);
    n1.compute(0.0);  // theNewState=1
    n1.substitute();  // applique : state=1
    n0.setState(0);   // change la source
    n1.substitute();  // hasANewState=false, rien ne se passe
    EXPECT_EQ(n1.getState(), 1);
}
