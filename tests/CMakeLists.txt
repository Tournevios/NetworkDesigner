# Unit tests — requires Google Test
# Install: apt install libgtest-dev  or  vcpkg install gtest
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(STATUS "GTest not found — skipping tests. Install libgtest-dev to enable.")
    return()
endif()

set(TEST_SOURCES
    unit/test_Network.cpp
    unit/test_NetworkState.cpp
    unit/test_Simulation.cpp
    unit/test_NetworkStatesSet.cpp
    unit/test_Neuron.cpp
)

# Only compile what the current tests actually use.
# Simulation subclasses depend on Computer (src/app) and QThread — add them
# here only once a QCoreApplication fixture is introduced.
set(CORE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/core/network/Network.cpp
    ${CMAKE_SOURCE_DIR}/src/core/network/Neuron.cpp
    ${CMAKE_SOURCE_DIR}/src/core/network/Synapse.cpp
    ${CMAKE_SOURCE_DIR}/src/core/state/NetworkState.cpp
    ${CMAKE_SOURCE_DIR}/src/core/state/NetworkState2.cpp
    ${CMAKE_SOURCE_DIR}/src/core/state/NetworkStatesSet.cpp
    ${CMAKE_SOURCE_DIR}/src/core/state/SetOfNetworkStatesSet.cpp
    ${CMAKE_SOURCE_DIR}/src/core/scheduling/UpdateBlock.cpp
    ${CMAKE_SOURCE_DIR}/src/core/scheduling/UpdateSchedulingPlan.cpp
    ${CMAKE_SOURCE_DIR}/src/core/scheduling/BlockSelector.cpp
    ${CMAKE_SOURCE_DIR}/src/core/utils/random-singleton.cpp
    # pCalculateTransitions.cpp inclut transitivemet Simulation.h → Computer.h (src/app)
    # À ajouter ici uniquement quand src/app sera inclus dans le build de test
)

add_executable(NetworkDesigner_tests ${TEST_SOURCES} ${CORE_SOURCES})

target_include_directories(NetworkDesigner_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/network
    ${CMAKE_SOURCE_DIR}/src/core/simulation
    ${CMAKE_SOURCE_DIR}/src/core/state
    ${CMAKE_SOURCE_DIR}/src/core/scheduling
    ${CMAKE_SOURCE_DIR}/src/core/utils
)

target_link_libraries(NetworkDesigner_tests PRIVATE GTest::GTest GTest::Main ${QT_LIBS})

include(GoogleTest)
gtest_discover_tests(NetworkDesigner_tests)

# ── Integration tests (require QApplication + example NML files) ─────────────
set(INTEGRATION_SOURCES
    integration/test_nml_loading.cpp
    ${CMAKE_SOURCE_DIR}/src/app/NetworkDesignerParser.cpp
)

add_executable(NetworkDesigner_integration_tests ${INTEGRATION_SOURCES} ${CORE_SOURCES})

target_compile_definitions(NetworkDesigner_integration_tests PRIVATE
    EXAMPLES_DIR_DEFAULT="${CMAKE_SOURCE_DIR}/examples"
)

target_include_directories(NetworkDesigner_integration_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/network
    ${CMAKE_SOURCE_DIR}/src/core/simulation
    ${CMAKE_SOURCE_DIR}/src/core/state
    ${CMAKE_SOURCE_DIR}/src/core/scheduling
    ${CMAKE_SOURCE_DIR}/src/core/utils
    ${CMAKE_SOURCE_DIR}/src/app
)

target_link_libraries(NetworkDesigner_integration_tests PRIVATE
    GTest::GTest ${QT_LIBS}
)

add_test(
    NAME IntegrationTests_NmlLoading
    COMMAND NetworkDesigner_integration_tests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_tests_properties(IntegrationTests_NmlLoading PROPERTIES
    ENVIRONMENT "DISPLAY=:99;EXAMPLES_DIR=${CMAKE_SOURCE_DIR}/examples"
)
