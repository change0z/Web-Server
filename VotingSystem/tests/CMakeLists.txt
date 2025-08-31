# Testing CMakeLists.txt for ClearBallot System
cmake_minimum_required(VERSION 3.10)

# Test executables configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories for all tests
include_directories(${CMAKE_SOURCE_DIR}/src/core)
include_directories(${CMAKE_SOURCE_DIR}/src/services)
include_directories(${CMAKE_SOURCE_DIR}/src/regional)
include_directories(${CMAKE_SOURCE_DIR}/src/validation)

# Core Unit Tests
add_executable(TestElectionCore
    unit/core/test_election_core.cpp
)
target_link_libraries(TestElectionCore ElectionLib)

# Service Architecture Tests
add_executable(TestServiceArchitecture
    unit/services/test_service_architecture.cpp
)
target_link_libraries(TestServiceArchitecture ClearBallotServiceLib ElectionLib)

# Input Validation Tests
add_executable(TestInputValidation
    unit/validation/test_input_validation.cpp
)
target_link_libraries(TestInputValidation ElectionLib)

# Regional Voting Tests
add_executable(TestRegionalVoting
    unit/regional/test_regional_voting.cpp
)
target_link_libraries(TestRegionalVoting ElectionLib)

# Integration Tests
add_executable(TestSystemIntegration
    integration/test_full_system_integration.cpp
)
target_link_libraries(TestSystemIntegration ClearBallotServiceLib ElectionLib)

# Test runner target - runs all tests
add_custom_target(run_all_tests
    COMMAND echo "=== Running All ClearBallot Tests ==="
    COMMAND echo ""
    COMMAND echo "--- Core Election Tests ---"
    COMMAND $<TARGET_FILE:TestElectionCore>
    COMMAND echo ""
    COMMAND echo "--- Service Architecture Tests ---"
    COMMAND $<TARGET_FILE:TestServiceArchitecture>
    COMMAND echo ""
    COMMAND echo "--- Input Validation Tests ---"
    COMMAND $<TARGET_FILE:TestInputValidation>
    COMMAND echo ""
    COMMAND echo "--- Regional Voting Tests ---"
    COMMAND $<TARGET_FILE:TestRegionalVoting>
    COMMAND echo ""
    COMMAND echo "--- System Integration Tests ---"
    COMMAND $<TARGET_FILE:TestSystemIntegration>
    COMMAND echo ""
    COMMAND echo "=== All Tests Complete ==="
    DEPENDS TestElectionCore TestServiceArchitecture TestInputValidation TestRegionalVoting TestSystemIntegration
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Individual test runner targets
add_custom_target(test_core
    COMMAND $<TARGET_FILE:TestElectionCore>
    DEPENDS TestElectionCore
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(test_services
    COMMAND $<TARGET_FILE:TestServiceArchitecture>
    DEPENDS TestServiceArchitecture
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(test_validation
    COMMAND $<TARGET_FILE:TestInputValidation>
    DEPENDS TestInputValidation
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(test_regional
    COMMAND $<TARGET_FILE:TestRegionalVoting>
    DEPENDS TestRegionalVoting
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(test_integration
    COMMAND $<TARGET_FILE:TestSystemIntegration>
    DEPENDS TestSystemIntegration
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Help target to show available test commands
add_custom_target(test_help
    COMMAND echo "Available test targets:"
    COMMAND echo "  run_all_tests      - Run all test suites"
    COMMAND echo "  test_core          - Run core Election tests"
    COMMAND echo "  test_services      - Run service architecture tests" 
    COMMAND echo "  test_validation    - Run input validation tests"
    COMMAND echo "  test_regional      - Run regional voting tests"
    COMMAND echo "  test_integration   - Run system integration tests"
    COMMAND echo ""
    COMMAND echo "Example usage:"
    COMMAND echo "  cmake --build . --target run_all_tests"
    COMMAND echo "  cmake --build . --target test_core"
)
