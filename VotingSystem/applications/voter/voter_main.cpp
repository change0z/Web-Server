#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "../../src/services/ServiceClient.h"

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

void registerVoter(ServiceClient& client, int electionId) {
    std::string firstName, lastName, phoneNumber, address;
    std::string uniqueIdStr, ageStr;
    
    std::cout << "\n=== Enhanced Voter Registration ===\n";
    std::cout << "Please provide your information to register:\n";
    std::cout << "Note: All fields will be validated according to election requirements.\n\n";
    
    clearInputBuffer();
    
    std::cout << "First name (alphabets only, 2-50 characters): ";
    std::getline(std::cin, firstName);
    
    std::cout << "Last name (alphabets only, 2-50 characters): ";
    std::getline(std::cin, lastName);
    
    std::cout << "Phone number (exactly 10 digits): ";
    std::getline(std::cin, phoneNumber);
    
    std::cout << "Address (5-500 characters): ";
    std::getline(std::cin, address);
    
    std::cout << "Unique ID number (exactly 9 digits): ";
    std::getline(std::cin, uniqueIdStr);
    
    std::cout << "Age (18 or older): ";
    std::getline(std::cin, ageStr);
    
    // Use ServiceClient for voter registration
    ServiceResponse response = client.registerVoter(electionId, firstName, lastName, phoneNumber, address, uniqueIdStr, ageStr);
    
    if (response.success) {
        std::cout << "\n*** Registration Successful! ***\n";
        std::cout << "Welcome to the election, " << firstName << " " << lastName << "!\n";
        std::cout << "You can now cast your vote.\n";
    } else {
        std::cout << "\n*** Registration Failed ***\n";
        std::cout << "Error: " << response.message << "\n";
        std::cout << "Please review the requirements and try again.\n";
    }
}

void castVote(ServiceClient& client, int electionId) {
    std::string voterIdStr, candidateIndexStr;
    int voterId, candidateIndex;
    
    std::cout << "\n=== Cast Your Vote ===\n";
    
    // Show available candidates
    std::cout << "Available candidates:\n";
    ServiceResponse candidatesResponse = client.getCandidates(electionId);
    if (candidatesResponse.success) {
        std::cout << candidatesResponse.message << "\n";
    } else {
        std::cout << "Error retrieving candidates: " << candidatesResponse.message << "\n";
        return;
    }
    
    clearInputBuffer();
    
    std::cout << "\nEnter your voter ID: ";
    std::getline(std::cin, voterIdStr);
    
    std::cout << "Enter candidate number (0-based): ";
    std::getline(std::cin, candidateIndexStr);
    
    try {
        voterId = std::stoi(voterIdStr);
        candidateIndex = std::stoi(candidateIndexStr);
    } catch (const std::exception&) {
        std::cout << "Error: Invalid input. Please enter valid numbers.\n";
        return;
    }
    
    ServiceResponse response = client.castVote(electionId, voterId, candidateIndex);
    
    if (response.success) {
        std::cout << "\n*** Vote Successfully Cast! ***\n";
        std::cout << "Thank you for participating in the democratic process.\n";
        std::cout << "Your vote has been recorded securely.\n";
    } else {
        std::cout << "\n*** Voting Failed ***\n";
        std::cout << "Error: " << response.message << "\n";
    }
}

void checkRegistration(ServiceClient& client, int electionId) {
    std::string voterIdStr;
    int voterId;
    
    std::cout << "\n=== Check Registration Status ===\n";
    clearInputBuffer();
    
    std::cout << "Enter your voter ID: ";
    std::getline(std::cin, voterIdStr);
    
    try {
        voterId = std::stoi(voterIdStr);
    } catch (const std::exception&) {
        std::cout << "Error: Invalid voter ID format.\n";
        return;
    }
    
    ServiceResponse response = client.checkVoterRegistration(electionId, voterId);
    
    if (response.success) {
        std::cout << "[SUCCESS] You are registered to vote!\n";
        std::cout << response.message << "\n";
    } else {
        std::cout << "[ERROR] You are not registered. Please register first.\n";
        std::cout << "Details: " << response.message << "\n";
    }
}

// Load pre-configured election (simulating admin setup)
void loadDemoElection(ServiceClient& client, int electionId) {
    std::cout << "Loading election configuration...\n";
    
    // Create demo parties and candidates (simulating admin setup)
    client.createParty(electionId, "Democratic Party");
    client.createParty(electionId, "Republican Party");
    client.createParty(electionId, "Green Party");
    
    client.addCandidateWithParty(electionId, "Alice Johnson", "Democratic Party");
    client.addCandidateWithParty(electionId, "Bob Smith", "Republican Party");
    client.addCandidateWithParty(electionId, "Carol Green", "Green Party");
    client.addCandidate(electionId, "David Independent"); // Independent candidate
    
    std::cout << "Election setup complete!\n";
    std::cout << "Candidates and parties have been configured by election officials.\n\n";
}

int main() {
    ServiceClient client;
    int electionId;
    
    std::cout << "===========================================\n";
    std::cout << "     WELCOME TO THE VOTING SYSTEM\n";
    std::cout << "===========================================\n";
    
    // Connect to the service
    std::cout << "Connecting to ClearBallot service...\n";
    if (!client.connectToService()) {
        std::cout << "Error: Unable to connect to ClearBallot service.\n";
        std::cout << "Please ensure the service is running and try again.\n";
        return 1;
    }
    std::cout << "Successfully connected to ClearBallot service!\n\n";
    
    // Create or use default election
    electionId = client.createElection("2024 Local Elections");
    if (electionId <= 0) {
        std::cout << "Error: Unable to create election.\n";
        return 1;
    }
    
    // Try to load admin-configured election data first
    std::cout << "Checking for election configuration...\n";
    ServiceResponse loadResponse = client.loadElectionData(electionId, "shared_election_data.txt");
    
    if (!loadResponse.success) {
        // Try loading admin session data
        loadResponse = client.loadElectionData(electionId, "admin_session_complete.txt");
    }
    
    if (!loadResponse.success) {
        std::cout << "No election configuration found. Loading demo election...\n";
        // Load pre-configured election (simulating that admin has set it up)
        loadDemoElection(client, electionId);
    } else {
        std::cout << "Election configuration loaded from previous session!\n";
    }
    
    std::cout << "Election: 2024 Local Elections\n";
    std::cout << "Voting is now open!\n\n";
    
    std::cout << "Instructions:\n";
    std::cout << "1. You must register before voting\n";
    std::cout << "2. You must be 18 or older to register\n";
    std::cout << "3. Each person can only vote once\n";
    std::cout << "4. Your vote is secret and secure\n\n";
    
    int choice;
    while (true) {
        displayVoterMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                registerVoter(client, electionId);
                break;
            case 2:
                castVote(client, electionId);
                break;
            case 3:
                std::cout << "\n=== Available Candidates ===\n";
                {
                    ServiceResponse candidatesResponse = client.getCandidates(electionId);
                    if (candidatesResponse.success) {
                        std::cout << candidatesResponse.message << "\n";
                    } else {
                        std::cout << "Error retrieving candidates: " << candidatesResponse.message << "\n";
                    }
                }
                break;
            case 4:
                checkRegistration(client, electionId);
                break;
            case 5:
                std::cout << "\n=== Current Election Results ===\n";
                {
                    ServiceResponse resultsResponse = client.getElectionResults(electionId);
                    if (resultsResponse.success) {
                        std::cout << resultsResponse.message << "\n";
                    } else {
                        std::cout << "Error retrieving results: " << resultsResponse.message << "\n";
                    }
                }
                break;
            case 0:
                std::cout << "\nThank you for using the Voting System!\n";
                std::cout << "Your participation strengthens democracy.\n";
                std::cout << "Saving election state...\n";
                client.saveCompleteElectionData(electionId, "shared_election_data.txt");
                client.disconnect();
                return 0;
            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    }
    
    return 0;
}
