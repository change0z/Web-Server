cmake_minimum_required(VERSION 3.10)
project(VotingSystem)

set(CMAKE_CXX_STANDARD 17)

# Include directories for the new structure
include_directories(src/core)
include_directories(src/services)
include_directories(src/regional)
include_directories(src/validation)

# Shared library for common election functionality
add_library(ElectionLib
    src/core/Election.cpp
    src/core/Candidate.cpp
    src/core/Voter.cpp
    src/core/party.cpp
    src/regional/Region.cpp
    src/validation/InputValidator.cpp
)

# ClearBallot Service Library
add_library(ClearBallotServiceLib
    src/services/ClearBallotService.cpp
    src/services/ServiceClient.cpp
)
target_link_libraries(ClearBallotServiceLib ElectionLib)

# Election Administration Tool
add_executable(ElectionAdmin
    applications/admin/admin_main.cpp
)
target_link_libraries(ElectionAdmin ElectionLib ClearBallotServiceLib)

# Voter Interface
add_executable(VoterInterface
    applications/voter/voter_main.cpp
)
target_link_libraries(VoterInterface ElectionLib ClearBallotServiceLib)

# Standalone ClearBallot Service
add_executable(ClearBallotService
    applications/service/service_main.cpp
)
target_link_libraries(ClearBallotService ElectionLib ClearBallotServiceLib)

# Keep the original for compatibility
add_executable(VotingSystem
    applications/main.cpp
)
target_link_libraries(VotingSystem ElectionLib ClearBallotServiceLib)

# Service Infrastructure Test
add_executable(ServiceTest
    tests/service_test.cpp
)
target_link_libraries(ServiceTest ElectionLib ClearBallotServiceLib)

# Regional Voting System Demo
add_executable(RegionalDemo
    applications/demos/regional_demo.cpp
)
target_link_libraries(RegionalDemo ElectionLib)

# Test Regional Restrictions
add_executable(TestRestrictions
    applications/demos/test_restrictions.cpp
)
target_link_libraries(TestRestrictions ElectionLib)

# Include testing directory
add_subdirectory(tests)