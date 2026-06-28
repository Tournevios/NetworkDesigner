#include "Computer.h"
#include <algorithm>

Computer::Computer() = default;
Computer::~Computer() = default;

Computer::Computer(const Computer& computer)
    : network(new Network(*computer.getNetwork()))
    , updateSchedulingPlan(new UpdateSchedulingPlan(*computer.getUpdateSchedulingPlan()))
    , nb_iterations(computer.getNbIterations())
    , ui(computer.getUi())
    , repaintDesign(false)
{
    Random::Uniform<double>(0.0, 1.0);
    connect(this, SIGNAL(setProgressBarValue_Signal(int)), ui->progressBar, SLOT(setValue(int)));
}

Computer::Computer(Network* network, UpdateSchedulingPlan* updateSchedulingPlan, Ui::MainWindow* ui)
    : network(network)
    , updateSchedulingPlan(updateSchedulingPlan)
    , nb_iterations(ui->sbUpdatesNumber->value())
    , ui(ui)
    , repaintDesign(true)
{
    Random::Uniform<double>(0.0, 1.0);
    connect(this, SIGNAL(setProgressBarValue_Signal(int)), ui->progressBar, SLOT(setValue(int)));
}

Ui::MainWindow* Computer::getUi() const { return ui; }

void Computer::setNetwork(Network* n)                               { network = n; }
Network* Computer::getNetwork() const                               { return network; }
void Computer::setUpdateSchedulingPlan(UpdateSchedulingPlan* usp)  { updateSchedulingPlan = usp; }
UpdateSchedulingPlan* Computer::getUpdateSchedulingPlan() const    { return updateSchedulingPlan; }
int Computer::getNbIterations() const                               { return nb_iterations; }
void Computer::setNbIterations(int n)                               { nb_iterations = n; }
void Computer::stopComputing()                                      { stop = true; }

// ── helpers ──────────────────────────────────────────────────────────────────

void Computer::setProgressBarValue(int value) {
    emit setProgressBarValue_Signal(value);
}

void Computer::setStateOfTheNetwork(NetworkState* networkState) {
    for (int i = 0; i < network->getNbNeurons(); ++i)
        network->getNeuron(i)->setState(networkState->getState(i));
}

// Returns the temperature to use for neuron j.
double Computer::neuronTemperature(int j) const {
    return network->getUniformalTemperature()
        ? network->getTemperature()
        : network->getNeuron(j)->getTemperature();
}

// Repaint + throttle when speed < 100%.
void Computer::maybeRepaint() {
    if (!repaintDesign) return;
    if (ui->sbSpeedPercent->value() != 100) {
        ui->frmDesign->repaint();
        usleep(100'000 - ui->sbSpeedPercent->value() * 1'000);
    }
}

// Apply synchrony-gated compute+substitute for a single neuron (sequential mode).
void Computer::computeAndSubstitute(int j, double syncRate) {
    if (Random::Uniform() <= syncRate) {
        network->getNeuron(j)->compute(neuronTemperature(j));
        network->getNeuron(j)->substitute();
    }
}

// ── Parallel update ───────────────────────────────────────────────────────────

void Computer::computeP() {
    stop = false;
    const double syncRate = ui->dsbSynchronyRate->value();
    const int n = network->getNbNeurons();

    for (int iter = 0; iter < nb_iterations; ++iter) {
        if (stop) return;

        for (int j = 0; j < n; ++j)
            if (Random::Uniform() <= syncRate)
                network->getNeuron(j)->compute(neuronTemperature(j));

        for (int j = 0; j < n; ++j)
            network->getNeuron(j)->substitute();

        maybeRepaint();
        emit tick();
    }
    if (repaintDesign) ui->frmDesign->repaint();
}

// ── Sequential update ─────────────────────────────────────────────────────────

void Computer::computeS() {
    stop = false;
    const double syncRate = ui->dsbSynchronyRate->value();
    const int n = network->getNbNeurons();

    for (int iter = 0; iter < nb_iterations; ++iter) {
        if (stop) return;

        for (int j = 0; j < n; ++j) {
            computeAndSubstitute(j, syncRate);
            maybeRepaint();
        }
        emit tick();
    }
    if (repaintDesign) ui->frmDesign->repaint();
}

// ── Block Sequential update ───────────────────────────────────────────────────

void Computer::applyBlockMethod(UpdateBlock* ub, double syncRate, int iterIndex) {
    const int method = ub->getUpdateMethods();
    const int sz     = ub->getSize();

    auto neuronAt = [&](int j) { return network->getNeuron(ub->getNeuronIndex(j)); };
    auto gated    = [&](int j, auto action) {
        if (Random::Uniform() <= syncRate) action(neuronAt(j));
    };

    switch (method) {
    case COMPUTE:
        for (int j = 0; j < sz; ++j)
            gated(j, [&](Neuron* n) { n->compute(neuronTemperature(ub->getNeuronIndex(j))); });
        for (int j = 0; j < sz; ++j)
            neuronAt(j)->substitute();
        break;

    case FIXE_1:
        for (int j = 0; j < sz; ++j)
            gated(j, [](Neuron* n) { n->setState(true); });
        break;

    case FIXE_0:
        for (int j = 0; j < sz; ++j)
            gated(j, [](Neuron* n) { n->setState(false); });
        break;

    case FIXE_01:
        for (int j = 0; j < sz; ++j)
            gated(j, [j](Neuron* n) { n->setState(static_cast<bool>(j % 2)); });
        break;

    case FIXE_10:
        for (int j = 0; j < sz; ++j)
            gated(j, [j](Neuron* n) { n->setState(static_cast<bool>((j + 1) % 2)); });
        break;

    case RANDOMLY:
        for (int j = 0; j < sz; ++j)
            gated(j, [](Neuron* n) { n->setState(Random::Uniform() >= 0.5); });
        break;

    default:
        break;
    }
}

void Computer::computeBS() {
    stop = false;
    const double syncRate  = ui->dsbSynchronyRate->value();
    const int    nb_blocks = updateSchedulingPlan->getNb_blocks();

    for (int iter = 0; iter < nb_iterations; ++iter) {
        if (stop) return;

        for (int i = 0; i < nb_blocks; ++i) {
            applyBlockMethod(updateSchedulingPlan->getUpdateBlock(i), syncRate, iter);
            emit tick();
        }
        maybeRepaint();
    }
    if (repaintDesign) ui->frmDesign->repaint();
}

// ── Block Parallel update ─────────────────────────────────────────────────────

void Computer::computeBP() {
    stop = false;
    const double syncRate  = ui->dsbSynchronyRate->value();
    const int    nb_blocks = updateSchedulingPlan->getNb_blocks();

    network->setUniformalTemperature(true);

    for (int iter = 0; iter < nb_iterations; ++iter) {
        if (stop) return;

        // Compute phase (all blocks)
        for (int j = 0; j < nb_blocks; ++j) {
            auto* ub  = updateSchedulingPlan->getUpdateBlock(j);
            int   idx = iter % ub->getSize();
            if (Random::Uniform() <= syncRate)
                applyBlockMethod(ub, syncRate, iter);
        }

        // Substitute phase (all blocks)
        for (int j = 0; j < nb_blocks; ++j) {
            auto* ub = updateSchedulingPlan->getUpdateBlock(j);
            network->getNeuron(ub->getNeuronIndex(iter % ub->getSize()))->substitute();
        }

        maybeRepaint();
        emit tick();
    }
    if (repaintDesign) ui->frmDesign->repaint();
}
