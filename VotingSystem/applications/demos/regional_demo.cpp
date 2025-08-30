#include <iostream>
#include <string>
#include <limits>
#include "../../src/core/Election.h"

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayRegionalMenu() {
    std::cout << "\n========================================\n";
    std::cout << "    REGIONAL VOTING SYSTEM DEMO\n";
    std::cout << "========================================\n";
    std::cout << "1. Setup Regions\n";
    std::cout << "2. Create Parties\n";
    std::cout << "3. Add Candidates to Regions\n";
    std::cout << "4. Register Voters in Regions\n";
    std::cout << "5. Display Regional Information\n";
    std::cout << "6. Cast Votes in Region\n";
    std::cout << "7. View Regional Results\n";
    std::cout << "8. View All Regions Overview\n";
    std::cout << "9. Test Regional Restrictions\n";
    std::cout << "0. Exit\n";
    std::cout << "========================================\n";
    std::cout << "Choose an option: ";
}

void setupRegions(Election& election) {
    std::cout << "\n--- Setup Election Regions ---\n";
    
    // Create demo regions
    auto northRegion = election.createRegion("North District", "NORTH");
    auto southRegion = election.createRegion("South District", "SOUTH");
    auto eastRegion = election.createRegion("East District", "EAST");
    
    std::cout << "\nDemo regions created successfully!\n";
}

void createDemoParties(Election& election) {
    std::cout << "\n--- Creating Political Parties ---\n";
    
    auto democrats = election.createParty("Democratic Party");
    auto republicans = election.createParty("Republican Party");
    auto greens = election.createParty("Green Party");
    auto independents = election.createParty("Independent Alliance");
    
    std::cout << "\nDemo parties created successfully!\n";
}

void addCandidatesToRegions(Election& election) {
    std::cout << "\n--- Adding Candidates to Regions ---\n";
    
    election.displayRegions();
    election.displayParties();
    
    std::string candidateName, regionIndex, partyIndex;
    std::cout << "\nEnter candidate name: ";
    clearInputBuffer();
    std::getline(std::cin, candidateName);
    
    std::cout << "Enter region number (1-based): ";
    std::getline(std::cin, regionIndex);
    
    std::cout << "Enter party number (1-based, 0 for independent): ";
    std::getline(std::cin, partyIndex);
    
    try {
        int regIdx = std::stoi(regionIndex);
        auto region = election.getRegionByIndex(regIdx);
        
        if (!region) {
            std::cout << "Invalid region number.\n";
            return;
        }
        
        int partyIdx = std::stoi(partyIndex);
        if (partyIdx == 0) {
            // Independent candidate
            election.addCandidateToRegion(candidateName, region);
        } else {
            auto party = election.getPartyByIndex(partyIdx);
            if (party) {
                election.addCandidateToRegion(candidateName, party, region);
            } else {
                std::cout << "Invalid party number.\n";
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid input format.\n";
    }
}

void registerVotersInRegions(Election& election) {
    std::cout << "\n--- Register Voter in Region ---\n";
    
    election.displayRegions();
    
    std::string firstName, lastName, phone, address, uniqueId, age, regionIndex;
    
    clearInputBuffer();
    std::cout << "Enter first name: ";
    std::getline(std::cin, firstName);
    
    std::cout << "Enter last name: ";
    std::getline(std::cin, lastName);
    
    std::cout << "Enter phone number: ";
    std::getline(std::cin, phone);
    
    std::cout << "Enter address: ";
    std::getline(std::cin, address);
    
    std::cout << "Enter unique ID: ";
    std::getline(std::cin, uniqueId);
    
    std::cout << "Enter age: ";
    std::getline(std::cin, age);
    
    std::cout << "Enter region number (1-based): ";
    std::getline(std::cin, regionIndex);
    
    try {
        int regIdx = std::stoi(regionIndex);
        auto region = election.getRegionByIndex(regIdx);
        
        if (region) {
            election.registerVoterInRegion(firstName, lastName, phone, address, uniqueId, age, region);
        } else {
            std::cout << "Invalid region number.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid region number format.\n";
    }
}

void displayRegionalInfo(Election& election) {
    std::cout << "\n--- Regional Information ---\n";
    
    election.displayRegions();
    
    std::string regionIndex;
    std::cout << "\nEnter region number to view details (1-based): ";
    clearInputBuffer();
    std::getline(std::cin, regionIndex);
    
    try {
        int regIdx = std::stoi(regionIndex);
        auto region = election.getRegionByIndex(regIdx);
        
        if (region) {
            region->displayRegionInfo();
            election.displayCandidatesInRegion(region);
            election.displayVotersInRegion(region);
        } else {
            std::cout << "Invalid region number.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid input format.\n";
    }
}

void castVoteInRegion(Election& election) {
    std::cout << "\n--- Cast Vote in Region ---\n";
    
    election.displayRegions();
    
    std::string voterIdStr, regionIndex;
    
    clearInputBuffer();
    std::cout << "Enter your voter ID: ";
    std::getline(std::cin, voterIdStr);
    
    std::cout << "Enter region number where you want to vote (1-based): ";
    std::getline(std::cin, regionIndex);
    
    try {
        int voterId = std::stoi(voterIdStr);
        int regIdx = std::stoi(regionIndex);
        auto region = election.getRegionByIndex(regIdx);
        
        if (!region) {
            std::cout << "Invalid region number.\n";
            return;
        }
        
        // Show candidates in this region
        std::cout << "\nCandidates in " << region->getName() << ":\n";
        election.displayCandidatesInRegion(region);
        
        std::string candidateIndex;
        std::cout << "\nEnter candidate number (0-based): ";
        std::getline(std::cin, candidateIndex);
        
        int candIdx = std::stoi(candidateIndex);
        election.castVoteInRegion(voterId, candIdx, region);
        
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid input format.\n";
    }
}

void viewRegionalResults(Election& election) {
    std::cout << "\n--- Regional Election Results ---\n";
    
    election.displayRegions();
    
    std::string regionIndex;
    std::cout << "\nEnter region number to view results (1-based): ";
    clearInputBuffer();
    std::getline(std::cin, regionIndex);
    
    try {
        int regIdx = std::stoi(regionIndex);
        auto region = election.getRegionByIndex(regIdx);
        
        if (region) {
            election.displayResultsInRegion(region);
        } else {
            std::cout << "Invalid region number.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid input format.\n";
    }
}

void viewAllRegionsOverview(Election& election) {
    std::cout << "\n--- All Regions Overview ---\n";
    election.displayRegions();
    
    std::cout << "\n--- Global Parties ---\n";
    election.displayParties();
}

void testRegionalRestrictions(Election& election) {
    std::cout << "\n--- Testing Regional Restrictions ---\n";
    std::cout << "This will demonstrate the regional voting restrictions:\n";
    std::cout << "1. Same candidate cannot be in multiple regions\n";
    std::cout << "2. Voters can only vote in their assigned region\n";
    std::cout << "3. One candidate per party per region\n\n";
    
    // Get regions for testing
    auto northRegion = election.getRegionByCode("NORTH");
    auto southRegion = election.getRegionByCode("SOUTH");
    auto democraticParty = election.getPartyByIndex(1);
    
    if (!northRegion || !southRegion || !democraticParty) {
        std::cout << "Please setup regions and parties first (options 1 and 2).\n";
        return;
    }
    
    std::cout << "Test 1: Attempting to add same candidate to multiple regions...\n";
    election.addCandidateToRegion("John Duplicate", democraticParty, northRegion);
    election.addCandidateToRegion("John Duplicate", democraticParty, southRegion); // Should fail
    
    std::cout << "\nTest 2: Attempting to add multiple candidates from same party to same region...\n";
    election.addCandidateToRegion("Alice Democrat", democraticParty, northRegion);
    election.addCandidateToRegion("Bob Democrat", democraticParty, northRegion); // Should fail
    
    std::cout << "\nTest 3: Attempting cross-region voting...\n";
    std::cout << "Register a voter in North region, then try voting in South region:\n";
    
    // Register voter in North
    election.registerVoterInRegion("Test", "Voter", "1234567890", "123 Test St", "999888", "25", northRegion);
    
    // Try to vote in South (should fail)
    std::cout << "Attempting to vote in South region with North region voter ID 999888:\n";
    election.castVoteInRegion(999888, 0, southRegion); // Should fail
}

int main() {
    Election election("2024 Regional Elections");
    int choice;
    
    std::cout << "Welcome to the Regional Voting System Demo!\n";
    std::cout << "This system demonstrates region-based voting with restrictions:\n";
    std::cout << "- Candidates belong to specific regions\n";
    std::cout << "- Voters can only vote in their assigned region\n";
    std::cout << "- One candidate per party per region\n";
    std::cout << "- Parties can operate across multiple regions\n\n";
    
    while (true) {
        displayRegionalMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                setupRegions(election);
                break;
            case 2:
                createDemoParties(election);
                break;
            case 3:
                addCandidatesToRegions(election);
                break;
            case 4:
                registerVotersInRegions(election);
                break;
            case 5:
                displayRegionalInfo(election);
                break;
            case 6:
                castVoteInRegion(election);
                break;
            case 7:
                viewRegionalResults(election);
                break;
            case 8:
                viewAllRegionsOverview(election);
                break;
            case 9:
                testRegionalRestrictions(election);
                break;
            case 0:
                std::cout << "Thank you for using the Regional Voting System!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    
    return 0;
}
