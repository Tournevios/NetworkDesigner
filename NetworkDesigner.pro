QT += app \
    xml
TARGET = NetworkDesigner
QT += core \
    gui \
    xml
HEADERS += pCalculateTransitions.h \
    NetworkState2.h \
    SimulationAttractorsAndBasinsOfAttraction2.h \
    SetOfNetworkStatesSet.h \
    NetworkStatesSet.h \
    SimulationAttractorsAndBasinsOfAttraction.h \
    NetworkState.h \
    SimulationDiffusion.h \
    SimulationChangement.h \
    BlockSelector.h \
    Computer.h \
    SimulationActivity.h \
    Simulation.h \
    NeuronSynapse.h \
    random-singleton.h \
    designplan.h \
    NetworkDesignerParser.h \
    SignalsSlotsConnector.h \
    EvenementHandler.h \
    constFile.h \
    UpdateBlock.h \
    UpdateSchedulingPlan.h \
    Network.h \
    networkdesigner.h
SOURCES += pCalculateTransitions.cpp \
    NetworkState2.cpp \
    SimulationAttractorsAndBasinsOfAttraction2.cpp \
    SetOfNetworkStatesSet.cpp \
    NetworkStatesSet.cpp \
    SimulationAttractorsAndBasinsOfAttraction.cpp \
    NetworkState.cpp \
    SimulationDiffusion.cpp \
    SimulationChangement.cpp \
    BlockSelector.cpp \
    Computer.cpp \
    SimulationActivity.cpp \
    Simulation.cpp \
    Synapse.cpp \
    random-singleton.cpp \
    NetworkDesignerParser.cpp \
    SignalsSlotsConnector.cpp \
    EvenementHandler.cpp \
    DesignPlan.cpp \
    Neuron.cpp \
    UpdateBlock.cpp \
    UpdateSchedulingPlan.cpp \
    Network.cpp \
    main.cpp \
    networkdesigner.cpp
FORMS += mainWindow.ui
RESOURCES += 
CONFIG += console
