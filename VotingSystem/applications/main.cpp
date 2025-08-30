#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "../src/core/Election.h"

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayMenu() {
    std::cout << "\n=== Voting System CLI ===\n";
    std::cout << "1. Register Voter\n";
    std::cout << "2. Create Party\n";
    std::cout << "3. Add Independent Candidate\n";
    std::cout << "4. Add Party Candidate\n";
    std::cout << "5. Cast Vote\n";
    std::cout << "6. Display Voters\n";
    std::cout << "7. Display Parties\n";
    std::cout << "8. Display Candidates\n";
    std::cout << "9. Display Results\n";
    std::cout << "10. Save Results to File\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

void registerVoter(Election& election) {
    std::string firstName, lastName, phoneNumber, address;
    std::string uniqueIdStr, ageStr;
    int uniqueId, age;
    
    std::cout << "\n--- Voter Registration ---\n";
    
    clearInputBuffer(); // Clear any leftover input
    
    std::cout << "Enter first name: ";
    std::getline(std::cin, firstName);
    
    std::cout << "Enter last name: ";
    std::getline(std::cin, lastName);
    
    std::cout << "Enter phone number: ";
    std::getline(std::cin, phoneNumber);
    
    std::cout << "Enter address: ";
    std::getline(std::cin, address);
    
    std::cout << "Enter unique ID: ";
    std::getline(std::cin, uniqueIdStr);
    
    std::cout << "Enter age: ";
    std::getline(std::cin, ageStr);
    
    // Convert strings to integers with error checking
    try {
        uniqueId = std::stoi(uniqueIdStr);
        age = std::stoi(ageStr);
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid input for ID or age. Please enter valid numbers.\n";
        return;
    }
    
    election.registerVoter(firstName, lastName, phoneNumber, address, uniqueId, age);
}

void createParty(Election& election) {
    std::string partyName;
    std::cout << "\n--- Create Party ---\n";
    clearInputBuffer(); // Clear input buffer
    std::cout << "Enter party name: ";
    std::getline(std::cin, partyName);
    election.createParty(partyName);
}

void addIndependentCandidate(Election& election) {
    std::string candidateName;
    std::cout << "\n--- Add Independent Candidate ---\n";
    clearInputBuffer(); // Clear input buffer
    std::cout << "Enter candidate name: ";
    std::getline(std::cin, candidateName);
    election.addCandidate(candidateName);
}

void addPartyCandidate(Election& election) {
    std::string candidateName, partyIndexStr;
    int partyIndex;
    
    std::cout << "\n--- Add Party Candidate ---\n";
    election.displayParties();
    
    clearInputBuffer(); // Clear input buffer
    std::cout << "Enter candidate name: ";
    std::getline(std::cin, candidateName);
    
    std::cout << "Enter party number (1-based): ";
    std::getline(std::cin, partyIndexStr);
    
    try {
        partyIndex = std::stoi(partyIndexStr);
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid party number.\n";
        return;
    }
    
    auto party = election.getPartyByIndex(partyIndex);
    if (party) {
        election.addCandidate(candidateName, party);
    } else {
        std::cout << "Invalid party number.\n";
    }
}

void castVote(Election& election) {
    std::string voterIdStr, candidateIndexStr;
    int voterId, candidateIndex;
    
    std::cout << "\n--- Cast Vote ---\n";
    election.displayCandidates();
    
    clearInputBuffer(); // Clear input buffer
    std::cout << "Enter your unique voter ID: ";
    std::getline(std::cin, voterIdStr);
    
    std::cout << "Enter candidate number (0-based): ";
    std::getline(std::cin, candidateIndexStr);
    
    try {
        voterId = std::stoi(voterIdStr);
        candidateIndex = std::stoi(candidateIndexStr);
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid input. Please enter valid numbers.\n";
        return;
    }
    
    election.castVote(voterId, candidateIndex);
}

int main() {
    Election election("2024 Local Elections");
    int choice;
    
    std::cout << "Welcome to the Voting System!\n";
    std::cout << "Election: 2024 Local Elections\n";
    
    // Demo setup
    std::cout << "\n--- Demo Setup ---\n";
    auto democrats = election.createParty("Democratic Party");
    auto republicans = election.createParty("Republican Party");
    
    election.addCandidate("Alice Johnson", democrats);
    election.addCandidate("Bob Smith", republicans);
    election.addCandidate("Charlie Brown"); // Independent
    
    std::cout << "\nDemo candidates added!\n";
    
    while (true) {
        displayMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                registerVoter(election);
                break;
            case 2:
                createParty(election);
                break;
            case 3:
                addIndependentCandidate(election);
                break;
            case 4:
                addPartyCandidate(election);
                break;
            case 5:
                castVote(election);
                break;
            case 6:
                election.displayVoters();
                break;
            case 7:
                election.displayParties();
                break;
            case 8:
                election.displayCandidates();
                break;
            case 9:
                election.displayResults();
                break;
            case 10: {
                std::string filename;
                clearInputBuffer(); // Clear input buffer
                std::cout << "Enter filename: ";
                std::getline(std::cin, filename);
                election.saveResultsToFile(filename);
                break;
            }
            case 0:
                std::cout << "Thank you for using the Voting System!\n";
                return 0;
            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    }
    
    return 0;
}
