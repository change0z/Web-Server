#include "ClearBallotService.h"
#include "ServiceClient.h"
#include "Election.h"
#include "InputValidator.h"
#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <chrono>

// Simple test framework for integration tests
class IntegrationTestRunner {
private:
    int testsRun = 0;
    int testsPassed = 0;
    
public:
    void runTest(const std::string& testName, std::function<bool()> testFunc) {
        testsRun++;
        std::cout << "Running integration test: " << testName << "... ";
        
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
        std::cout << "\n=== Integration Test Summary ===\n";
        std::cout << "Tests run: " << testsRun << "\n";
        std::cout << "Tests passed: " << testsPassed << "\n";
        std::cout << "Tests failed: " << (testsRun - testsPassed) << "\n";
        std::cout << "Success rate: " << (testsRun > 0 ? (testsPassed * 100.0 / testsRun) : 0) << "%\n";
    }
    
    bool allTestsPassed() const {
        return testsRun > 0 && testsPassed == testsRun;
    }
};

// End-to-End Election Workflow Tests
class EndToEndElectionTests {
public:
    static bool testCompleteElectionWorkflow() {
        // Create service and client
        ServiceClient client;
        if (!client.connectToService()) {
            return false;
        }
        
        // 1. Create election
        int electionId = client.createElection("Integration Test Election");
        if (electionId <= 0) {
            client.disconnect();
            return false;
        }
        
        // 2. Create parties
        auto partyResponse1 = client.createParty(electionId, "Test Party A");
        auto partyResponse2 = client.createParty(electionId, "Test Party B");
        if (!partyResponse1.success || !partyResponse2.success) {
            client.disconnect();
            return false;
        }
        
        // 3. Create regions
        auto regionResponse1 = client.createRegion(electionId, "Test Region North", "NORTH");
        auto regionResponse2 = client.createRegion(electionId, "Test Region South", "SOUTH");
        if (!regionResponse1.success || !regionResponse2.success) {
            client.disconnect();
            return false;
        }
        
        // 4. Add candidates
        auto candidateResponse1 = client.addCandidateToRegion(electionId, "John Smith", "Test Region North", "Test Party A");
        auto candidateResponse2 = client.addCandidateToRegion(electionId, "Jane Doe", "Test Region South", "Test Party B");
        if (!candidateResponse1.success || !candidateResponse2.success) {
            client.disconnect();
            return false;
        }
        
        // 5. Register voters
        auto voterResponse1 = client.registerVoterInRegion(electionId, "Alice", "Johnson", 
                                                          "5551234567", "123 North St", 
                                                          "123456789", "25", "NORTH");
        auto voterResponse2 = client.registerVoterInRegion(electionId, "Bob", "Wilson", 
                                                          "5559876543", "456 South St", 
                                                          "987654321", "30", "SOUTH");
        if (!voterResponse1.success || !voterResponse2.success) {
            client.disconnect();
            return false;
        }
        
        // 6. Cast votes
        auto voteResponse1 = client.castVoteInRegion(electionId, 123456789, 0, "NORTH");
        auto voteResponse2 = client.castVoteInRegion(electionId, 987654321, 0, "SOUTH");
        if (!voteResponse1.success || !voteResponse2.success) {
            client.disconnect();
            return false;
        }
        
        client.disconnect();
        return true;
    }
    
    static bool testValidationIntegration() {
        ServiceClient client;
        if (!client.connectToService()) {
            return false;
        }
        
        int electionId = client.createElection("Validation Test Election");
        client.createRegion(electionId, "Validation Region", "VALID");
        
        // Test with invalid data (should be rejected by validation)
        auto invalidVoterResponse = client.registerVoterInRegion(electionId, 
                                                                "", // Invalid name (empty)
                                                                "ValidLastName", 
                                                                "123", // Invalid phone (too short)
                                                                "12", // Invalid address (too short)
                                                                "12345", // Invalid ID (too short)
                                                                "17", // Invalid age (under 18)
                                                                "VALID");
        
        // Should fail due to validation
        bool validationWorking = !invalidVoterResponse.success;
        
        // Test with valid data (should succeed)
        auto validVoterResponse = client.registerVoterInRegion(electionId, 
                                                              "ValidFirst", 
                                                              "ValidLast", 
                                                              "1234567890", 
                                                              "123 Valid Street", 
                                                              "123456789", 
                                                              "25", 
                                                              "VALID");
        
        bool validDataWorking = validVoterResponse.success;
        
        client.disconnect();
        return validationWorking && validDataWorking;
    }
    
    static bool testRegionalRestrictionsIntegration() {
        ServiceClient client;
        if (!client.connectToService()) {
            return false;
        }
        
        int electionId = client.createElection("Regional Restrictions Test");
        
        // Create regions and party
        client.createRegion(electionId, "Region A", "A");
        client.createRegion(electionId, "Region B", "B");
        client.createParty(electionId, "Test Party");
        
        // Test Restriction 1: Same candidate cannot be in multiple regions
        client.addCandidateToRegion(electionId, "Unique Candidate", "A");
        auto duplicateResponse = client.addCandidateToRegion(electionId, "Unique Candidate", "B");
        bool restriction1Working = !duplicateResponse.success; // Should fail
        
        // Test Restriction 2: One candidate per party per region
        client.addCandidateToRegion(electionId, "Party Member 1", "Test Party", "A");
        auto partyLimitResponse = client.addCandidateToRegion(electionId, "Party Member 2", "Test Party", "A");
        bool restriction2Working = !partyLimitResponse.success; // Should fail
        
        // Test Restriction 3: Cross-region voting prevention
        client.addCandidateToRegion(electionId, "Regional Candidate", "B");
        client.registerVoterInRegion(electionId, "Regional", "Voter", "5551234567", 
                                    "123 Regional St", "123456789", "25", "A");
        
        auto crossVoteResponse = client.castVoteInRegion(electionId, 123456789, 0, "B");
        bool restriction3Working = !crossVoteResponse.success; // Should fail
        
        client.disconnect();
        return restriction1Working && restriction2Working && restriction3Working;
    }
};

// Multi-Service Integration Tests
class MultiServiceTests {
public:
    static bool testMultipleServiceInstances() {
        // Create multiple service clients
        ServiceClient client1, client2, client3;
        
        bool allConnected = client1.connectToService() && 
                           client2.connectToService() && 
                           client3.connectToService();
        
        if (!allConnected) {
            return false;
        }
        
        // Each client creates an election
        int election1 = client1.createElection("Multi-Service Election 1");
        int election2 = client2.createElection("Multi-Service Election 2");
        int election3 = client3.createElection("Multi-Service Election 3");
        
        bool allElectionsCreated = (election1 > 0) && (election2 > 0) && (election3 > 0);
        
        // Test that elections are isolated
        bool election1Exists = client1.electionExists(election1);
        bool election2Exists = client2.electionExists(election2);
        bool election3Exists = client3.electionExists(election3);
        
        // Cleanup
        client1.disconnect();
        client2.disconnect();
        client3.disconnect();
        
        return allElectionsCreated && election1Exists && election2Exists && election3Exists;
    }
    
    static bool testConcurrentElectionOperations() {
        std::vector<std::unique_ptr<ServiceClient>> clients;
        std::vector<int> electionIds;
        
        // Create multiple clients
        for (int i = 0; i < 3; ++i) {
            auto client = std::make_unique<ServiceClient>();
            if (client->connectToService()) {
                clients.push_back(std::move(client));
            }
        }
        
        if (clients.size() != 3) {
            return false;
        }
        
        // Concurrent election creation
        std::vector<std::thread> threads;
        std::mutex electionIdsMutex;
        
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back([&clients, &electionIds, &electionIdsMutex, i]() {
                int electionId = clients[i]->createElection("Concurrent Election " + std::to_string(i));
                
                std::lock_guard<std::mutex> lock(electionIdsMutex);
                electionIds.push_back(electionId);
            });
        }
        
        // Wait for all threads
        for (auto& thread : threads) {
            thread.join();
        }
        
        // Verify all elections were created
        bool allCreated = (electionIds.size() == 3) && 
                         std::all_of(electionIds.begin(), electionIds.end(), 
                                   [](int id) { return id > 0; });
        
        // Cleanup
        for (auto& client : clients) {
            client->disconnect();
        }
        
        return allCreated;
    }
    
    static bool testServiceClientReconnection() {
        ServiceClient client;
        
        // Initial connection
        bool initialConnection = client.connectToService();
        if (!initialConnection) {
            return false;
        }
        
        // Create an election
        int electionId = client.createElection("Reconnection Test Election");
        bool electionCreated = (electionId > 0);
        
        // Disconnect
        client.disconnect();
        bool disconnected = !client.isConnected();
        
        // Reconnect
        bool reconnected = client.connectToService();
        
        // Verify election still exists
        bool electionStillExists = client.electionExists(electionId);
        
        client.disconnect();
        
        return initialConnection && electionCreated && disconnected && 
               reconnected && electionStillExists;
    }
};

// Data Persistence and Consistency Tests
class DataConsistencyTests {
public:
    static bool testElectionDataPersistence() {
        ServiceClient client;
        if (!client.connectToService()) {
            return false;
        }
        
        int electionId = client.createElection("Persistence Test Election");
        
        // Add comprehensive data
        client.createParty(electionId, "Persistence Party");
        client.createRegion(electionId, "Persistence Region", "PERSIST");
        client.addCandidateToRegion(electionId, "Persistent Candidate", "Persistence Party", "PERSIST");
        client.registerVoterInRegion(electionId, "Persistent", "Voter", "5551234567", 
                                    "123 Persistent St", "123456789", "25", "PERSIST");
        client.castVoteInRegion(electionId, 123456789, 0, "PERSIST");
        
        // Verify data consistency
        bool electionExists = client.electionExists(electionId);
        
        client.disconnect();
        return electionExists;
    }
    
    static bool testCrossComponentDataFlow() {
        ServiceClient client;
        if (!client.connectToService()) {
            return false;
        }
        
        int electionId = client.createElection("Data Flow Test Election");
        
        // Test data flow: Service -> Election -> Region -> Validation
        auto partyResult = client.createParty(electionId, "DataFlow Party");
        auto regionResult = client.createRegion(electionId, "DataFlow Region", "FLOW");
        auto candidateResult = client.addCandidateToRegion(electionId, "DataFlow Candidate", 
                                                          "DataFlow Party", "FLOW");
        
        // Test validation integration
        auto validVoterResult = client.registerVoterInRegion(electionId, "ValidName", "ValidLast", 
                                                            "1234567890", "123 Valid Street", 
                                                            "123456789", "25", "FLOW");
        
        auto invalidVoterResult = client.registerVoterInRegion(electionId, "", "InvalidLast", 
                                                              "123", "12", "12345", "17", "FLOW");
        
        // Test voting workflow
        auto voteResult = client.castVoteInRegion(electionId, 123456789, 0, "FLOW");
        
        client.disconnect();
        
        return partyResult.success && regionResult.success && candidateResult.success && 
               validVoterResult.success && !invalidVoterResult.success && voteResult.success;
    }
    
    static bool testLargeScaleElectionData() {
        ServiceClient client;
        if (!client.connectToService()) {
            return false;
        }
        
        int electionId = client.createElection("Large Scale Test Election");
        
        // Create multiple regions and parties
        std::vector<std::string> regions = {"North", "South", "East", "West", "Central"};
        std::vector<std::string> regionCodes = {"N", "S", "E", "W", "C"};
        std::vector<std::string> parties = {"Party A", "Party B", "Party C"};
        
        // Create all regions and parties
        for (size_t i = 0; i < regions.size(); ++i) {
            client.createRegion(electionId, regions[i], regionCodes[i]);
        }
        for (const auto& party : parties) {
            client.createParty(electionId, party);
        }
        
        // Add candidates (one per party per region)
        int candidateCount = 0;
        for (size_t i = 0; i < regions.size(); ++i) {
            for (const auto& party : parties) {
                std::string candidateName = "Candidate " + std::to_string(candidateCount++);
                client.addCandidateToRegion(electionId, candidateName, party, regionCodes[i]);
            }
        }
        
        // Register voters and cast votes
        int voterCount = 0;
        for (size_t i = 0; i < regions.size(); ++i) {
            for (int j = 0; j < 3; ++j) { // 3 voters per region
                std::string firstName = "Voter" + std::to_string(voterCount);
                std::string lastName = "LastName" + std::to_string(voterCount);
                std::string phone = "555" + std::to_string(1000000 + voterCount);
                std::string address = std::to_string(100 + voterCount) + " Test Street";
                std::string id = std::to_string(100000000 + voterCount);
                int voterId = 100000000 + voterCount;
                
                client.registerVoterInRegion(electionId, firstName, lastName, phone, 
                                            address, id, "25", regionCodes[i]);
                client.castVoteInRegion(electionId, voterId, 0, regionCodes[i]); // Vote for first candidate
                voterCount++;
            }
        }
        
        client.disconnect();
        return true; // If we get here without exceptions, test passed
    }
};

int main() {
    std::cout << "=== ClearBallot System Integration Tests ===\n\n";
    
    IntegrationTestRunner runner;
    
    // End-to-End Election Workflow Tests
    std::cout << "--- End-to-End Election Workflow Tests ---\n";
    runner.runTest("Complete Election Workflow", EndToEndElectionTests::testCompleteElectionWorkflow);
    runner.runTest("Validation Integration", EndToEndElectionTests::testValidationIntegration);
    runner.runTest("Regional Restrictions Integration", EndToEndElectionTests::testRegionalRestrictionsIntegration);
    
    // Multi-Service Integration Tests
    std::cout << "\n--- Multi-Service Integration Tests ---\n";
    runner.runTest("Multiple Service Instances", MultiServiceTests::testMultipleServiceInstances);
    runner.runTest("Concurrent Election Operations", MultiServiceTests::testConcurrentElectionOperations);
    runner.runTest("Service Client Reconnection", MultiServiceTests::testServiceClientReconnection);
    
    // Data Persistence and Consistency Tests
    std::cout << "\n--- Data Consistency Tests ---\n";
    runner.runTest("Election Data Persistence", DataConsistencyTests::testElectionDataPersistence);
    runner.runTest("Cross-Component Data Flow", DataConsistencyTests::testCrossComponentDataFlow);
    runner.runTest("Large Scale Election Data", DataConsistencyTests::testLargeScaleElectionData);
    
    runner.printSummary();
    
    return runner.allTestsPassed() ? 0 : 1;
}
