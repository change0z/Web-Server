# ClearBallot Service Deployment & Database Preparation Plan

## 📋 **Executive Summary**

This document outlines the evolution of ClearBallot from an in-process service architecture to a true enterprise-grade system with OS-level service deployment and database-ready data structures. The plan maintains backward compatibility while establishing foundations for production deployment.

## 🎯 **Project Vision**

**Current State**: Desktop applications with in-process service architecture
**Target State**: Enterprise service with OS registration, IPC communication, and database-ready data models

### **Core Objectives**
1. **True Service Architecture**: Deploy ClearBallot as Windows Service / Linux Daemon
2. **Inter-Process Communication**: Admin and voter tools communicate via IPC with running service
3. **Database Preparation**: Restructure data models for seamless database integration
4. **Production Ready**: Enterprise-grade deployment with proper service lifecycle management

## 🏗️ **Architecture Evolution**

### **Phase 0: Current Architecture (Baseline)**
```
AdminApp ──┐
VoterApp ──┼─► ClearBallotService (in-process) ──► Election ──► File I/O
DemoApps ──┘    │                                   │           │
               └─► ServiceClient                    └─► TXT/CSV Files
```

**Characteristics:**
- ✅ Service pattern established
- ✅ Thread-safe multi-election support
- ✅ Clean API abstraction
- ❌ Single-process limitation
- ❌ File-based data storage
- ❌ No OS-level service registration

### **Phase 1: Standalone Service (Target)**
```
AdminApp ──┐                                    ┌─► Database Models
VoterApp ──┼─► IPC/Pipes ──► ClearBallotService ┼─► Structured Data
WebApp ────┘                (Windows Service)   └─► JSON Serialization
```

**Characteristics:**
- ✅ True service-oriented architecture
- ✅ OS-level service registration
- ✅ Inter-process communication
- ✅ Database-ready data structures
- ✅ Multi-client support
- ✅ Production deployment ready

## 🧪 **Testing Strategy & Framework**

### **Testing Framework: Google Test (gtest) + Google Mock (gmock)**

**Why Google Test for ClearBallot:**
- ✅ **Enterprise Standard**: Widely adopted in production C++ projects
- ✅ **CMake Integration**: Seamless integration with existing build system
- ✅ **Parameterized Testing**: Perfect for regional restriction validation
- ✅ **Mock Support**: Essential for service layer and IPC testing
- ✅ **Thread Safety**: Advanced testing for concurrent service operations
- ✅ **Continuous Integration**: Easy integration with CI/CD pipelines

### **Testing Architecture**
```cpp
tests/
├── unit/                          # Unit tests for individual components
│   ├── core/                      # Core domain logic tests
│   │   ├── test_election.cpp      # Election class functionality
│   │   ├── test_candidate.cpp     # Candidate entity tests
│   │   ├── test_voter.cpp         # Voter entity tests
│   │   └── test_party.cpp         # Party management tests
│   ├── regional/                  # Regional voting system tests
│   │   ├── test_region.cpp        # Region class tests
│   │   └── test_restrictions.cpp  # Business rule validation
│   ├── validation/                # Input validation tests
│   │   └── test_input_validator.cpp
│   ├── services/                  # Service architecture tests
│   │   ├── test_service.cpp       # ClearBallotService tests
│   │   ├── test_service_client.cpp # ServiceClient tests
│   │   └── test_ipc.cpp           # Inter-process communication
│   └── models/                    # Database model tests
│       ├── test_election_record.cpp
│       ├── test_voter_record.cpp
│       └── test_vote_record.cpp
├── integration/                   # Integration tests
│   ├── test_service_integration.cpp
│   ├── test_regional_workflow.cpp
│   └── test_data_persistence.cpp
├── performance/                   # Performance benchmarks
│   ├── test_service_performance.cpp
│   └── test_concurrent_access.cpp
└── mocks/                         # Mock objects for testing
    ├── mock_service.h
    ├── mock_database.h
    └── mock_ipc.h
```

### **Key Testing Areas**

#### **1. Core Domain Testing**
```cpp
// tests/unit/core/test_election.cpp
class ElectionTest : public ::testing::Test {
protected:
    void SetUp() override {
        election = std::make_unique<Election>("Test Election");
    }
    
    std::unique_ptr<Election> election;
};

TEST_F(ElectionTest, AddCandidateIncreasesCount) {
    EXPECT_EQ(election->getCandidateCount(), 0);
    election->addCandidate("Test Candidate");
    EXPECT_EQ(election->getCandidateCount(), 1);
}

// Parameterized testing for regional restrictions
class RegionalRestrictionTest : public ::testing::TestWithParam<RegionTestCase> {
    // Test all regional business rules with multiple scenarios
};
```

#### **2. Service Architecture Testing**
```cpp
// tests/unit/services/test_service.cpp
class ClearBallotServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        service = std::make_unique<ClearBallotService>();
    }
    
    std::unique_ptr<ClearBallotService> service;
};

TEST_F(ClearBallotServiceTest, ConcurrentElectionCreation) {
    // Test thread safety with multiple simultaneous elections
    std::vector<std::thread> threads;
    std::atomic<int> successCount{0};
    
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&, i]() {
            auto response = service->createElection("Election " + std::to_string(i));
            if (response.success) {
                successCount++;
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_EQ(successCount.load(), 10);
}
```

#### **3. IPC Communication Testing**
```cpp
// tests/unit/services/test_ipc.cpp
class IPCTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockService = std::make_unique<MockClearBallotService>();
        comm = std::make_unique<ServiceCommunication>(mockService.get());
    }
    
    std::unique_ptr<MockClearBallotService> mockService;
    std::unique_ptr<ServiceCommunication> comm;
};

TEST_F(IPCTest, RequestResponseSerialization) {
    // Test JSON serialization/deserialization
    ServiceRequest request = createTestVoterRegistration();
    std::string serialized = request.toJSON();
    ServiceRequest deserialized = ServiceRequest::fromJSON(serialized);
    
    EXPECT_EQ(request.requestType, deserialized.requestType);
    EXPECT_EQ(request.data, deserialized.data);
}
```

#### **4. Database Model Testing**
```cpp
// tests/unit/models/test_voter_record.cpp
class VoterRecordTest : public ::testing::Test {
protected:
    VoterRecord createValidVoter() {
        VoterRecord voter;
        voter.first_name = "John";
        voter.last_name = "Doe";
        voter.phone_number = "1234567890";
        voter.unique_voter_id = "123456789";
        voter.age = 25;
        voter.address = "123 Main St";
        return voter;
    }
};

TEST_F(VoterRecordTest, JSONSerializationRoundTrip) {
    VoterRecord original = createValidVoter();
    nlohmann::json json = original.toJSON();
    VoterRecord deserialized = VoterRecord::fromJSON(json);
    
    EXPECT_EQ(original.first_name, deserialized.first_name);
    EXPECT_EQ(original.phone_number, deserialized.phone_number);
    EXPECT_EQ(original.age, deserialized.age);
}

// Parameterized validation testing
class VoterValidationTest : public ::testing::TestWithParam<std::pair<VoterRecord, bool>> {};

INSTANTIATE_TEST_SUITE_P(
    ValidationCases,
    VoterValidationTest,
    ::testing::Values(
        // Valid cases
        std::make_pair(createValidVoter(), true),
        // Invalid cases
        std::make_pair(createVoterWithInvalidPhone(), false),
        std::make_pair(createVoterUnder18(), false),
        std::make_pair(createVoterWithShortName(), false)
    )
);
```

### **Testing Integration with CMake**
```cmake
# CMakeLists.txt additions
enable_testing()

# Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test executables
add_executable(CoreTests
    tests/unit/core/test_election.cpp
    tests/unit/core/test_candidate.cpp
    tests/unit/core/test_voter.cpp
    tests/unit/regional/test_region.cpp
    tests/unit/regional/test_restrictions.cpp
    tests/unit/validation/test_input_validator.cpp
)
target_link_libraries(CoreTests ${GTEST_LIBRARIES} pthread ElectionLib)

add_executable(ServiceTests
    tests/unit/services/test_service.cpp
    tests/unit/services/test_service_client.cpp
    tests/unit/services/test_ipc.cpp
)
target_link_libraries(ServiceTests ${GTEST_LIBRARIES} pthread ClearBallotServiceLib)

add_executable(ModelTests
    tests/unit/models/test_election_record.cpp
    tests/unit/models/test_voter_record.cpp
    tests/unit/models/test_vote_record.cpp
)
target_link_libraries(ModelTests ${GTEST_LIBRARIES} pthread ElectionLib)

add_executable(IntegrationTests
    tests/integration/test_service_integration.cpp
    tests/integration/test_regional_workflow.cpp
)
target_link_libraries(IntegrationTests ${GTEST_LIBRARIES} pthread ClearBallotServiceLib ElectionLib)

# Register tests with CTest
add_test(NAME CoreUnitTests COMMAND CoreTests)
add_test(NAME ServiceUnitTests COMMAND ServiceTests)
add_test(NAME ModelUnitTests COMMAND ModelTests)
add_test(NAME IntegrationTests COMMAND IntegrationTests)
```

### **Continuous Integration Pipeline**
```yaml
# .github/workflows/ci.yml
name: ClearBallot CI

on: [push, pull_request]

jobs:
  test:
    runs-on: [ubuntu-latest, windows-latest]
    steps:
    - uses: actions/checkout@v3
    
    - name: Install Google Test
      run: |
        # Ubuntu
        sudo apt-get install libgtest-dev libgmock-dev
        # Windows (vcpkg)
        vcpkg install gtest gmock
    
    - name: Configure CMake
      run: cmake -B build -S . -DBUILD_TESTING=ON
    
    - name: Build
      run: cmake --build build --config Release
    
    - name: Run Tests
      run: |
        cd build
        ctest --output-on-failure --parallel 4
    
    - name: Generate Coverage Report
      run: |
        # Generate code coverage reports
        gcov --coverage build/tests/
```

## 🚀 **Implementation Phases**

### **Phase 1: Service Foundation + Testing Framework (Weeks 1-2)**

#### **1.1: Standalone Service Executable**
**Goal**: Create deployable service that runs independently

**New Components:**
```cpp
// applications/service/service_main.cpp
class ClearBallotServiceExecutable {
    // Main entry point for service
    // Console mode for development
    // Service mode for production
};

// src/services/ServiceCommunication.h
class ServiceCommunication {
    // Named Pipes (Windows) / Unix Domain Sockets (Linux)
    // Request/Response handling
    // Client connection management
};
```

**Deliverables:**
- [ ] `ClearBallotService.exe` - Standalone service executable
- [ ] Named Pipe communication layer
- [ ] Service installation/uninstallation utilities
- [ ] Development console mode for testing

#### **1.2: Inter-Process Communication**
**Goal**: Enable admin/voter tools to communicate with running service

**Communication Protocol:**
```json
{
  "request_type": "register_voter",
  "election_id": 1,
  "data": {
    "first_name": "John",
    "last_name": "Doe",
    "phone": "1234567890",
    "address": "123 Main St",
    "unique_id": "123456789",
    "age": "25",
    "region_id": 1
  }
}
```

**Response Format:**
```json
{
  "success": true,
  "message": "Voter registered successfully",
  "data": {
    "voter_id": 42,
    "registration_time": "2025-08-30T10:30:00Z"
  }
}
```

### **Phase 2: Database-Ready Data Models (Weeks 3-4)**

#### **2.1: Relational Data Structures**
**Goal**: Prepare data models that map directly to database tables

**Core Tables Design:**
```sql
-- Elections table
CREATE TABLE elections (
    election_id INTEGER PRIMARY KEY AUTOINCREMENT,
    title VARCHAR(255) NOT NULL,
    status VARCHAR(20) DEFAULT 'ACTIVE',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Regions table
CREATE TABLE regions (
    region_id INTEGER PRIMARY KEY AUTOINCREMENT,
    election_id INTEGER REFERENCES elections(election_id),
    name VARCHAR(255) NOT NULL,
    code VARCHAR(50) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Parties table
CREATE TABLE parties (
    party_id INTEGER PRIMARY KEY AUTOINCREMENT,
    election_id INTEGER REFERENCES elections(election_id),
    name VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Candidates table
CREATE TABLE candidates (
    candidate_id INTEGER PRIMARY KEY AUTOINCREMENT,
    election_id INTEGER REFERENCES elections(election_id),
    region_id INTEGER REFERENCES regions(region_id),
    party_id INTEGER REFERENCES parties(party_id),
    name VARCHAR(255) NOT NULL,
    vote_count INTEGER DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Voters table
CREATE TABLE voters (
    voter_id INTEGER PRIMARY KEY AUTOINCREMENT,
    election_id INTEGER REFERENCES elections(election_id),
    region_id INTEGER REFERENCES regions(region_id),
    unique_voter_id VARCHAR(9) UNIQUE NOT NULL,
    first_name VARCHAR(100) NOT NULL,
    last_name VARCHAR(100) NOT NULL,
    phone_number VARCHAR(10) NOT NULL,
    address VARCHAR(500) NOT NULL,
    age INTEGER NOT NULL,
    has_voted BOOLEAN DEFAULT FALSE,
    registered_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    voted_at TIMESTAMP NULL
);

-- Vote Records table
CREATE TABLE vote_records (
    vote_id INTEGER PRIMARY KEY AUTOINCREMENT,
    voter_id INTEGER REFERENCES voters(voter_id),
    candidate_id INTEGER REFERENCES candidates(candidate_id),
    election_id INTEGER REFERENCES elections(election_id),
    region_id INTEGER REFERENCES regions(region_id),
    cast_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    audit_hash VARCHAR(64) NOT NULL
);
```

#### **2.2: C++ Data Models**
**Goal**: Create C++ structures that mirror database schema

```cpp
// src/core/models/ElectionRecord.h
struct ElectionRecord {
    int election_id = -1;                           // -1 = not persisted
    std::string title;
    std::string status = "ACTIVE";                  // ACTIVE, CLOSED, ARCHIVED
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    
    // JSON serialization for IPC
    nlohmann::json toJSON() const;
    static ElectionRecord fromJSON(const nlohmann::json& j);
    
    // Database preparation
    std::map<std::string, std::string> toDatabaseFields() const;
};

// src/core/models/VoterRecord.h
struct VoterRecord {
    int voter_id = -1;                              // Auto-increment PK
    int election_id;                                // FK to elections
    int region_id;                                  // FK to regions
    std::string unique_voter_id;                    // Business key (9 digits)
    std::string first_name;                         // Validated input
    std::string last_name;                          // Validated input
    std::string phone_number;                       // Exactly 10 digits
    std::string address;                            // Minimum 5 characters
    int age;                                        // 18+ validation
    bool has_voted = false;
    std::chrono::system_clock::time_point registered_at;
    std::optional<std::chrono::system_clock::time_point> voted_at;
    
    // Validation using existing InputValidator
    bool isValid() const;
    nlohmann::json toJSON() const;
    static VoterRecord fromJSON(const nlohmann::json& j);
};

// src/core/models/VoteRecord.h
struct VoteRecord {
    int vote_id = -1;                               // Auto-increment PK
    int voter_id;                                   // FK to voters
    int candidate_id;                               // FK to candidates
    int election_id;                                // FK to elections
    int region_id;                                  // FK to regions
    std::chrono::system_clock::time_point cast_at;
    std::string audit_hash;                         // SHA-256 for integrity
    
    // Audit trail
    std::string generateAuditHash() const;
    bool verifyIntegrity() const;
    nlohmann::json toJSON() const;
};
```

### **Phase 3: Windows Service Integration (Weeks 5-6)**

#### **3.1: Windows Service Registration**
**Goal**: Register ClearBallot as official Windows Service

```cpp
// src/services/WindowsService.h
class ClearBallotWindowsService {
public:
    // Service Control Manager interface
    static void WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
    static void WINAPI ServiceCtrlHandler(DWORD ctrl);
    
    // Installation utilities
    bool InstallService(const std::string& serviceName = "ClearBallotService");
    bool UninstallService(const std::string& serviceName = "ClearBallotService");
    bool StartService();
    bool StopService();
    
    // Service lifecycle
    void Run();
    void OnStart();
    void OnStop();
    void OnPause();
    void OnContinue();
    
private:
    static SERVICE_STATUS serviceStatus;
    static SERVICE_STATUS_HANDLE serviceStatusHandle;
    std::unique_ptr<ClearBallotService> coreService;
    std::unique_ptr<ServiceCommunication> communicationLayer;
};
```

**Service Configuration:**
```ini
Service Name: ClearBallotService
Display Name: ClearBallot Election Management Service
Description: Provides secure election management and voting services
Start Type: Automatic (Delayed Start)
Account: Local System
Dependencies: None
```

#### **3.2: Linux Daemon Support**
**Goal**: Support Linux daemon deployment

```bash
# /etc/systemd/system/clearballot.service
[Unit]
Description=ClearBallot Election Management Service
After=network.target

[Service]
Type=simple
User=clearballot
Group=clearballot
ExecStart=/usr/local/bin/ClearBallotService
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
```

## 📊 **Data Migration Strategy**

### **Current File-Based Storage:**
```cpp
// Current pattern (to be deprecated)
void Election::saveResultsToFile(const std::string& filename) {
    std::ofstream file(filename);
    file << "Election: " << title << "\n";
    // ... text-based output
}
```

### **New Structured Storage:**
```cpp
// New pattern (database-ready)
class ElectionDataManager {
public:
    std::vector<ElectionRecord> getAllElections();
    std::vector<VoterRecord> getVotersByElection(int electionId);
    std::vector<VoteRecord> getVotesByElection(int electionId);
    
    // File-based implementation (Phase 2)
    bool saveElectionData(const ElectionRecord& election);
    bool loadElectionData(int electionId);
    
    // Database implementation (Future Phase 4)
    // bool saveToDatabase(const ElectionRecord& election);
    // bool loadFromDatabase(int electionId);
};
```

### **Migration Benefits:**
1. **Queryable Data**: Structured queries instead of text parsing
2. **Relational Integrity**: Foreign key relationships maintained
3. **Audit Trail**: Complete voting history with timestamps
4. **Performance**: Indexed database access vs file I/O
5. **Concurrency**: Database transactions vs file locking

## 🔧 **Implementation Timeline**

### **Week 1: Service Foundation + Testing Setup**
- [ ] **Testing Framework**: Set up Google Test and Google Mock
- [ ] **Core Unit Tests**: Create tests for existing Election, Candidate, Voter classes
- [ ] **Regional Tests**: Comprehensive testing of all 3 regional restrictions
- [ ] **Service Executable**: Create `applications/service/service_main.cpp`
- [ ] **Service Tests**: Unit tests for ClearBallotService thread safety

### **Week 2: IPC Communication + Integration Tests**
- [ ] **IPC Implementation**: Named Pipe communication layer
- [ ] **IPC Testing**: Mock-based testing of request/response serialization
- [ ] **Integration Tests**: Test admin app connecting to standalone service
- [ ] **Performance Tests**: Benchmark IPC vs in-process performance
- [ ] **Continuous Integration**: Set up automated testing pipeline

### **Week 3: Data Models + Model Testing**
- [ ] **Database Models**: Create database-ready record structures
- [ ] **Model Unit Tests**: Test JSON serialization/deserialization
- [ ] **Validation Testing**: Comprehensive InputValidator test coverage
- [ ] **Data Migration**: Replace Election object usage with structured records
- [ ] **Model Integration**: Test data persistence and retrieval with mocks

### **Week 4: Client Migration + End-to-End Testing**
- [ ] **Admin App Migration**: Update to use IPC instead of in-process service
- [ ] **Voter App Migration**: Update to use IPC instead of in-process service
- [ ] **End-to-End Tests**: Complete workflow testing (register → vote → results)
- [ ] **Regression Testing**: Ensure all existing functionality preserved
- [ ] **Performance Validation**: Compare IPC vs original performance

### **Week 5: Windows Service + Service Testing**
- [ ] **Windows Service**: Implement Windows Service registration
- [ ] **Service Lifecycle Tests**: Test installation/uninstallation/start/stop
- [ ] **Service Integration**: Test service start/stop via Services panel
- [ ] **Error Handling**: Add proper logging and monitoring
- [ ] **Configuration**: Add configuration file support

### **Week 6: Polish + Comprehensive Testing**
- [ ] **Linux Daemon**: Support for Linux daemon deployment
- [ ] **Load Testing**: Concurrent client testing and performance benchmarks
- [ ] **Security Testing**: Validate service permissions and IPC security
- [ ] **Documentation**: Update all guides with testing procedures
- [ ] **CI/CD Pipeline**: Complete automated testing and deployment

## ⚠️ **Risk Assessment & Mitigation**

### **Risk 1: IPC Complexity**
**Impact**: Medium
**Mitigation**: Start with simple Named Pipes, extensive testing, fallback to existing in-process mode

### **Risk 2: Service Permissions**
**Impact**: Medium  
**Mitigation**: Proper service account configuration, file system permissions testing

### **Risk 3: Data Model Changes**
**Impact**: Low
**Mitigation**: Maintain backward compatibility, gradual migration, comprehensive testing

### **Risk 4: Performance Degradation**
**Impact**: Low
**Mitigation**: IPC benchmarking, async communication patterns, connection pooling

## 🎯 **Success Criteria**

### **Phase 1 Success Metrics:**
- [ ] **Service Deployment**: Service runs as standalone executable
- [ ] **IPC Communication**: Admin/Voter tools connect via IPC
- [ ] **Functionality Preservation**: All existing functionality preserved
- [ ] **Service Management**: Service can be installed/uninstalled
- [ ] **Concurrent Access**: Multiple client connections supported
- [ ] **Test Coverage**: >90% code coverage with comprehensive unit tests
- [ ] **Performance**: IPC performance within 10% of in-process performance

### **Phase 2 Success Metrics:**
- [ ] **Structured Data**: Data stored in database-ready format
- [ ] **Serialization**: JSON serialization/deserialization working perfectly
- [ ] **Data Access**: Query-like data access patterns implemented
- [ ] **Migration Complete**: Migration from file-based storage complete
- [ ] **Performance Maintained**: Performance maintained or improved
- [ ] **Model Testing**: 100% test coverage for all data models
- [ ] **Integration Testing**: End-to-end workflow testing passing

### **Final Success Criteria:**
- [ ] **OS Integration**: Service appears in Windows Services panel
- [ ] **Auto-Start**: Service starts automatically on boot
- [ ] **Tool Integration**: Admin/Voter tools work with registered service
- [ ] **Business Rules**: All regional voting restrictions maintained
- [ ] **Audit Trail**: Complete audit trail available and tested
- [ ] **Database Ready**: Ready for database integration (Phase 4)
- [ ] **Test Suite**: Comprehensive test suite with CI/CD integration
- [ ] **Performance**: Production-ready performance characteristics

## 📚 **Next Phase Preview: Database Integration**

### **Phase 4: Database Integration (Future)**
Once service deployment is complete, the structured data models will enable seamless database integration:

```cpp
// Future database layer
class DatabaseManager {
public:
    // SQLite for development
    bool initializeSQLiteDatabase(const std::string& dbPath);
    
    // PostgreSQL for production
    bool initializePostgreSQLDatabase(const std::string& connectionString);
    
    // CRUD operations using existing data models
    int insertElection(const ElectionRecord& election);
    std::vector<VoterRecord> selectVotersByRegion(int regionId);
    bool updateVoteCount(int candidateId);
    bool deleteElection(int electionId);
};
```

The groundwork laid in Phases 1-3 makes database integration straightforward, with data models that map directly to database tables and service architecture that supports database transactions.

---

**This plan leverages ClearBallot's excellent existing architecture while establishing enterprise-grade foundations for production deployment and future scalability.**
