cmake_minimum_required(VERSION 3.10)
project(VotingSystem)

set(CMAKE_CXX_STANDARD 17)

add_executable(VotingSystem
    main.cpp
    Candidate.cpp
    Voter.cpp
    Election.cpp
    party.cpp
)