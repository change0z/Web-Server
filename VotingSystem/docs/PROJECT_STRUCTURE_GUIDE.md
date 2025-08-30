# ClearBallot Project Structure Guide

## Overview
This document describes the organized folder structure of the ClearBallot voting system, designed for scalability, maintainability, and clear separation of concerns.

## Folder Structure

```
VotingSystem/
├── src/                           # Source code organized by domain
│   ├── core/                      # Core domain logic
│   │   ├── Election.h/.cpp        # Central election management
│   │   ├── Candidate.h/.cpp       # Candidate entity
│   │   ├── Voter.h/.cpp           # Voter entity  
│   │   └── party.h/.cpp           # Political party entity
│   ├── services/                  # Service architecture layer
│   │   ├── ClearBallotService.h/.cpp  # Core service with thread safety
│   │   └── ServiceClient.h/.cpp   # Client interface layer
│   ├── regional/                  # Regional voting functionality
│   │   └── Region.h/.cpp          # Regional management and restrictions
│   └── validation/                # Input validation and security
│       └── InputValidator.h/.cpp  # Comprehensive validation rules
├── applications/                  # Executable applications
│   ├── admin/                     # Election administration tools
│   │   └── admin_main.cpp         # Election setup and management
│   ├── voter/                     # Voter-facing applications
│   │   └── voter_main.cpp         # Voter registration and voting
│   ├── demos/                     # Demonstration applications
│   │   ├── regional_demo.cpp      # Interactive regional voting demo
│   │   └── test_restrictions.cpp  # Automated restriction testing
│   └── main.cpp                   # Legacy compatibility application
├── tests/                         # Test applications and validation
│   └── service_test.cpp           # Service infrastructure testing
├── docs/                          # Documentation and guides
│   ├── SERVICE_ARCHITECTURE_GUIDE.md
│   ├── REGIONAL_VOTING_GUIDE.md
│   ├── INPUT_VALIDATION_GUIDE.md
│   ├── DATA_PERSISTENCE_GUIDE.md
│   └── PROJECT_STRUCTURE_GUIDE.md (this file)
├── Debug/                         # Build output directory
├── CMakeLists.txt                 # Build configuration
└── *.vcxproj                      # Visual Studio project files
```

## Domain Architecture

### 1. Core Domain (`src/core/`)
**Purpose**: Fundamental business logic and entities
**Dependencies**: None (self-contained)
**Key Files**:
- `Election.h/.cpp`: Central orchestrator managing all aspects of elections
- `Candidate.h/.cpp`: Represents election candidates with vote tracking
- `Voter.h/.cpp`: Represents registered voters with eligibility and status
- `party.h/.cpp`: Political party management and membership

**Design Principles**:
- Pure domain logic without external dependencies
- Smart pointer memory management (`std::unique_ptr`, `std::shared_ptr`)
- STL containers for efficient data management
- RAII resource management patterns

### 2. Service Layer (`src/services/`)
**Purpose**: Service-oriented architecture for scalability
**Dependencies**: Core domain
**Key Files**:
- `ClearBallotService.h/.cpp`: Thread-safe service managing multiple elections
- `ServiceClient.h/.cpp`: Clean API interface for applications

**Design Principles**:
- Thread-safe concurrent access with `std::mutex`
- Multi-instance election management
- Type-safe communication with `ServiceResponse<T>`
- Zero-risk delegation to existing Election class

### 3. Regional Management (`src/regional/`)
**Purpose**: Geographical voting restrictions and organization
**Dependencies**: Core domain
**Key Files**:
- `Region.h/.cpp`: Regional voting management with strict restrictions

**Business Rules Enforced**:
- Same candidate cannot be in multiple regions
- One candidate per party per region
- Voters can only vote in assigned regions
- Parties can operate across multiple regions

### 4. Validation Layer (`src/validation/`)
**Purpose**: Input validation and security
**Dependencies**: None (utility layer)
**Key Files**:
- `InputValidator.h/.cpp`: Comprehensive validation rules

**Validation Rules**:
- Names: Alphabetic characters only, 2+ chars
- Phone: Exactly 10 digits
- ID: Exactly 9 digits
- Age: 18+ for voting eligibility
- Address: 5+ characters minimum

## Application Architecture

### 1. Administration (`applications/admin/`)
**Purpose**: Election setup and management tools
**Target Users**: Election officials and administrators
**Features**:
- Party and candidate registration
- Election configuration
- Data export and persistence
- Complete election data management

### 2. Voter Interface (`applications/voter/`)
**Purpose**: Voter-facing applications
**Target Users**: Registered voters
**Features**:
- Voter registration with validation
- Secure voting interface
- Regional voting support
- Election results viewing

### 3. Demonstrations (`applications/demos/`)
**Purpose**: Showcase system capabilities
**Target Users**: Developers and stakeholders
**Files**:
- `regional_demo.cpp`: Interactive regional voting demonstration
- `test_restrictions.cpp`: Automated validation of all regional restrictions

### 4. Legacy Support (`applications/main.cpp`)
**Purpose**: Backward compatibility
**Features**: Original console-based voting system interface

## Testing Strategy

### Service Testing (`tests/service_test.cpp`)
**Purpose**: Validates service architecture integrity
**Coverage**:
- Service connection and initialization
- Multi-election management
- Thread safety verification
- Client-service communication

### Regional Testing (`applications/demos/test_restrictions.cpp`)
**Purpose**: Validates all regional restrictions
**Coverage**:
- Candidate uniqueness across regions
- Party-candidate limits per region
- Cross-region voting prevention
- Valid scenario verification

## Build Configuration

### CMake Structure
```cmake
# Core library with all domain logic
add_library(ElectionLib
    src/core/Election.cpp
    src/core/Candidate.cpp
    src/core/Voter.cpp
    src/core/party.cpp
    src/regional/Region.cpp
    src/validation/InputValidator.cpp
)

# Service layer library
add_library(ClearBallotServiceLib
    src/services/ClearBallotService.cpp
    src/services/ServiceClient.cpp
)

# Applications link to appropriate libraries
target_link_libraries(ElectionAdmin ElectionLib ClearBallotServiceLib)
```

### Include Path Management
- `include_directories(src/core)` - Core domain accessible globally
- `include_directories(src/services)` - Service layer for applications
- `include_directories(src/regional)` - Regional functionality
- `include_directories(src/validation)` - Validation utilities

## Benefits of This Structure

### 1. Separation of Concerns
- **Domain Logic**: Isolated in `src/core/` without external dependencies
- **Service Infrastructure**: Clean abstraction in `src/services/`
- **Regional Features**: Modular in `src/regional/`
- **Applications**: User-facing logic separated from business logic

### 2. Scalability
- **Modular Libraries**: Easy to extend individual components
- **Clear Dependencies**: Predictable build and deployment
- **Service Architecture**: Ready for web/mobile interfaces
- **Regional Support**: Supports any number of geographical regions

### 3. Maintainability
- **Logical Organization**: Related files grouped together
- **Clear Interfaces**: Well-defined boundaries between layers
- **Documentation**: Comprehensive guides for each domain
- **Testing**: Isolated testing strategies for each component

### 4. Developer Experience
- **Intuitive Navigation**: Easy to find relevant code
- **Predictable Structure**: Follows standard architectural patterns
- **Build Efficiency**: Modular compilation and linking
- **IDE Integration**: Better project navigation and IntelliSense

## Future Expansion Guidelines

### Adding New Features
1. **Core Logic**: Add to `src/core/` if fundamental to voting
2. **Service Features**: Extend `src/services/` for new APIs
3. **Regional Features**: Add to `src/regional/` for geographical functionality
4. **Applications**: Create new folders under `applications/` for new interfaces

### Adding New Applications
1. Create appropriate subfolder under `applications/`
2. Update `CMakeLists.txt` with new executable
3. Link to required libraries (ElectionLib, ClearBallotServiceLib)
4. Follow existing include patterns for dependencies

### Adding New Tests
1. Add test files to `tests/` directory
2. Create executable in CMakeLists.txt
3. Link to libraries being tested
4. Follow existing patterns for validation and service testing

This organized structure provides a solid foundation for the ClearBallot system's continued growth and evolution while maintaining code quality and developer productivity.
