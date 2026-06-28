set(CORE_SOURCES
    core/network/Network.cpp
    core/network/Neuron.cpp
    core/network/Synapse.cpp
    core/simulation/Simulation.cpp
    core/simulation/SimulationActivity.cpp
    core/simulation/SimulationAttractorsAndBasinsOfAttraction.cpp
    core/simulation/SimulationAttractorsAndBasinsOfAttraction2.cpp
    core/simulation/SimulationChangement.cpp
    core/simulation/SimulationDiffusion.cpp
    core/state/NetworkState.cpp
    core/state/NetworkState2.cpp
    core/state/NetworkStatesSet.cpp
    core/state/SetOfNetworkStatesSet.cpp
    core/scheduling/UpdateBlock.cpp
    core/scheduling/UpdateSchedulingPlan.cpp
    core/scheduling/BlockSelector.cpp
    core/utils/pCalculateTransitions.cpp
    core/utils/random-singleton.cpp
)

set(APP_SOURCES
    app/main.cpp
    app/networkdesigner.cpp
    app/designplan.h
    app/DesignPlan.cpp
    app/Computer.cpp
    app/EvenementHandler.cpp
    app/NetworkDesignerParser.cpp
    app/SignalsSlotsConnector.cpp
)

set(APP_FORMS
    app/mainWindow.ui
    app/networkdesigner.ui
)

add_executable(NetworkDesigner
    ${CORE_SOURCES}
    ${APP_SOURCES}
    ${APP_FORMS}
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
)

target_include_directories(NetworkDesigner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core/network
    ${CMAKE_CURRENT_SOURCE_DIR}/core/simulation
    ${CMAKE_CURRENT_SOURCE_DIR}/core/state
    ${CMAKE_CURRENT_SOURCE_DIR}/core/scheduling
    ${CMAKE_CURRENT_SOURCE_DIR}/core/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/app
)

target_link_libraries(NetworkDesigner PRIVATE ${QT_LIBS})

target_compile_options(NetworkDesigner PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

install(TARGETS NetworkDesigner RUNTIME DESTINATION bin)
