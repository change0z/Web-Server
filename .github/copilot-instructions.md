# Copilot Instructions for Web-Server Project

## Project Overview
This is a C++ voting system application with a clean object-oriented architecture. Despite the repository name "Web-Server", the current implementation is a console-based voting system located in the `VotingSystem/` directory.

## Architecture Patterns

### Core Components
- **Election**: Central orchestrator class that manages candidates, voters, and the voting process
- **Candidate**: Represents election candidates with vote tracking functionality  
- **Voter**: Represents registered voters with unique IDs
- **main.cpp**: Entry point demonstrating the complete voting workflow

### Memory Management Pattern
The project uses modern C++ smart pointers consistently:
```cpp
std::vector<std::unique_ptr<Candidate>> candidates;
std::vector<std::unique_ptr<Voter>> voters;
```
Always use `std::make_unique<>()` for object creation, as seen in `Election::addCandidate()` and `Election::registerVoter()`.

### Error Handling Convention
Input validation is performed with bounds checking and console output:
```cpp
if (candidateIndex < 0 || candidateIndex >= static_cast<int>(candidates.size())) {
    std::cout << "Invalid candidate index. \n";
    return;
}
```
Follow this pattern for all validation - check bounds, output descriptive error, and return early.

## Build System
- **CMake**: Uses CMake 3.10+ with C++17 standard
- **Build command**: `cmake . && make` (or equivalent for Windows)
- **Executable name**: `VotingSystem`

## File Organization Conventions
- Header files use `#pragma once` for include guards
- Class implementations follow the pattern: constructor initialization lists, public methods, private members
- All classes have separate `.h` and `.cpp` files with matching names

## Key Workflows

### Adding New Features
1. Voting system expansion should extend the `Election` class
2. New entity types should follow the `Candidate`/`Voter` pattern with separate header/implementation
3. Update `CMakeLists.txt` to include new source files

### Data Flow Pattern
Election workflow: `registerVoter()` → `addCandidate()` → `castVote()` → `displayResults()` → `saveResultsToFile()`

The vote casting uses candidate indices (0-based) rather than candidate objects directly.

## Development Notes
- No current web server implementation despite repository name
- File I/O uses basic `std::ofstream` with error checking
- Console output provides immediate feedback for all operations
- Voter ID validation is registered but not enforced during vote casting (potential enhancement area)
