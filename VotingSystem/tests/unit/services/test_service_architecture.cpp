#include "ClearBallotService.h"
#include "ServiceClient.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>
#include <functional>

// Simple test framework
class ServiceTestRunner {
private:
    int testsRun = 0;
    int testsPassed = 0;
    
public:
    void runTest(const std::string& testName, std::function<bool()> testFunc) {
        testsRun++;
        std::cout << "Running test: " << testName << "... ";
        
        try {
            if (testFunc()) {
                testsPassed++;
                std::cout << "PASSED\n";
            } else {
                std::cout << "FAILED\n";
            }
        } catch (const std::exception& e) {
            std::cout << "FAILED (Exception: " << e.what() << ")\n";
        }
    }
    
    void printSummary() {
        std::cout << "\n=== Service Test Summary ===\n";
        std::cout << "Tests run: " << testsRun << "\n";
        std::cout << "Tests passed: " << testsPassed << "\n";
        std::cout << "Tests failed: " << (testsRun - testsPassed) << "\n";
        std::cout << "Success rate: " << (testsRun > 0 ? (testsPassed * 100.0 / testsRun) : 0) << "%\n";
    }
    
    bool allTestsPassed() const {
        return testsRun > 0 && testsPassed == testsRun;
    }
};

// Service Architecture Tests
class ServiceArchitectureTests {
public:
    static bool testServiceLifecycle() {
        auto service = std::make_unique<ClearBallotService>();
        
        // Test service start
        bool startResult = service->startService();
        bool isRunning = service->isRunning();
        
        // Test service stop
        bool stopResult = service->stopService();
        bool isStopped = !service->isRunning();
        
        return startResult && isRunning && stopResult && isStopped;
    }
    
    static bool testElectionCreation() {
        auto service = std::make_unique<ClearBallotService>();
        service->startService();
        
        int electionId = service->createElection("Test Election");
        bool exists = service->electionExists(electionId);
        auto election = service->getElection(electionId);
        
        service->stopService();
        
        return electionId > 0 && exists && election != nullptr;
    }
    
    static bool testMultiElectionSupport() {
        auto service = std::make_unique<ClearBallotService>();
        service->startService();
        
        int election1 = service->createElection("Election 1");
        int election2 = service->createElection("Election 2");
        int election3 = service->createElection("Election 3");
        
        auto activeElections = service->getActiveElectionIds();
        
        service->stopService();
        
        return election1 != election2 && 
               election2 != election3 && 
               election1 != election3 &&
               activeElections.size() == 3;
    }
    
    static bool testServiceResponseStructure() {
        auto service = std::make_unique<ClearBallotService>();
        service->startService();
        
        int electionId = service->createElection("Test Election");
        
        // Test successful response
        VoterRegistrationData voterData("John", "Doe", "1234567890", "123 Main St", "123456789", "25");
        
        ServiceResponse response = service->registerVoter(electionId, voterData);
        
        service->stopService();
        
        return response.success == true && !response.message.empty();
    }
    
    static bool testInvalidElectionHandling() {
        auto service = std::make_unique<ClearBallotService>();
        service->startService();
        
        // Test with non-existent election ID
        VoterRegistrationData voterData("Jane", "Smith", "9876543210", "456 Oak St", "987654321", "30");
        
        ServiceResponse response = service->registerVoter(9999, voterData); // Non-existent election
        
        service->stopService();
        
        return response.success == false && !response.message.empty();
    }
    
    static bool testConcurrentElectionAccess() {
        auto service = std::make_unique<ClearBallotService>();
        service->startService();
        
        std::atomic<int> successCount{0};
        std::vector<std::thread> threads;
        
        // Create multiple elections concurrently
        for (int i = 0; i < 5; ++i) {
            threads.emplace_back([&service, &successCount, i]() {
                int electionId = service->createElection("Concurrent Election " + std::to_string(i));
                if (electionId > 0) {
                    successCount++;
                }
            });
        }
        
        // Wait for all threads to complete
        for (auto& thread : threads) {
            thread.join();
        }
        
        service->stopService();
        
        return successCount.load() == 5;
    }
    
    static bool testServiceDelegation() {
        auto service = std::make_unique<ClearBallotService>();
        service->startService();
        
        int electionId = service->createElection("Delegation Test");
        
        // Test that service properly delegates to Election class
        auto response1 = service->addCandidate(electionId, "Test Candidate");
        auto response2 = service->createParty(electionId, "Test Party");
        
        // Get direct access to Election to verify
        auto election = service->getElection(electionId);
        bool hasElection = (election != nullptr);
        
        service->stopService();
        
        return response1.success && response2.success && hasElection;
    }
};

// ServiceClient Tests
class ServiceClientTests {
public:
    static bool testClientConnection() {
        ServiceClient client;
        
        bool connected = client.connectToService();
        bool isConnected = client.isConnected();
        
        if (connected) {
            client.disconnect();
        }
        
        return connected && isConnected;
    }
    
    static bool testClientElectionOperations() {
        ServiceClient client;
        
        if (!client.connectToService()) {
            return false;
        }
        
        int electionId = client.createElection("Client Test Election");
        bool exists = client.electionExists(electionId);
        
        client.disconnect();
        
        return electionId > 0 && exists;
    }
    
    static bool testClientVoterOperations() {
        ServiceClient client;
        
        if (!client.connectToService()) {
            return false;
        }
        
        int electionId = client.createElection("Voter Test Election");
        
        auto response = client.registerVoter(electionId, "Alice", "Johnson", 
                                           "5555551234", "123 Voter St", 
                                           "555666777", "28");
        
        client.disconnect();
        
        return response.success;
    }
    
    static bool testBackwardCompatibility() {
        ServiceClient client;
        
        if (!client.connectToService()) {
            return false;
        }
        
        // Test direct Election access (backward compatibility)
        auto election = client.getElection(-1); // Default election
        bool hasElection = (election != nullptr);
        
        if (hasElection) {
            // Test that we can use Election methods directly
            election->addCandidate("Legacy Candidate");
        }
        
        client.disconnect();
        
        return hasElection;
    }
    
    static bool testMultipleClientConnections() {
        std::vector<std::unique_ptr<ServiceClient>> clients;
        std::atomic<int> connectionCount{0};
        
        // Create multiple clients
        for (int i = 0; i < 3; ++i) {
            auto client = std::make_unique<ServiceClient>();
            if (client->connectToService()) {
                connectionCount++;
                clients.push_back(std::move(client));
            }
        }
        
        // Test that all clients can create elections
        std::atomic<int> electionCount{0};
        for (auto& client : clients) {
            int electionId = client->createElection("Multi-Client Election");
            if (electionId > 0) {
                electionCount++;
            }
        }
        
        // Disconnect all clients
        for (auto& client : clients) {
            client->disconnect();
        }
        
        return connectionCount.load() == 3 && electionCount.load() == 3;
    }
};

int main() {
    std::cout << "=== ClearBallot Service Architecture Tests ===\n\n";
    
    ServiceTestRunner runner;
    
    // Service Architecture Tests
    std::cout << "--- Service Architecture Tests ---\n";
    runner.runTest("Service Lifecycle", ServiceArchitectureTests::testServiceLifecycle);
    runner.runTest("Election Creation", ServiceArchitectureTests::testElectionCreation);
    runner.runTest("Multi-Election Support", ServiceArchitectureTests::testMultiElectionSupport);
    runner.runTest("Service Response Structure", ServiceArchitectureTests::testServiceResponseStructure);
    runner.runTest("Invalid Election Handling", ServiceArchitectureTests::testInvalidElectionHandling);
    runner.runTest("Concurrent Election Access", ServiceArchitectureTests::testConcurrentElectionAccess);
    runner.runTest("Service Delegation", ServiceArchitectureTests::testServiceDelegation);
    
    // ServiceClient Tests
    std::cout << "\n--- ServiceClient Tests ---\n";
    runner.runTest("Client Connection", ServiceClientTests::testClientConnection);
    runner.runTest("Client Election Operations", ServiceClientTests::testClientElectionOperations);
    runner.runTest("Client Voter Operations", ServiceClientTests::testClientVoterOperations);
    runner.runTest("Backward Compatibility", ServiceClientTests::testBackwardCompatibility);
    runner.runTest("Multiple Client Connections", ServiceClientTests::testMultipleClientConnections);
    
    runner.printSummary();
    
    return runner.allTestsPassed() ? 0 : 1;
}
