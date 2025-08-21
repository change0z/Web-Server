cmake_minimum_required(VERSION 3.10)
project(VotingSystem)

set(CMAKE_CXX_STANDARD 17)

# Shared library for common election functionality
add_library(ElectionLib
    Election.cpp
    Candidate.cpp
    Voter.cpp
    party.cpp
)

# Election Administration Tool
add_executable(ElectionAdmin
    admin_main.cpp
)
target_link_libraries(ElectionAdmin ElectionLib)

# Voter Interface
add_executable(VoterInterface
    voter_main.cpp
)
target_link_libraries(VoterInterface ElectionLib)

# Keep the original for compatibility
add_executable(VotingSystem
    main.cpp
)
target_link_libraries(VotingSystem ElectionLib)