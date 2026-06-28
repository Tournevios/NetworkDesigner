#include <gtest/gtest.h>
#include "NetworkStatesSet.h"

// ── Helpers ───────────────────────────────────────────────────────────────────

// nbStates : chaque neurone a 2 états possibles
static NetworkState makeBinaryNbS(int size) {
    NetworkState nbS(size, nullptr);
    for (int i = 0; i < size; ++i) nbS.setState(i, 2);
    return nbS;
}

static NetworkState makeConcreteState(int size, long int val, NetworkState* nbS) {
    NetworkState ns(size, nbS);
    for (int i = 0; i < size; ++i) ns.setState(i, val);
    return ns;
}

// ── Construction ──────────────────────────────────────────────────────────────

TEST(NetworkStatesSetTest, DefaultConstructorEmptyCardinal) {
    NetworkStatesSet nss;
    EXPECT_EQ(nss.getCardinal(), 0);
}

TEST(NetworkStatesSetTest, DefaultMaxCardinalIsMinusOne) {
    NetworkStatesSet nss;
    EXPECT_EQ(nss.getMaxCardinal(), -1);
}

TEST(NetworkStatesSetTest, DefaultFilledIsFalse) {
    NetworkStatesSet nss;
    EXPECT_FALSE(nss.getFilled());
}

TEST(NetworkStatesSetTest, MaxCardinalConstructor) {
    NetworkStatesSet nss(5);
    EXPECT_EQ(nss.getMaxCardinal(), 5);
    EXPECT_EQ(nss.getCardinal(), 0);
}

// ── addNetworkState / getCardinal / getNetworkState ───────────────────────────

TEST(NetworkStatesSetTest, AddNetworkStateIncreasesCardinal) {
    NetworkStatesSet nss;
    NetworkState nbS = makeBinaryNbS(2);
    NetworkState s = makeConcreteState(2, 1, &nbS);
    nss.addNetworkState(s);
    EXPECT_EQ(nss.getCardinal(), 1);
}

TEST(NetworkStatesSetTest, AddMultipleStates) {
    NetworkStatesSet nss;
    NetworkState nbS = makeBinaryNbS(2);
    nss.addNetworkState(makeConcreteState(2, 0, &nbS));
    nss.addNetworkState(makeConcreteState(2, 1, &nbS));
    nss.addNetworkState(makeConcreteState(2, 0, &nbS));
    EXPECT_EQ(nss.getCardinal(), 3);
}

TEST(NetworkStatesSetTest, GetNetworkStateReturnsCorrectState) {
    NetworkStatesSet nss;
    NetworkState nbS = makeBinaryNbS(2);
    NetworkState s0 = makeConcreteState(2, 0, &nbS);
    NetworkState s1 = makeConcreteState(2, 1, &nbS);
    nss.addNetworkState(s0);
    nss.addNetworkState(s1);
    EXPECT_EQ(nss.getNetworkState(0)->getState(0), 0);
    EXPECT_EQ(nss.getNetworkState(1)->getState(0), 1);
}

TEST(NetworkStatesSetTest, GetNetworkStateOutOfBoundsReturnsNull) {
    NetworkStatesSet nss;
    EXPECT_EQ(nss.getNetworkState(0), nullptr);
}

// ── removeNetworkState ────────────────────────────────────────────────────────

TEST(NetworkStatesSetTest, RemoveNetworkStateDecreasesCardinal) {
    NetworkStatesSet nss;
    NetworkState nbS = makeBinaryNbS(2);
    nss.addNetworkState(makeConcreteState(2, 0, &nbS));
    nss.addNetworkState(makeConcreteState(2, 1, &nbS));
    nss.removeNetworkState(0);
    EXPECT_EQ(nss.getCardinal(), 1);
}

TEST(NetworkStatesSetTest, RemoveNetworkStateOutOfBoundsDoesNothing) {
    NetworkStatesSet nss;
    NetworkState nbS = makeBinaryNbS(2);
    nss.addNetworkState(makeConcreteState(2, 0, &nbS));
    nss.removeNetworkState(99);
    EXPECT_EQ(nss.getCardinal(), 1);
}

// ── maxCardinal enforcement ───────────────────────────────────────────────────

TEST(NetworkStatesSetTest, MaxCardinalEvictsOldestWhenFull) {
    NetworkStatesSet nss(2);
    NetworkState nbS = makeBinaryNbS(1);
    NetworkState s0 = makeConcreteState(1, 0, &nbS);
    NetworkState s1 = makeConcreteState(1, 1, &nbS);
    NetworkState s2 = makeConcreteState(1, 0, &nbS); // triggers eviction of s0
    nss.addNetworkState(s0);
    nss.addNetworkState(s1);
    nss.addNetworkState(s2); // now: [s1, s2], s0 evicted
    EXPECT_EQ(nss.getCardinal(), 2);
    EXPECT_EQ(nss.getNetworkState(0)->getState(0), 1); // s1 is now first
}

TEST(NetworkStatesSetTest, SetMaxCardinalTruncatesExistingSet) {
    NetworkStatesSet nss;
    NetworkState nbS = makeBinaryNbS(1);
    nss.addNetworkState(makeConcreteState(1, 0, &nbS));
    nss.addNetworkState(makeConcreteState(1, 1, &nbS));
    nss.addNetworkState(makeConcreteState(1, 0, &nbS));
    nss.setMaxCardinal(1);
    EXPECT_EQ(nss.getCardinal(), 1);
}

// ── setFilled / getFilled ─────────────────────────────────────────────────────

TEST(NetworkStatesSetTest, SetFilled) {
    NetworkStatesSet nss;
    nss.setFilled(true);
    EXPECT_TRUE(nss.getFilled());
}

// ── coherent() ────────────────────────────────────────────────────────────────

TEST(NetworkStatesSetTest, CoherentTrueWhenAllStatesCoherent) {
    NetworkStatesSet nss;
    NetworkState nbS = makeBinaryNbS(2);
    nss.addNetworkState(makeConcreteState(2, 1, &nbS));
    nss.addNetworkState(makeConcreteState(2, 0, &nbS));
    EXPECT_TRUE(nss.coherent());
}

TEST(NetworkStatesSetTest, CoherentFalseWhenAStateHasMinusOne) {
    NetworkStatesSet nss;
    NetworkState nbS = makeBinaryNbS(2);
    // state left with default -1 values
    nss.addNetworkState(NetworkState(2, &nbS));
    EXPECT_FALSE(nss.coherent());
}

// ── Copy constructor ──────────────────────────────────────────────────────────

TEST(NetworkStatesSetTest, CopyConstructorPreservesCardinal) {
    NetworkStatesSet nss;
    NetworkState nbS = makeBinaryNbS(2);
    nss.addNetworkState(makeConcreteState(2, 0, &nbS));
    nss.addNetworkState(makeConcreteState(2, 1, &nbS));

    NetworkStatesSet copy(nss);
    EXPECT_EQ(copy.getCardinal(), 2);
}

TEST(NetworkStatesSetTest, CopyConstructorIsDeep) {
    NetworkStatesSet nss;
    NetworkState nbS = makeBinaryNbS(1);
    nss.addNetworkState(makeConcreteState(1, 0, &nbS));

    NetworkStatesSet copy(nss);
    EXPECT_NE(copy.getNetworkState(0), nss.getNetworkState(0));
}

// ── operator= ────────────────────────────────────────────────────────────────

TEST(NetworkStatesSetTest, AssignmentCopiesContent) {
    NetworkStatesSet a;
    NetworkState nbS = makeBinaryNbS(1);
    a.addNetworkState(makeConcreteState(1, 1, &nbS));

    NetworkStatesSet b;
    b = a;
    EXPECT_EQ(b.getCardinal(), 1);
    EXPECT_EQ(b.getNetworkState(0)->getState(0), 1);
}

TEST(NetworkStatesSetTest, SelfAssignmentIsSafe) {
    NetworkStatesSet nss;
    NetworkState nbS = makeBinaryNbS(1);
    nss.addNetworkState(makeConcreteState(1, 0, &nbS));
    nss = nss;
    EXPECT_EQ(nss.getCardinal(), 1);
}

// ── addNetworkStatesSet ───────────────────────────────────────────────────────

TEST(NetworkStatesSetTest, AddNetworkStatesSetMergesAll) {
    NetworkState nbS = makeBinaryNbS(1);
    NetworkStatesSet a;
    a.addNetworkState(makeConcreteState(1, 0, &nbS));

    NetworkStatesSet b;
    b.addNetworkState(makeConcreteState(1, 1, &nbS));
    b.addNetworkState(makeConcreteState(1, 0, &nbS));

    a.addNetworkStatesSet(b);
    EXPECT_EQ(a.getCardinal(), 3);
}
