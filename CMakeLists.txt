cmake_minimum_required(VERSION 3.16)
project(NetworkDesigner VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Prefer Qt6, fall back to Qt5
find_package(Qt6 QUIET COMPONENTS Core Gui Widgets Xml)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
    set(QT_LIBS Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Xml)
    message(STATUS "Using Qt6")
else()
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Xml)
    set(QT_VERSION_MAJOR 5)
    set(QT_LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Xml)
    message(STATUS "Using Qt5")
endif()

set(SOURCES
    main.cpp
    networkdesigner.cpp
    Network.cpp
    Neuron.cpp
    Synapse.cpp
    NeuronSynapse.h
    DesignPlan.cpp
    NetworkDesignerParser.cpp
    EvenementHandler.cpp
    SignalsSlotsConnector.cpp
    UpdateBlock.cpp
    UpdateSchedulingPlan.cpp
    Computer.cpp
    Simulation.cpp
    SimulationActivity.cpp
    SimulationAttractorsAndBasinsOfAttraction.cpp
    SimulationAttractorsAndBasinsOfAttraction2.cpp
    SimulationChangement.cpp
    SimulationDiffusion.cpp
    NetworkState.cpp
    NetworkState2.cpp
    NetworkStatesSet.cpp
    SetOfNetworkStatesSet.cpp
    BlockSelector.cpp
    pCalculateTransitions.cpp
    random-singleton.cpp
)

set(HEADERS
    networkdesigner.h
    Network.h
    NeuronSynapse.h
    designplan.h
    NetworkDesignerParser.h
    EvenementHandler.h
    SignalsSlotsConnector.h
    UpdateBlock.h
    UpdateSchedulingPlan.h
    Computer.h
    Simulation.h
    SimulationActivity.h
    SimulationAttractorsAndBasinsOfAttraction.h
    SimulationAttractorsAndBasinsOfAttraction2.h
    SimulationChangement.h
    SimulationDiffusion.h
    NetworkState.h
    NetworkState2.h
    NetworkStatesSet.h
    SetOfNetworkStatesSet.h
    BlockSelector.h
    pCalculateTransitions.h
    random-singleton.h
    constFile.h
)

set(FORMS
    mainWindow.ui
    networkdesigner.ui
)

set(RESOURCES
    resources.qrc
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBS})

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
