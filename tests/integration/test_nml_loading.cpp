// Integration tests: load every example NML file via NetworkDesignerParser
// and verify that the resulting Network has the expected neuron/synapse counts.
#include <gtest/gtest.h>
#include <QApplication>
#include "Network.h"
#include "NetworkDesignerParser.h"
#include "UpdateSchedulingPlan.h"
#include <string>
#include <cstdlib>

static std::string examplesDir() {
    const char* env = std::getenv("EXAMPLES_DIR");
    return env ? std::string(env) : std::string(EXAMPLES_DIR_DEFAULT);
}

struct NmlCase {
    const char* file;
    int neurons;
    int synapses;
};

static const NmlCase CASES[] = {
    { "3X3OscilloX8.nml",          48, 144 },
    { "Arabidopsis_premodel.nml",   12,  24 },
    { "Arabiodopsis_thalina.nml",   12,  25 },
    { "CardioRespiratoire.nml",      4,   6 },
    { "EukarioteCell.nml",          12,  15 },
    { "HeartBeat.nml",              10,  42 },
    { "TheBigOne.nml",              10,  54 },
    { "allPositive_And_Cycle.nml",   3,   8 },
    { "anotherCoolNetwork.nml",      8,   9 },
    { "doubleOSCILLO.nml",           4,   7 },
    { "eukarioteCells.nml",         12,  15 },
    { "grilleWithoutBorder.nml",    48, 204 },
    { "peaceMakerGrid.nml",         25,  40 },
};

class NmlLoadTest : public ::testing::TestWithParam<NmlCase> {};

TEST_P(NmlLoadTest, LoadsCorrectNeuronCount) {
    const NmlCase& c = GetParam();
    std::string path = examplesDir() + "/" + c.file;

    NetworkDesignerParser parser;
    parser.load(QString::fromStdString(path));
    Network* network = parser.getNetwork();
    ASSERT_NE(network, nullptr) << "File: " << c.file;

    EXPECT_EQ(network->getNbNeurons(), c.neurons)
        << "File: " << c.file;
}

TEST_P(NmlLoadTest, LoadsCorrectSynapseCount) {
    const NmlCase& c = GetParam();
    std::string path = examplesDir() + "/" + c.file;

    NetworkDesignerParser parser;
    parser.load(QString::fromStdString(path));
    Network* network = parser.getNetwork();
    ASSERT_NE(network, nullptr) << "File: " << c.file;

    int totalSynapses = 0;
    for (int i = 0; i < network->getNbNeurons(); ++i)
        totalSynapses += network->getNeuron(i)->getNb_neighbors();
    EXPECT_EQ(totalSynapses, c.synapses)
        << "File: " << c.file;
}

INSTANTIATE_TEST_SUITE_P(
    Examples, NmlLoadTest,
    ::testing::ValuesIn(CASES),
    [](const ::testing::TestParamInfo<NmlCase>& info) {
        std::string name = info.param.file;
        for (char& ch : name)
            if (!std::isalnum(static_cast<unsigned char>(ch))) ch = '_';
        return name;
    });

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
