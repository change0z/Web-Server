#include "../src/services/ServiceClient.h"
#include <iostream>
#include <cassert>

// Simple test program to verify the service infrastructure
int main() {
    std::cout << "=== ClearBallot Service Infrastructure Test ===\n\n";
    
    // Test 1: Service Connection
    std::cout << "Test 1: Service Connection\n";
    ServiceClient client;
    
    bool connected = client.connectToService();
    assert(connected && "Failed to connect to service");
    std::cout << "[PASS] Service connection successful\n\n";
    
    // Test 2: Election Creation  
    std::cout << "Test 2: Election Creation\n";
    int electionId = client.createElection("Test Election 2024");
    assert(electionId > 0 && "Failed to create election");
    std::cout << "[PASS] Election created with ID: " << electionId << "\n\n";
    
    // Test 3: Party Creation
    std::cout << "Test 3: Party Creation\n";
    auto partyResponse = client.createParty(-1, "Test Party");  // -1 uses default election
    assert(partyResponse.success && "Failed to create party");
    std::cout << "[PASS] " << partyResponse.message << "\n\n";
    
    // Test 4: Candidate Addition
    std::cout << "Test 4: Candidate Addition\n";
    auto candidateResponse = client.addCandidateWithParty(-1, "Test Candidate", "Test Party");
    assert(candidateResponse.success && "Failed to add candidate");
    std::cout << "[PASS] " << candidateResponse.message << "\n\n";
    
    // Test 5: Voter Registration
    std::cout << "Test 5: Voter Registration\n";
    auto voterResponse = client.registerVoter(-1, "John", "Doe", "1234567890", 
                                            "123 Main St", "123456789", "25");
    assert(voterResponse.success && "Failed to register voter");
    std::cout << "[PASS] " << voterResponse.message << "\n\n";
    
    // Test 6: Invalid Voter Registration (should fail validation)
    std::cout << "Test 6: Invalid Voter Registration (Expected Failure)\n";
    auto invalidVoterResponse = client.registerVoter(-1, "John123", "Doe", "123", 
                                                    "", "abc", "17");  // Invalid data
    assert(!invalidVoterResponse.success && "Invalid voter registration should fail");
    std::cout << "[PASS] Invalid registration correctly rejected: " << invalidVoterResponse.message << "\n\n";
    
    // Test 7: Vote Casting
    std::cout << "Test 7: Vote Casting\n";
    auto voteResponse = client.castVote(-1, 123456789, 0);  // Vote for first candidate
    assert(voteResponse.success && "Failed to cast vote");
    std::cout << "[PASS] " << voteResponse.message << "\n\n";
    
    // Test 8: Voter Status Check
    std::cout << "Test 8: Voter Status Check\n";
    auto statusResponse = client.checkVoterRegistration(-1, 123456789);
    assert(statusResponse.success && "Failed to check voter status");
    std::cout << "[PASS] Voter status: " << statusResponse.message << "\n";
    for (const auto& info : statusResponse.data) {
        std::cout << "        " << info << "\n";
    }
    std::cout << "\n";
    
    // Test 9: Direct Election Access (Compatibility)
    std::cout << "Test 9: Direct Election Access (Compatibility)\n";
    auto election = client.getElection(-1);
    assert(election != nullptr && "Failed to get election object");
    std::cout << "[PASS] Direct election access working\n";
    std::cout << "Election title: " << election->getTitle() << "\n\n";
    
    // Test 10: Data Persistence
    std::cout << "Test 10: Data Persistence\n";
    auto saveResponse = client.saveCompleteElectionData(-1, "service_test_election.txt");
    assert(saveResponse.success && "Failed to save election data");
    std::cout << "[PASS] " << saveResponse.message << "\n\n";
    
    std::cout << "=== All Service Infrastructure Tests Passed! ===\n";
    std::cout << "The service architecture is working correctly and maintains\n";
    std::cout << "full compatibility with existing Election functionality.\n";
    
    return 0;
}
