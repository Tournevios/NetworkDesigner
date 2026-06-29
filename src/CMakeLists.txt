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

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(NetworkDesigner
        ${CORE_SOURCES}
        ${APP_SOURCES}
        ${APP_FORMS}
        ${CMAKE_SOURCE_DIR}/resources/resources.qrc
    )
else()
    add_executable(NetworkDesigner
        ${CORE_SOURCES}
        ${APP_SOURCES}
        ${APP_FORMS}
        ${CMAKE_SOURCE_DIR}/resources/resources.qrc
    )
endif()

target_include_directories(NetworkDesigner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core/network
    ${CMAKE_CURRENT_SOURCE_DIR}/core/simulation
    ${CMAKE_CURRENT_SOURCE_DIR}/core/state
    ${CMAKE_CURRENT_SOURCE_DIR}/core/scheduling
    ${CMAKE_CURRENT_SOURCE_DIR}/core/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/app
)

target_link_libraries(NetworkDesigner PRIVATE ${QT_LIBS})

if(ANDROID)
    target_link_libraries(NetworkDesigner PRIVATE log)
endif()

target_compile_options(NetworkDesigner PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

# No console window on Windows; bundle on macOS
set_target_properties(NetworkDesigner PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

if(NOT ANDROID)
    install(TARGETS NetworkDesigner RUNTIME DESTINATION bin)
endif()

# Android: point androiddeployqt at our custom manifest.
# Must be set in the same directory scope as qt_add_executable so that
# qt_finalize_executable (deferred to end of this scope) picks it up.
if(ANDROID)
    set_target_properties(NetworkDesigner PROPERTIES
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android"
    )
endif()
