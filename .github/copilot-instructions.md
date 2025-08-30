# Copilot Instructions for ClearBallot Voting System

## Project Overview
**ClearBallot** is a comprehensive C++ election management system that has evolved from a simple console voting application into a sophisticated, service-oriented architecture supporting regional voting, input validation, and scalable interfaces. Despite the repository name "Web-Server", this is currently a desktop-based voting system with architectural foundations ready for web and mobile expansion.

### Current System State (August 2025)
- **Branch**: `runtime-service-exp` - Service architecture implementation
- **Architecture**: Service-oriented with modular domain structure
- **Status**: Production-ready with comprehensive regional voting and validation
- **Build System**: CMake with multiple libraries and 8+ executables

## System Architecture

### 🏗️ **Organized Project Structure (Professional Domain-Based)**

```
VotingSystem/
├── src/                           # Source code organized by domain
│   ├── core/                      # Core domain logic (Election, Candidate, Voter, Party)
│   ├── services/                  # Service architecture layer (ClearBallotService, ServiceClient)
│   ├── regional/                  # Regional voting functionality (Region)
│   └── validation/                # Input validation and security (InputValidator)
├── applications/                  # Executable applications
│   ├── admin/                     # Election administration tools
│   ├── voter/                     # Voter-facing applications
│   ├── demos/                     # Demonstration applications
│   └── main.cpp                   # Legacy compatibility
├── tests/                         # Test applications and validation
├── docs/                          # Comprehensive documentation (9 guides)
└── CMakeLists.txt                 # Modular build configuration
```

### 🎯 **Core Components (Enhanced Architecture)**

#### **1. Service-Oriented Architecture**
- **ClearBallotService**: Thread-safe service managing multiple elections
  - Multi-election support with `std::map<int, std::shared_ptr<Election>>`
  - Thread safety with `std::mutex` and `std::lock_guard`
  - Service lifecycle management (start/stop with data persistence)
  - Zero-risk delegation to existing Election class

- **ServiceClient**: Clean API interface for applications
  - Type-safe communication with `ServiceResponse<T>` structures
  - Backward compatibility - direct Election access available
  - Default election support for single-election scenarios

#### **2. Regional Voting System**
- **Region Class**: Geographical voting districts with strict isolation
  - **Restriction 1**: Same candidate cannot be in multiple regions
  - **Restriction 2**: One candidate per party per region 
  - **Restriction 3**: Voters can only vote in their assigned region
  - **Valid**: Multiple parties per region, same party across regions

#### **3. Comprehensive Input Validation**
- **InputValidator**: Security-focused validation system
  - Names: Alphabetic only, 2-50 characters
  - Phone: Exactly 10 digits
  - ID: Exactly 9 digits  
  - Age: 18+ for voting eligibility
  - Address: 5+ characters minimum

#### **4. Enhanced Core Domain**
- **Election**: Central orchestrator enhanced with regional management
  - Regional methods: `createRegion()`, `addCandidateToRegion()`, `castVoteInRegion()`
  - Multi-instance support via service architecture
  - Complete data persistence and CSV export

- **Candidate/Voter**: Enhanced with region assignment
  - Region-based validation and restrictions
  - Smart pointer memory management throughout

- **Party**: Political party management with cross-regional support
  - Shared across regions with candidate limits per region

### 💻 **Applications & Executables (8 Built Applications)**

1. **ElectionAdmin.exe** - Administration interface for election officials
2. **VoterInterface.exe** - Voter registration and voting interface  
3. **VotingSystem.exe** - Legacy compatibility application
4. **ServiceTest.exe** - Comprehensive service architecture testing
5. **RegionalDemo.exe** - Interactive regional voting demonstration
6. **TestRestrictions.exe** - Automated validation of regional restrictions
7. **ValidationDemo.exe** - Input validation testing and demonstration
8. **DataPersistenceDemo.exe** - Data persistence and export functionality

### 🔒 **Security & Data Management**
- **Complete Election Records**: Full voter registry, voting records, and results
- **Multiple Export Formats**: TXT (human-readable) and CSV (analysis-ready)
- **Audit Trail**: Comprehensive election data for verification and compliance
- **Input Validation**: Prevents security vulnerabilities with strict validation rules
- **Thread Safety**: Concurrent access protection with mutex synchronization

## Technical Patterns & Best Practices

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

## Development Roadmap & Plans

### **Service Architecture Evolution**

#### **Phase 1: Service Infrastructure ✅ COMPLETE**
- [x] ClearBallotService core implementation with thread safety
- [x] ServiceClient interface layer with type-safe communication
- [x] Multi-election support with concurrent access
- [x] Comprehensive testing suite (ServiceTest.exe)
- [x] Backward compatibility with existing console applications

#### **Phase 2: Console Interface Migration (In Progress)**
- [ ] Migrate `voter_main.cpp` to use ServiceClient API
- [ ] Migrate `admin_main.cpp` to use ServiceClient API  
- [ ] Maintain identical user experience during transition
- [ ] Validate service-based operations against legacy behavior

#### **Phase 3: Service Enhancement (Planned)**
- [ ] Authentication tokens for administrative operations
- [ ] Comprehensive audit logging for all election operations
- [ ] Database integration (SQLite for development, PostgreSQL for production)
- [ ] RESTful API layer foundation for web interfaces

#### **Phase 4: Interface Expansion (Future Vision)**
- [ ] Web-based voting interface using service architecture
- [ ] Mobile application support (iOS/Android)
- [ ] Real-time election monitoring dashboard
- [ ] Advanced reporting and analytics capabilities

### **Regional Voting System Status ✅ COMPLETE**
- [x] Region class with geographical restrictions
- [x] Candidate uniqueness across regions enforcement
- [x] One candidate per party per region validation
- [x] Cross-region voting prevention
- [x] Comprehensive testing with TestRestrictions.exe

### **Input Validation System Status ✅ COMPLETE**
- [x] InputValidator class with comprehensive rules
- [x] Name validation (alphabetic, 2-50 chars)
- [x] Phone validation (exactly 10 digits)
- [x] ID validation (exactly 9 digits)
- [x] Age validation (18+ for voting eligibility)
- [x] Address validation (5+ characters minimum)

### **Project Structure Status ✅ COMPLETE**
- [x] Organized domain-based folder structure
- [x] src/core, src/services, src/regional, src/validation separation
- [x] applications/ folder with admin, voter, demos subfolders
- [x] tests/ folder for testing applications
- [x] docs/ folder with comprehensive guides (9 documentation files)
- [x] Updated CMakeLists.txt with modular library structure

## Key Workflows

### Adding New Features
1. **Core Logic**: Add to `src/core/` if fundamental to voting system
2. **Service Features**: Extend `src/services/` for new service APIs
3. **Regional Features**: Add to `src/regional/` for geographical functionality
4. **Applications**: Create new folders under `applications/` for new user interfaces
5. Update `CMakeLists.txt` to include new source files and link appropriate libraries

### Data Flow Pattern
#### Service-Based Workflow (Recommended):
ServiceClient → ClearBallotService → Election → Data Persistence

#### Legacy Workflow (Backward Compatible):
Direct Election access: `registerVoter()` → `addCandidate()` → `castVote()` → `displayResults()` → `saveResultsToFile()`

#### Regional Workflow:
`createRegion()` → `addCandidateToRegion()` → `registerVoterInRegion()` → `castVoteInRegion()` → `displayResultsInRegion()`

### Service Usage Patterns
```cpp
// Modern service-based approach
ServiceClient client;
client.connectToService();
auto response = client.registerVoter(electionId, voterData);

// Legacy compatibility (still supported)
auto election = client.getElection(-1);  // -1 = default election
election->registerVoter(...);  // Original method calls
```

## Development Notes
- **Architecture Evolution**: Transitioned from monolithic to service-oriented architecture
- **Regional Support**: Complete geographical voting district implementation with strict business rules
- **Input Validation**: Production-ready security-focused validation preventing malicious input
- **Thread Safety**: Mutex-protected concurrent access enabling multiple simultaneous elections
- **Data Persistence**: Enhanced from basic file I/O to comprehensive election data management
- **Backward Compatibility**: All existing functionality preserved during architectural evolution
- **Future Ready**: Architecture foundations support web interfaces, mobile apps, and database integration
- Vote casting uses candidate indices (0-based) rather than candidate objects directly
- Comprehensive testing with 8 different executable applications covering all functionality
