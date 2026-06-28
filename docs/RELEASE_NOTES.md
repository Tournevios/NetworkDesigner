# Release Notes

## v1.0.0 — 2026-06-28

First stable release of **NetworkDesigner** — a Qt5/C++17 desktop application for designing and simulating Boolean and multi-valued neural networks.

### Highlights

- Full C++17 modernisation of the entire codebase (`constexpr`, `nullptr`, `static_cast`, `std::unique_ptr`, `#pragma once`)
- Qt5 PMF (Pointer-to-Member-Function) signal/slot syntax throughout the UI layer
- 100 unit tests (Google Test) covering neurons, synapses, network states, state sets, and simulation primitives — all passing
- 26 integration tests that load all 13 bundled example `.nml` files and verify neuron and synapse counts
- GitHub Actions CI on Ubuntu (Qt5 + GTest + Ninja)

### What's new in this release

#### Core (src/core/)

| Area | Change |
|------|--------|
| `NeuronSynapse.h` | Added `#include <QtGui/QPainterPath>`; `setNodeID(char*)` → `setNodeID(const char*)` |
| `Network.h` | `std::vector<Neuron*>` → `std::vector<std::unique_ptr<Neuron>>` |
| `Neuron.cpp` | `NULL` → `nullptr`; C-style casts → `static_cast` |
| `Synapse.cpp` | `NULL` → `nullptr`; C-style casts → `static_cast` |
| `NetworkState.cpp` | `NULL` → `nullptr`; `nbStates = NULL` → `nullptr`; C-style casts |
| `NetworkStatesSet.cpp` / `SetOfNetworkStatesSet.cpp` | `return NULL` → `return nullptr` |
| `SimulationActivity.cpp` | `ofstream` → `std::ofstream` |
| `SimulationAttractorsAndBasinsOfAttraction.h` | `static const double` → `static constexpr double` |
| `SimulationAttractorsAndBasinsOfAttraction.cpp` | Added `#include <vector>`; `vector<>` → `std::vector<>`; fixed parenthesis bug in `pow()` cast |
| `SimulationAttractorsAndBasinsOfAttraction2.cpp` | `NULL` → `nullptr`; `(int)pow(2.0,j)` → `static_cast<int>(pow(2.0,j))` |
| `pCalculateTransitions.cpp` | Unscoped `P`/`BP`/`BS`/`S` → `UpdateType::P` etc. |
| `constFile.h` | `#define` constants → `constexpr int/double` |
| `random-singleton.cpp` | Removed `using namespace std` |
| `BlockSelector.h` | `#include<QtGui/QWidget>` → `#include<QtWidgets/QWidget>` |
| `UpdateSchedulingPlan.cpp` | Fixed broken erase: C-style iterator cast → `sequentialblocks.begin() + index` |

#### App layer (src/app/)

| Area | Change |
|------|--------|
| `networkdesigner.h/.cpp` | `#include <memory>`; `unique_ptr<EvenementHandler>` and `unique_ptr<SignalsSlotsConnector>`; Qt PMF connects; `getScale`/`setScale` toolbar zoom; status-bar helpers |
| `EvenementHandler.h/.cpp` | `unique_ptr<Network> aSimpleCopy`; `unique_ptr<Computer>`; `unique_ptr<Simulation*>` variants; `sprintf` → `QString::arg`; `stack<>` / `vector<>` → `std::stack<>` / `std::vector<>` |
| `Computer.cpp` | `(bool)(j%2)` → `static_cast<bool>`; added `#include <unistd.h>` for `usleep` |
| `NetworkDesignerParser.cpp` | `bool * okki = malloc(...)` → local `bool okki = false`; removed `free(okki)`; `(char*)` → `const char*`; `#include<QtWidgets/QMessageBox>` |
| `DesignPlan.cpp` | Added `getScale()` / `setScale()` methods |
| `designplan.h` | `#include<QtGui/QMouseEvent>` (corrected; `QMouseEvent` is in `QtGui` in Qt5) |
| `mainWindow.ui` | Replaced `<property name="contentsMargins">` with per-axis margin properties to work around `uic` 5.15 code-generation quirk |

#### Build / CI

| Area | Change |
|------|--------|
| `src/CMakeLists.txt` | Added `designplan.h` to sources so AUTOMOC generates the `DesignPlan` MOC |
| `.github/workflows/ci.yml` | Added `libqt5xml5-dev`, `xvfb`; runs unit tests and integration tests separately |

### Test results (v1.0.0)

#### Unit tests — 100/100 PASSED

| Suite | Tests |
|-------|-------|
| NeuronTest | 28 |
| NetworkTest | 24 |
| SimulationTest | 8 |
| NetworkStateTest | 20 |
| NetworkStatesSetTest | 20 |

#### Integration tests — 26/26 PASSED

All 13 example `.nml` files load correctly with the expected neuron and synapse counts:

| File | Neurons | Synapses | Status |
|------|---------|----------|--------|
| `3X3OscilloX8.nml` | 48 | 144 | PASS |
| `Arabidopsis_premodel.nml` | 12 | 24 | PASS |
| `Arabiodopsis_thalina.nml` | 12 | 25 | PASS |
| `CardioRespiratoire.nml` | 4 | 6 | PASS |
| `EukarioteCell.nml` | 12 | 15 | PASS |
| `HeartBeat.nml` | 10 | 42 | PASS |
| `TheBigOne.nml` | 10 | 54 | PASS |
| `allPositive_And_Cycle.nml` | 3 | 8 | PASS |
| `anotherCoolNetwork.nml` | 8 | 9 | PASS |
| `doubleOSCILLO.nml` | 4 | 7 | PASS |
| `eukarioteCells.nml` | 12 | 15 | PASS |
| `grilleWithoutBorder.nml` | 48 | 204 | PASS |
| `peaceMakerGrid.nml` | 25 | 40 | PASS |

### Known limitations

- The exhaustive attractor finder (`SimulationAttractorsAndBasinsOfAttraction`) is exponential in the number of neurons; use it only for networks with ≤ 20 neurons.
- Undo/redo is not yet implemented.
- Export to GNBox Premodel format is experimental.
- Multi-valued neurons (NbStates > 2) work in the core engine but the UI does not yet provide a threshold editor for more than two states.
