cmake_minimum_required(VERSION 3.10)
project(VotingSystem)

set(CMAKE_CXX_STANDARD 17)



# Shared library for common election functionality
add_library(ElectionLib
    Election.cpp
    Candidate.cpp
    Voter.cpp
    party.cpp
    InputValidator.cpp
)

# ClearBallot Service Library
add_library(ClearBallotServiceLib
    ClearBallotService.cpp
    ServiceClient.cpp
)
target_link_libraries(ClearBallotServiceLib ElectionLib)

# Election Administration Tool
add_executable(ElectionAdmin
    admin_main.cpp
)
target_link_libraries(ElectionAdmin ElectionLib ClearBallotServiceLib)

# Voter Interface
add_executable(VoterInterface
    voter_main.cpp
)
target_link_libraries(VoterInterface ElectionLib ClearBallotServiceLib)

# Keep the original for compatibility
add_executable(VotingSystem
    main.cpp
)
target_link_libraries(VotingSystem ElectionLib ClearBallotServiceLib)

# Service Infrastructure Test
add_executable(ServiceTest
    service_test.cpp
)
target_link_libraries(ServiceTest ElectionLib ClearBallotServiceLib)