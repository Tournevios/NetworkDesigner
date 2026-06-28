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
    ${CMAKE_SOURCE_DIR}/src/core/utils/pCalculateTransitions.cpp
    ${CMAKE_SOURCE_DIR}/src/core/utils/random-singleton.cpp
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
