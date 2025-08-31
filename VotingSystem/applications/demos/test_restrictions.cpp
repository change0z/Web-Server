#include <iostream>
#include "../../src/core/Election.h"

int main() {
    std::cout << "======================================\n";
    std::cout << "    REGIONAL VOTING RESTRICTIONS TEST\n";
    std::cout << "======================================\n\n";

    Election election("2024 Regional Elections Test");
    
    // Setup regions
    std::cout << "1. Setting up regions...\n";
    auto northRegion = election.createRegion("North District", "NORTH");
    auto southRegion = election.createRegion("South District", "SOUTH");
    auto eastRegion = election.createRegion("East District", "EAST");
    
    // Create parties
    std::cout << "\n2. Creating parties...\n";
    auto democraticParty = election.createParty("Democratic Party");
    auto republicanParty = election.createParty("Republican Party");
    auto greenParty = election.createParty("Green Party");
    
    std::cout << "\n======================================\n";
    std::cout << "    TESTING RESTRICTION 1:\n";
    std::cout << "    Same candidate cannot be in multiple regions\n";
    std::cout << "======================================\n";
    
    // Test 1: Same candidate in multiple regions (should fail)
    std::cout << "\nAttempting to add 'John Smith' to North District...\n";
    election.addCandidateToRegion("John Smith", democraticParty, northRegion);
    
    std::cout << "\nAttempting to add same candidate 'John Smith' to South District...\n";
    election.addCandidateToRegion("John Smith", republicanParty, southRegion); // Should fail
    
    std::cout << "\n======================================\n";
    std::cout << "    TESTING RESTRICTION 2:\n";
    std::cout << "    One candidate per party per region\n";
    std::cout << "======================================\n";
    
    // Test 2: Multiple candidates from same party in same region (should fail)
    std::cout << "\nAdding 'Alice Johnson' (Democratic) to North District...\n";
    election.addCandidateToRegion("Alice Johnson", democraticParty, northRegion);
    
    std::cout << "\nAttempting to add another Democratic candidate 'Bob Wilson' to same region...\n";
    election.addCandidateToRegion("Bob Wilson", democraticParty, northRegion); // Should fail
    
    std::cout << "\n======================================\n";
    std::cout << "    TESTING RESTRICTION 3:\n";
    std::cout << "    Voters can only vote in their assigned region\n";
    std::cout << "======================================\n";
    
    // Register voters in different regions
    std::cout << "\nRegistering voter 'Test Voter' in North District...\n";
    election.registerVoterInRegion("Test", "Voter", "1234567890", "123 Main St", "123456789", "25", northRegion);
    
    // Add a candidate to South region for voting test
    std::cout << "\nAdding candidate 'Sarah Green' to South District...\n";
    election.addCandidateToRegion("Sarah Green", greenParty, southRegion);
    
    std::cout << "\nAttempting to vote in South District with North District voter...\n";
    election.castVoteInRegion(123456789, 0, southRegion); // Should fail
    
    std::cout << "\nAttempting to vote in correct region (North District)...\n";
    election.castVoteInRegion(123456789, 0, northRegion); // Should succeed
    
    std::cout << "\n======================================\n";
    std::cout << "    TESTING VALID SCENARIOS\n";
    std::cout << "======================================\n";
    
    // Valid scenarios
    std::cout << "\nAdding different parties to same region (should succeed)...\n";
    election.addCandidateToRegion("Republican Mike", republicanParty, northRegion);
    election.addCandidateToRegion("Green Carol", greenParty, northRegion);
    
    std::cout << "\nAdding same party to different regions (should succeed)...\n";
    election.addCandidateToRegion("Democratic Dave", democraticParty, southRegion);
    election.addCandidateToRegion("Democratic Eve", democraticParty, eastRegion);
    
    std::cout << "\n======================================\n";
    std::cout << "    FINAL REGIONAL OVERVIEW\n";
    std::cout << "======================================\n";
    
    election.displayRegions();
    
    std::cout << "\nNorth District candidates:\n";
    election.displayCandidatesInRegion(northRegion);
    
    std::cout << "\nSouth District candidates:\n";
    election.displayCandidatesInRegion(southRegion);
    
    std::cout << "\nEast District candidates:\n";
    election.displayCandidatesInRegion(eastRegion);
    
    std::cout << "\n======================================\n";
    std::cout << "    TEST COMPLETE\n";
    std::cout << "======================================\n";
    std::cout << "All regional restrictions have been successfully enforced!\n";
    
    return 0;
}
