# NetworkDesigner — User Guide

## Overview

NetworkDesigner is a desktop application for designing, visualising, and simulating **Boolean and multi-valued neural networks** (gene regulatory networks, biological oscillators, etc.).  
You draw networks graphically, assign weights and thresholds, choose an update schedule, and then run simulations to study dynamics, attractors, and basins of attraction.

---

## Installation

### Requirements

| Dependency | Version |
|-----------|---------|
| Qt        | 5.x (5.9 – 5.15) |
| CMake     | 3.16+ |
| C++ compiler | C++17 (GCC 9+ / Clang 9+) |
| Ninja (optional) | any |

### Building from source

```bash
# 1. Clone
git clone https://github.com/tournevios/networkdesigner.git
cd networkdesigner

# 2. Configure
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# 3. Build
cmake --build build

# 4. Run
./build/src/NetworkDesigner
```

To also build and run the test suite:

```bash
cmake -B build -G Ninja -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## Quick-start

### Opening an example

1. Launch NetworkDesigner.
2. Go to **File → Open** (or Ctrl+O).
3. Navigate to the `examples/` folder and open any `.nml` file (e.g., `doubleOSCILLO.nml`).
4. The network is drawn on the canvas.

### Creating a new network

1. **File → New** (Ctrl+N) creates a blank network.
2. **Left-click** on an empty area of the canvas to add a neuron.
3. **Right-click** a neuron and drag to another neuron to create a synapse.
4. Select a neuron to edit its properties in the left panel (state, threshold, temperature, NbStates).
5. Select a synapse to edit its weight in the left panel.

### Zooming and panning

| Action | Effect |
|--------|--------|
| Mouse wheel | Zoom in / out |
| `+` / `−` toolbar buttons | Zoom in / out |
| `1:1` toolbar button | Reset zoom |
| Middle-click drag | Pan canvas |

---

## Neuron properties

| Property | Description |
|----------|-------------|
| **State** | Current activation value (0 … NbStates−1) |
| **NbStates** | Number of discrete states (2 = Boolean, 3+ = multi-valued) |
| **Threshold** | Per-state-boundary threshold values; controls the activation function |
| **Temperature** | Stochastic noise parameter (0 = deterministic) |
| **Update method** | `COMPUTE` (normal), `FIXE`, `FIXE_0`, `FIXE_1`, `FIXE_01`, `FIXE_10`, `RANDOMLY` |

### Synapse properties

| Property | Description |
|----------|-------------|
| **Weight** | Synaptic strength (positive = excitatory, negative = inhibitory) |
| **Delay** | Number of time steps before the signal arrives |

---

## Update scheduling

The **Schedule** tab in the left panel controls how neurons are updated each time step.

| Schedule type | Meaning |
|--------------|---------|
| **P** — Parallel | All neurons updated simultaneously |
| **BP** — Block Parallel | Blocks updated in parallel; neurons within a block updated simultaneously |
| **BS** — Block Sequential | Blocks updated sequentially; neurons within a block updated simultaneously |
| **S** — Sequential | Neurons updated one at a time in order |

### Creating blocks

1. Select a schedule type that uses blocks (BP or BS).
2. Click **Add a new block…** in the schedule drop-down.
3. Click neurons on the canvas while a block is selected to assign them to it.

---

## Simulation types

Select the simulation from the **Simulation** tab and click **Start**.

| Type | Description |
|------|-------------|
| **Activity** | Tracks the fraction of active neurons over time; exports a `.dat` file |
| **Changement** | Counts how many neurons change state per step |
| **Diffusion** | Measures state diffusion through the network |
| **Attractors & Basins** | Finds all attractors and their basins of attraction (exhaustive) |
| **Attractors & Basins 2** | Faster probabilistic attractor finder (parallel threads) |

### Temperature

- **Temperature = 0** (default): deterministic dynamics.  
- **Temperature > 0**: stochastic dynamics — neurons can flip according to a Boltzmann distribution.

Set a global temperature with the **Temperature** spin-box, or enable **Uniform temperature** to apply it to all neurons.

---

## File format (NML)

Networks are saved as `.nml` files — XML with the following structure:

```xml
<Configuration>
  <Network Nb_Neurons="4" Temperature="0" UniformalTemperature="1">
    <Neuron Index="0" State="1" NbStates="2" Threshold_0="0.0001" cx="100" cy="200" ...>
      <Synapse FinalNeuronIndex="1" Weight="1.0" Delay="0"/>
    </Neuron>
    ...
  </Network>
  <UpdateSchedulingPlan Nb_Blocks="0"/>
</Configuration>
```

---

## Example files

| File | Neurons | Synapses | Description |
|------|---------|----------|-------------|
| `doubleOSCILLO.nml` | 4 | 7 | Two coupled oscillators |
| `3X3OscilloX8.nml` | 48 | 144 | 3×3 oscillator grid ×8 |
| `allPositive_And_Cycle.nml` | 3 | 8 | Simple positive-feedback cycle |
| `anotherCoolNetwork.nml` | 8 | 9 | General demo network |
| `HeartBeat.nml` | 10 | 42 | Cardiac rhythm model |
| `CardioRespiratoire.nml` | 4 | 6 | Cardio-respiratory model |
| `Arabidopsis_premodel.nml` | 12 | 24 | Arabidopsis thaliana premodel |
| `Arabiodopsis_thalina.nml` | 12 | 25 | Arabidopsis thaliana full |
| `EukarioteCell.nml` | 12 | 15 | Eukaryote cell cycle |
| `eukarioteCells.nml` | 12 | 15 | Eukaryote cells variant |
| `TheBigOne.nml` | 10 | 54 | Dense regulatory network |
| `grilleWithoutBorder.nml` | 48 | 204 | Grid network (no borders) |
| `peaceMakerGrid.nml` | 25 | 40 | 5×5 peacemaker grid |

---

## Keyboard shortcuts

| Shortcut | Action |
|----------|--------|
| Ctrl+N | New network |
| Ctrl+O | Open file |
| Ctrl+S | Save file |
| Ctrl+Z | Undo (if available) |
| `+` | Zoom in |
| `-` | Zoom out |
| Ctrl+0 | Reset zoom (1:1) |
| Delete | Delete selected neuron or synapse |

---

## Troubleshooting

**Canvas is blank after opening a file**  
Ensure the `.nml` file is well-formed XML. Try opening one of the bundled examples first.

**Simulation does not stop**  
Click the **Stop** button. For the exhaustive attractor finder, computation time grows exponentially with the number of neurons.

**Application crashes on large networks**  
The exhaustive attractor search (`Attractors & Basins`) enumerates all 2^N states. For N > 20 this requires large amounts of memory. Use `Attractors & Basins 2` for large networks.
