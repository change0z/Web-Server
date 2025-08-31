#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "../../src/services/ServiceClient.h"

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayAdminMenu() {
    std::cout << "\n=== Election Administration System ===\n";
    std::cout << "1. Create Party\n";
    std::cout << "2. Add Independent Candidate\n";
    std::cout << "3. Add Party Candidate\n";
    std::cout << "4. Display Parties\n";
    std::cout << "5. Display Candidates\n";
    std::cout << "6. Display Registered Voters\n";
    std::cout << "7. Display Current Results\n";
    std::cout << "8. Save Results Summary\n";
    std::cout << "9. Save Complete Election Data\n";
    std::cout << "10. Export to CSV Files\n";
    std::cout << "11. Load Election Data\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

void createParty(ServiceClient& client, int electionId) {
    std::string partyName;
    std::cout << "\n--- Create Party ---\n";
    clearInputBuffer();
    std::cout << "Enter party name: ";
    std::getline(std::cin, partyName);
    
    ServiceResponse response = client.createParty(electionId, partyName);
    if (response.success) {
        std::cout << "Party '" << partyName << "' created successfully.\n";
    } else {
        std::cout << "Failed to create party: " << response.message << "\n";
    }
}

void addIndependentCandidate(ServiceClient& client, int electionId) {
    std::string candidateName;
    std::cout << "\n--- Add Independent Candidate ---\n";
    clearInputBuffer();
    std::cout << "Enter candidate name: ";
    std::getline(std::cin, candidateName);
    
    ServiceResponse response = client.addCandidate(electionId, candidateName);
    if (response.success) {
        std::cout << "Independent candidate '" << candidateName << "' added successfully.\n";
    } else {
        std::cout << "Failed to add candidate: " << response.message << "\n";
    }
}

void addPartyCandidate(ServiceClient& client, int electionId) {
    std::string candidateName, partyName;
    
    std::cout << "\n--- Add Party Candidate ---\n";
    
    // Display available parties
    ServiceResponse partiesResponse = client.getParties(electionId);
    if (partiesResponse.success) {
        std::cout << "Available parties:\n";
        std::cout << partiesResponse.message << "\n";
    } else {
        std::cout << "Error retrieving parties: " << partiesResponse.message << "\n";
        return;
    }
    
    clearInputBuffer();
    std::cout << "Enter candidate name: ";
    std::getline(std::cin, candidateName);
    
    std::cout << "Enter party name: ";
    std::getline(std::cin, partyName);
    
    ServiceResponse response = client.addCandidateWithParty(electionId, candidateName, partyName);
    if (response.success) {
        std::cout << "Candidate '" << candidateName << "' added to party '" << partyName << "' successfully.\n";
    } else {
        std::cout << "Failed to add party candidate: " << response.message << "\n";
    }
}

void saveElectionData(ServiceClient& client, int electionId) {
    std::string filename;
    clearInputBuffer();
    std::cout << "Enter filename for basic results: ";
    std::getline(std::cin, filename);
    
    ServiceResponse response = client.saveElectionResults(electionId, filename);
    if (response.success) {
        std::cout << "Election results saved to " << filename << " successfully.\n";
    } else {
        std::cout << "Failed to save election results: " << response.message << "\n";
    }
}

void saveCompleteElectionData(ServiceClient& client, int electionId) {
    std::string filename;
    clearInputBuffer();
    std::cout << "Enter filename for complete election data: ";
    std::getline(std::cin, filename);
    
    ServiceResponse response = client.saveCompleteElectionData(electionId, filename);
    if (response.success) {
        std::cout << "Complete election data saved to " << filename << " successfully.\n";
    } else {
        std::cout << "Failed to save complete election data: " << response.message << "\n";
    }
}

void loadElectionData(ServiceClient& client, int electionId) {
    std::string filename;
    std::cout << "\n--- Load Election Data ---\n";
    clearInputBuffer();
    std::cout << "Enter filename to load from (or press Enter for 'shared_election_data.txt'): ";
    std::getline(std::cin, filename);
    
    if (filename.empty()) {
        filename = "shared_election_data.txt";
    }
    
    ServiceResponse response = client.loadElectionData(electionId, filename);
    if (response.success) {
        std::cout << "Election data loaded successfully!\n";
        std::cout << response.message << "\n";
    } else {
        std::cout << "Failed to load election data from " << filename << "\n";
        std::cout << "Error: " << response.message << "\n";
    }
}

void exportToCSV(ServiceClient& client, int electionId) {
    std::string baseFilename;
    std::cout << "\n--- Export to CSV ---\n";
    clearInputBuffer();
    std::cout << "Enter base filename for CSV export (without extension): ";
    std::getline(std::cin, baseFilename);
    
    ServiceResponse response = client.exportElectionToCSV(electionId, baseFilename);
    if (response.success) {
        std::cout << "Election data exported to CSV files successfully.\n";
        std::cout << "Files created with base name: " << baseFilename << "\n";
    } else {
        std::cout << "Failed to export to CSV: " << response.message << "\n";
    }
}

int main() {
    ServiceClient client;
    int electionId;
    
    std::cout << "=== Election Administration System ===\n";
    std::cout << "Setting up: 2024 Local Elections\n";
    std::cout << "Admin access granted.\n\n";
    
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
    
    std::cout << "Election created with ID: " << electionId << "\n";
    std::cout << "Administration interface ready.\n\n";
    
    int choice;
    while (true) {
        displayAdminMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                createParty(client, electionId);
                break;
            case 2:
                addIndependentCandidate(client, electionId);
                break;
            case 3:
                addPartyCandidate(client, electionId);
                break;
            case 4:
                std::cout << "\n=== Available Parties ===\n";
                {
                    ServiceResponse partiesResponse = client.getParties(electionId);
                    if (partiesResponse.success) {
                        std::cout << partiesResponse.message << "\n";
                    } else {
                        std::cout << "Error retrieving parties: " << partiesResponse.message << "\n";
                    }
                }
                break;
            case 5:
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
            case 6:
                std::cout << "\n=== Registered Voters ===\n";
                {
                    ServiceResponse votersResponse = client.getVoters(electionId);
                    if (votersResponse.success) {
                        std::cout << votersResponse.message << "\n";
                    } else {
                        std::cout << "Error retrieving voters: " << votersResponse.message << "\n";
                    }
                }
                break;
            case 7:
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
            case 8:
                saveElectionData(client, electionId);
                break;
            case 9:
                saveCompleteElectionData(client, electionId);
                break;
            case 10:
                exportToCSV(client, electionId);
                break;
            case 11:
                loadElectionData(client, electionId);
                break;
            case 0:
                std::cout << "Election administration session ended.\n";
                std::cout << "Saving complete election state...\n";
                client.saveCompleteElectionData(electionId, "shared_election_data.txt");
                client.saveCompleteElectionData(electionId, "admin_session_complete.txt"); // Keep backup
                client.exportElectionToCSV(electionId, "admin_session_export");
                client.disconnect();
                return 0;
            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    }
    
    return 0;
}
