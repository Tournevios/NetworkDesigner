#include <gtest/gtest.h>
#include "NetworkState.h"

// Helper : crée un nbStates de taille N avec chaque neurone à 2 états possibles
static NetworkState* makeBinaryNbStates(int size) {
    auto* ns = new NetworkState(size, nullptr);
    for (int i = 0; i < size; ++i)
        ns->setState(i, 2);
    ns->setNbStatesNS(ns); // auto-référence (pattern utilisé dans le code)
    return ns;
}

// ── Construction ──────────────────────────────────────────────────────────────

TEST(NetworkStateTest, DefaultConstructorEmptySize) {
    NetworkState ns;
    EXPECT_EQ(ns.getSize(), 0);
}

TEST(NetworkStateTest, SizeConstructorSetsCorrectSize) {
    NetworkState nbS(3, nullptr);
    NetworkState ns(3, &nbS);
    EXPECT_EQ(ns.getSize(), 3);
}

TEST(NetworkStateTest, SizeConstructorInitializesStatesToMinusOne) {
    NetworkState nbS(2, nullptr);
    NetworkState ns(2, &nbS);
    EXPECT_EQ(ns.getState(0), -1);
    EXPECT_EQ(ns.getState(1), -1);
}

TEST(NetworkStateTest, CopyConstructorPreservesStates) {
    NetworkState nbS(2, nullptr);
    NetworkState original(2, &nbS);
    original.setState(0, 1);
    original.setState(1, 0);

    NetworkState copy(original);
    EXPECT_EQ(copy.getState(0), 1);
    EXPECT_EQ(copy.getState(1), 0);
    EXPECT_EQ(copy.getSize(), 2);
}

// ── Get / Set state ───────────────────────────────────────────────────────────

TEST(NetworkStateTest, SetAndGetState) {
    NetworkState nbS(3, nullptr);
    NetworkState ns(3, &nbS);
    ns.setState(0, 1);
    ns.setState(1, 0);
    ns.setState(2, 1);
    EXPECT_EQ(ns.getState(0), 1);
    EXPECT_EQ(ns.getState(1), 0);
    EXPECT_EQ(ns.getState(2), 1);
}

TEST(NetworkStateTest, GetStateOutOfBoundsReturnsMinusOne) {
    NetworkState nbS(2, nullptr);
    NetworkState ns(2, &nbS);
    EXPECT_EQ(ns.getState(99), -1);
}

// ── coherent() ────────────────────────────────────────────────────────────────

TEST(NetworkStateTest, CoherentReturnsFalseWhenAnyStateIsMinusOne) {
    NetworkState nbS(2, nullptr);
    NetworkState ns(2, &nbS);
    ns.setState(0, 1);
    // ns.setState(1) stays -1
    EXPECT_FALSE(ns.coherent());
}

TEST(NetworkStateTest, CoherentReturnsTrueWhenAllStatesSet) {
    NetworkState nbS(2, nullptr);
    NetworkState ns(2, &nbS);
    ns.setState(0, 1);
    ns.setState(1, 0);
    EXPECT_TRUE(ns.coherent());
}

// ── initialize() ─────────────────────────────────────────────────────────────

TEST(NetworkStateTest, InitializeResetsVisitedAndAttractorFlags) {
    NetworkState nbS(1, nullptr);
    NetworkState ns(1, &nbS);
    ns.setVisited(true);
    ns.setIsAttractor(true);
    ns.setAttractorNumber(5);
    ns.setNextOne(3);

    ns.initialize();

    EXPECT_FALSE(ns.isVisited());
    EXPECT_FALSE(ns.isIsAttractor());
    EXPECT_EQ(ns.getAttractorNumber(), -1);
    EXPECT_EQ(ns.getNextOne(), -1);
}

// ── operator== ───────────────────────────────────────────────────────────────

TEST(NetworkStateTest, EqualityTrueForIdenticalStates) {
    NetworkState nbS(2, nullptr);
    NetworkState a(2, &nbS);
    a.setState(0, 1); a.setState(1, 0);
    NetworkState b(a);
    EXPECT_TRUE(a == b);
}

TEST(NetworkStateTest, EqualityFalseForDifferentConcreteStates) {
    NetworkState nbS(2, nullptr);
    NetworkState a(2, &nbS);
    a.setState(0, 1); a.setState(1, 0);
    NetworkState b(2, &nbS);
    b.setState(0, 0); b.setState(1, 1);
    EXPECT_FALSE(a == b);
}

// ── operator= ────────────────────────────────────────────────────────────────

TEST(NetworkStateTest, AssignmentCopiesStates) {
    NetworkState nbS(2, nullptr);
    NetworkState a(2, &nbS);
    a.setState(0, 1); a.setState(1, 0);
    NetworkState b;
    b = a;
    EXPECT_EQ(b.getState(0), 1);
    EXPECT_EQ(b.getState(1), 0);
}

// ── operator< and operator> ──────────────────────────────────────────────────

TEST(NetworkStateTest, LessThanTrueWhenConcreteIsSubsetOfSet) {
    NetworkState nbS(2, nullptr);
    // a = concrete state {1, 0}
    NetworkState a(2, &nbS);
    a.setState(0, 1); a.setState(1, 0);
    // b = set state that encodes both {1,0} and {0,0} → state[0] = -(2^1 | 2^0) = -3, state[1] = 0
    NetworkState b(2, &nbS);
    b.setState(0, -3); b.setState(1, 0);
    EXPECT_TRUE(a < b);
}

TEST(NetworkStateTest, GreaterThanIsReverseLessThan) {
    NetworkState nbS(2, nullptr);
    NetworkState a(2, &nbS);
    a.setState(0, 1); a.setState(1, 0);
    NetworkState b(2, &nbS);
    b.setState(0, -3); b.setState(1, 0);
    EXPECT_TRUE(b > a);
}

// ── visited / attractor flags ─────────────────────────────────────────────────

TEST(NetworkStateTest, VisitedDefaultFalse) {
    NetworkState nbS(1, nullptr);
    NetworkState ns(1, &nbS);
    EXPECT_FALSE(ns.isVisited());
}

TEST(NetworkStateTest, SetVisited) {
    NetworkState nbS(1, nullptr);
    NetworkState ns(1, &nbS);
    ns.setVisited(true);
    EXPECT_TRUE(ns.isVisited());
}

TEST(NetworkStateTest, IsAttractorDefaultFalse) {
    NetworkState nbS(1, nullptr);
    NetworkState ns(1, &nbS);
    EXPECT_FALSE(ns.isIsAttractor());
}

TEST(NetworkStateTest, SetIsAttractor) {
    NetworkState nbS(1, nullptr);
    NetworkState ns(1, &nbS);
    ns.setIsAttractor(true);
    EXPECT_TRUE(ns.isIsAttractor());
}

TEST(NetworkStateTest, AttractorNumberDefaultMinusOne) {
    NetworkState nbS(1, nullptr);
    NetworkState ns(1, &nbS);
    EXPECT_EQ(ns.getAttractorNumber(), -1);
}

TEST(NetworkStateTest, SetAttractorNumber) {
    NetworkState nbS(1, nullptr);
    NetworkState ns(1, &nbS);
    ns.setAttractorNumber(3);
    EXPECT_EQ(ns.getAttractorNumber(), 3);
}
