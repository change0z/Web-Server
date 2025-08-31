#include "Election.h"
#include <iostream>
#include <cassert>
#include <memory>
#include <functional>

// Simple custom test framework
class TestRunner {
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
        std::cout << "\n=== Test Summary ===\n";
        std::cout << "Tests run: " << testsRun << "\n";
        std::cout << "Tests passed: " << testsPassed << "\n";
        std::cout << "Tests failed: " << (testsRun - testsPassed) << "\n";
        std::cout << "Success rate: " << (testsRun > 0 ? (testsPassed * 100.0 / testsRun) : 0) << "%\n";
    }
    
    bool allTestsPassed() const {
        return testsRun > 0 && testsPassed == testsRun;
    }
};

// Election Core Tests
class ElectionCoreTests {
public:
    static bool testElectionCreation() {
        Election election("Test Election 2024");
        return election.getTitle() == "Test Election 2024";
    }
    
    static bool testPartyCreation() {
        Election election("Test Election");
        auto party = election.createParty("Test Party");
        return party != nullptr && party->getName() == "Test Party";
    }
    
    static bool testRegionCreation() {
        Election election("Test Election");
        auto region = election.createRegion("Test Region", "TEST");
        return region != nullptr && region->getName() == "Test Region" && region->getCode() == "TEST";
    }
    
    static bool testVoterRegistration() {
        Election election("Test Election");
        bool result = election.registerVoter("John", "Doe", "1234567890", "123 Main St", "123456789", "25");
        return result;
    }
    
    static bool testCandidateAddition() {
        Election election("Test Election");
        // Test legacy method
        election.addCandidate("Test Candidate");
        return true; // If no exception thrown, test passes
    }
    
    static bool testVoterRegistrationValidation() {
        Election election("Test Election");
        
        // Test valid voter
        bool validResult = election.registerVoter("Alice", "Smith", "9876543210", "456 Oak St", "987654321", "30");
        
        // Test invalid phone number (too short)
        bool invalidResult = election.registerVoter("Bob", "Jones", "123", "789 Pine St", "555666777", "25");
        
        return validResult && !invalidResult;
    }
    
    static bool testDuplicateVoterPrevention() {
        Election election("Test Election");
        
        // Register voter first time
        bool firstRegistration = election.registerVoter("Charlie", "Brown", "5555551234", "321 Elm St", "111222333", "22");
        
        // Try to register same voter ID again
        bool secondRegistration = election.registerVoter("Different", "Name", "5555559999", "999 Different St", "111222333", "35");
        
        return firstRegistration && !secondRegistration;
    }
    
    static bool testVoterRegistrationCheck() {
        Election election("Test Election");
        
        // Register a voter
        election.registerVoter("Diana", "Prince", "7778889999", "Wonder Ave", "444555666", "28");
        
        // Check if voter is registered
        bool isRegistered = election.isVoterRegistered(444555666);
        bool isNotRegistered = election.isVoterRegistered(999888777);
        
        return isRegistered && !isNotRegistered;
    }
};

// Regional Voting Tests
class RegionalVotingTests {
public:
    static bool testRegionalCandidateAddition() {
        Election election("Regional Test Election");
        
        auto region1 = election.createRegion("North District", "NORTH");
        auto region2 = election.createRegion("South District", "SOUTH");
        auto party = election.createParty("Test Party");
        
        // Add candidate to first region
        bool result1 = election.addCandidateToRegion("Alice Johnson", party, region1);
        
        // Try to add same candidate to second region (should fail)
        bool result2 = election.addCandidateToRegion("Alice Johnson", party, region2);
        
        return result1 && !result2; // First should succeed, second should fail
    }
    
    static bool testPartyLimitPerRegion() {
        Election election("Regional Test Election");
        
        auto region = election.createRegion("Test District", "TEST");
        auto party = election.createParty("Test Party");
        
        // Add first candidate from party to region
        bool result1 = election.addCandidateToRegion("Candidate One", party, region);
        
        // Try to add second candidate from same party to same region (should fail)
        bool result2 = election.addCandidateToRegion("Candidate Two", party, region);
        
        return result1 && !result2;
    }
    
    static bool testRegionalVoterRegistration() {
        Election election("Regional Test Election");
        
        auto region = election.createRegion("Test District", "TEST");
        
        bool result = election.registerVoterInRegion("John", "Doe", "1234567890", "123 Main St", "123456789", "25", region);
        
        return result;
    }
    
    static bool testCrossRegionVotingPrevention() {
        Election election("Regional Test Election");
        
        auto region1 = election.createRegion("North District", "NORTH");
        auto region2 = election.createRegion("South District", "SOUTH");
        auto party = election.createParty("Test Party");
        
        // Add candidates to regions
        election.addCandidateToRegion("North Candidate", party, region1);
        election.addCandidateToRegion("South Candidate", party, region2);
        
        // Register voter in region1
        election.registerVoterInRegion("Voter", "One", "1111111111", "North Address", "111111111", "25", region1);
        
        // Try to vote in region2 (should fail)
        bool crossRegionVote = election.castVoteInRegion(111111111, 0, region2);
        
        return !crossRegionVote; // Should fail
    }
};

// Input Validation Tests
class ValidationTests {
public:
    static bool testPhoneValidation() {
        Election election("Validation Test");
        
        // Valid phone number
        bool validPhone = election.registerVoter("Valid", "User", "1234567890", "123 Main St", "123456789", "25");
        
        // Invalid phone numbers
        bool tooShort = election.registerVoter("Invalid", "Short", "123", "123 Main St", "123456788", "25");
        bool tooLong = election.registerVoter("Invalid", "Long", "12345678901", "123 Main St", "123456787", "25");
        bool nonNumeric = election.registerVoter("Invalid", "Alpha", "123abc7890", "123 Main St", "123456786", "25");
        
        return validPhone && !tooShort && !tooLong && !nonNumeric;
    }
    
    static bool testIdValidation() {
        Election election("Validation Test");
        
        // Valid ID (9 digits)
        bool validId = election.registerVoter("Valid", "User", "1234567890", "123 Main St", "123456789", "25");
        
        // Invalid IDs
        bool tooShort = election.registerVoter("Invalid", "Short", "1234567891", "123 Main St", "12345", "25");
        bool tooLong = election.registerVoter("Invalid", "Long", "1234567892", "123 Main St", "1234567890", "25");
        
        return validId && !tooShort && !tooLong;
    }
    
    static bool testAgeValidation() {
        Election election("Validation Test");
        
        // Valid age (18+)
        bool validAge = election.registerVoter("Valid", "Adult", "1234567890", "123 Main St", "123456789", "25");
        
        // Invalid age (under 18)
        bool underAge = election.registerVoter("Invalid", "Minor", "1234567891", "123 Main St", "123456788", "17");
        
        return validAge && !underAge;
    }
    
    static bool testNameValidation() {
        Election election("Validation Test");
        
        // Valid names
        bool validName = election.registerVoter("Alice", "Johnson", "1234567890", "123 Main St", "123456789", "25");
        
        // Invalid names (too short)
        bool tooShort = election.registerVoter("A", "B", "1234567891", "123 Main St", "123456788", "25");
        
        return validName && !tooShort;
    }
};

int main() {
    std::cout << "=== ClearBallot Core Components Unit Tests ===\n\n";
    
    TestRunner runner;
    
    // Election Core Tests
    std::cout << "--- Election Core Tests ---\n";
    runner.runTest("Election Creation", ElectionCoreTests::testElectionCreation);
    runner.runTest("Party Creation", ElectionCoreTests::testPartyCreation);
    runner.runTest("Region Creation", ElectionCoreTests::testRegionCreation);
    runner.runTest("Voter Registration", ElectionCoreTests::testVoterRegistration);
    runner.runTest("Candidate Addition", ElectionCoreTests::testCandidateAddition);
    runner.runTest("Voter Registration Validation", ElectionCoreTests::testVoterRegistrationValidation);
    runner.runTest("Duplicate Voter Prevention", ElectionCoreTests::testDuplicateVoterPrevention);
    runner.runTest("Voter Registration Check", ElectionCoreTests::testVoterRegistrationCheck);
    
    // Regional Voting Tests
    std::cout << "\n--- Regional Voting Tests ---\n";
    runner.runTest("Regional Candidate Addition", RegionalVotingTests::testRegionalCandidateAddition);
    runner.runTest("Party Limit Per Region", RegionalVotingTests::testPartyLimitPerRegion);
    runner.runTest("Regional Voter Registration", RegionalVotingTests::testRegionalVoterRegistration);
    runner.runTest("Cross-Region Voting Prevention", RegionalVotingTests::testCrossRegionVotingPrevention);
    
    // Input Validation Tests
    std::cout << "\n--- Input Validation Tests ---\n";
    runner.runTest("Phone Number Validation", ValidationTests::testPhoneValidation);
    runner.runTest("ID Validation", ValidationTests::testIdValidation);
    runner.runTest("Age Validation", ValidationTests::testAgeValidation);
    runner.runTest("Name Validation", ValidationTests::testNameValidation);
    
    runner.printSummary();
    
    return runner.allTestsPassed() ? 0 : 1;
}
