#include "Network.h"
#include "constFile.h"
#include "random-singleton.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Network::Network(double temperature)
    : temperature(temperature)
    , uniformalTemperature(true)
{
    Random::Uniform<double>(0.0, 1.0);
}

Network::Network()
    : temperature(0.001)
    , uniformalTemperature(true)
{
    Random::Uniform<double>(0.0, 1.0);
}

Network::Network(const Network& other)
    : temperature(other.temperature)
    , uniformalTemperature(other.uniformalTemperature)
{
    Random::Uniform<double>(0.0, 1.0);

    for (int i = 0; i < other.getNbNeurons(); ++i)
        addNeuron(new Neuron(*other.getNeuron(i)));

    for (int i = 0; i < getNbNeurons(); ++i) {
        for (int j = 0; j < other.getNeuron(i)->getNb_neighbors(); ++j) {
            auto* syn = other.getNeuron(i)->getSynapse(j);
            getNeuron(i)->addSynapse(
                getNeuron(syn->getFinalNeuron()->getIndex()),
                other.getNeuron(i)->getSynapseWeight(j),
                other.getNeuron(i)->getSynapseDelay(j));
            getNeuron(i)->getSynapse(j)->setCX(syn->getCX());
            getNeuron(i)->getSynapse(j)->setCY(syn->getCY());
        }
    }
}

Network::~Network() = default;

void Network::addNetwork(Network* other) {
    const int offset = getNbNeurons();
    deselectAll();

    for (int i = 0; i < other->getNbNeurons(); ++i) {
        addNeuron(new Neuron(*other->getNeuron(i)));
        getNeuron(offset + i)->setSelected(true);
    }

    for (int i = 0; i < other->getNbNeurons(); ++i) {
        for (int j = 0; j < other->getNeuron(i)->getNb_neighbors(); ++j) {
            auto* syn = other->getNeuron(i)->getSynapse(j);
            getNeuron(offset + i)->addSynapse(
                getNeuron(syn->getFinalNeuron()->getIndex() + offset),
                other->getNeuron(i)->getSynapseWeight(j),
                other->getNeuron(i)->getSynapseDelay(j));
            getNeuron(offset + i)->getSynapse(j)->setCX(syn->getCX());
            getNeuron(offset + i)->getSynapse(j)->setCY(syn->getCY());
        }
    }
}

// ── Getters / setters ─────────────────────────────────────────────────────────

int  Network::getNbNeurons() const                       { return static_cast<int>(neurons.size()); }
void Network::setTemperature(double t)                   { temperature = t; }
double Network::getTemperature() const                   { return temperature; }
void Network::setUniformalTemperature(bool u)            { uniformalTemperature = u; }
bool Network::getUniformalTemperature() const            { return uniformalTemperature; }

// ── Neuron access ─────────────────────────────────────────────────────────────

Neuron* Network::getNeuron(int index) const {
    if (index >= 0 && index < getNbNeurons())
        return neurons[index].get();
    return nullptr;
}

Neuron* Network::getNeuron(int x, int y, int radius) const {
    auto it = std::find_if(neurons.begin(), neurons.end(), [=](const auto& n) {
        return std::abs(n->getX() - x) <= radius && std::abs(n->getY() - y) <= radius;
    });
    return it != neurons.end() ? it->get() : nullptr;
}

Synapse* Network::getSynapse(int x, int y) const {
    for (const auto& n : neurons)
        for (int j = 0; j < n->getNb_neighbors(); ++j)
            if (n->getSynapse(j)->selectable(x, y))
                return n->getSynapse(j);
    return nullptr;
}

void Network::addNeuron(Neuron* neuron) {
    neuron->setIndex(getNbNeurons());
    neurons.push_back(std::unique_ptr<Neuron>(neuron));
}

void Network::delNeuron(int index) {
    if (!getNeuron(index)) return;

    // Remove all synapses pointing to this neuron
    for (int i = 0; i < getNbNeurons(); ++i) {
        int si = getNeuron(i)->getSynapseByNeighborIndex(index);
        if (si < getNeuron(i)->getNb_neighbors()) {
            getNeuron(i)->delSynapseBySynapseIndex(si);
            --i;
        }
    }

    // Shift indices of neurons that come after
    for (int i = index + 1; i < getNbNeurons(); ++i)
        getNeuron(i)->setIndex(i - 1);

    neurons.erase(neurons.begin() + index);
}

// ── State / draw ──────────────────────────────────────────────────────────────

void Network::deselectAll() {
    for (const auto& n : neurons) {
        n->setSelected(false);
        for (int j = 0; j < n->getNb_neighbors(); ++j)
            n->getSynapse(j)->setSelected(false);
    }
}

int Network::getState(bool* states) const {
    int i = 0;
    for (const auto& n : neurons) states[i++] = n->getState();
    return i;
}

void Network::drawMe(QPainter* painter, float scale, int transX, int transY) {
    for (const auto& n : neurons) n->drawSynapses(painter, scale, transX, transY);
    for (const auto& n : neurons) n->drawMe(painter, scale, transX, transY);
}
