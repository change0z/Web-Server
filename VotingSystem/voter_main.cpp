#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "Election.h"

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayVoterMenu() {
    std::cout << "\n=== Voter Interface ===\n";
    std::cout << "1. Register to Vote\n";
    std::cout << "2. Cast Vote\n";
    std::cout << "3. View Candidates\n";
    std::cout << "4. Check My Registration\n";
    std::cout << "5. View Election Results\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

void registerVoter(Election& election) {
    std::string firstName, lastName, phoneNumber, address;
    std::string uniqueIdStr, ageStr;
    int uniqueId, age;
    
    std::cout << "\n=== Voter Registration ===\n";
    std::cout << "Please provide your information to register:\n";
    
    clearInputBuffer();
    
    std::cout << "First name: ";
    std::getline(std::cin, firstName);
    
    std::cout << "Last name: ";
    std::getline(std::cin, lastName);
    
    std::cout << "Phone number: ";
    std::getline(std::cin, phoneNumber);
    
    std::cout << "Address: ";
    std::getline(std::cin, address);
    
    std::cout << "Unique ID number: ";
    std::getline(std::cin, uniqueIdStr);
    
    std::cout << "Age: ";
    std::getline(std::cin, ageStr);
    
    try {
        uniqueId = std::stoi(uniqueIdStr);
        age = std::stoi(ageStr);
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid input for ID or age. Please enter valid numbers.\n";
        return;
    }
    
    bool success = election.registerVoter(firstName, lastName, phoneNumber, address, uniqueId, age);
    
    if (success) {
        std::cout << "\n*** Registration Successful! ***\n";
        std::cout << "Welcome to the election, " << firstName << " " << lastName << "!\n";
        std::cout << "Your voter ID is: " << uniqueId << "\n";
        std::cout << "You can now cast your vote.\n";
    }
}

void castVote(Election& election) {
    std::string voterIdStr, candidateIndexStr;
    int voterId, candidateIndex;
    
    std::cout << "\n=== Cast Your Vote ===\n";
    
    // Show available candidates
    std::cout << "Available candidates:\n";
    election.displayCandidates();
    
    clearInputBuffer();
    
    std::cout << "\nEnter your voter ID: ";
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
    
    bool success = election.castVote(voterId, candidateIndex);
    
    if (success) {
        std::cout << "\n*** Vote Successfully Cast! ***\n";
        std::cout << "Thank you for participating in the democratic process.\n";
        std::cout << "Your vote has been recorded securely.\n";
    }
}

void checkRegistration(Election& election) {
    std::string voterIdStr;
    int voterId;
    
    std::cout << "\n=== Check Registration Status ===\n";
    clearInputBuffer();
    
    std::cout << "Enter your voter ID: ";
    std::getline(std::cin, voterIdStr);
    
    try {
        voterId = std::stoi(voterIdStr);
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid voter ID format.\n";
        return;
    }
    
    if (election.isVoterRegistered(voterId)) {
        std::cout << "✓ You are registered to vote!\n";
        if (election.hasVoterVoted(voterId)) {
            std::cout << "✓ You have already cast your vote.\n";
        } else {
            std::cout << "○ You have not yet voted. You can cast your vote now!\n";
        }
    } else {
        std::cout << "✗ You are not registered. Please register first.\n";
    }
}

// Load pre-configured election (simulating admin setup)
void loadDemoElection(Election& election) {
    std::cout << "Loading election configuration...\n";
    
    // Create demo parties and candidates (simulating admin setup)
    auto democraticParty = election.createParty("Democratic Party");
    auto republicanParty = election.createParty("Republican Party");
    auto greenParty = election.createParty("Green Party");
    
    election.addCandidate("Alice Johnson", democraticParty);
    election.addCandidate("Bob Smith", republicanParty);
    election.addCandidate("Carol Green", greenParty);
    election.addCandidate("David Independent"); // Independent candidate
    
    std::cout << "Election setup complete!\n";
    std::cout << "Candidates and parties have been configured by election officials.\n\n";
}

int main() {
    Election election("2024 Local Elections");
    int choice;
    
    std::cout << "===========================================\n";
    std::cout << "     WELCOME TO THE VOTING SYSTEM\n";
    std::cout << "===========================================\n";
    std::cout << "Election: 2024 Local Elections\n";
    std::cout << "Voting is now open!\n\n";
    
    // Load pre-configured election (simulating that admin has set it up)
    loadDemoElection(election);
    
    std::cout << "Instructions:\n";
    std::cout << "1. You must register before voting\n";
    std::cout << "2. You must be 18 or older to register\n";
    std::cout << "3. Each person can only vote once\n";
    std::cout << "4. Your vote is secret and secure\n\n";
    
    while (true) {
        displayVoterMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                registerVoter(election);
                break;
            case 2:
                castVote(election);
                break;
            case 3:
                std::cout << "\n=== Available Candidates ===\n";
                election.displayCandidates();
                break;
            case 4:
                checkRegistration(election);
                break;
            case 5:
                std::cout << "\n=== Current Election Results ===\n";
                election.displayResults();
                break;
            case 0:
                std::cout << "\nThank you for using the Voting System!\n";
                std::cout << "Your participation strengthens democracy.\n";
                std::cout << "Saving election state...\n";
                election.saveCompleteElectionData("voter_session_complete.txt");
                return 0;
            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    }
    
    return 0;
}
