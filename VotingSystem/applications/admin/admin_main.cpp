#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "../../src/core/Election.h"

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

void createParty(Election& election) {
    std::string partyName;
    std::cout << "\n--- Create Party ---\n";
    clearInputBuffer();
    std::cout << "Enter party name: ";
    std::getline(std::cin, partyName);
    election.createParty(partyName);
}

void addIndependentCandidate(Election& election) {
    std::string candidateName;
    std::cout << "\n--- Add Independent Candidate ---\n";
    clearInputBuffer();
    std::cout << "Enter candidate name: ";
    std::getline(std::cin, candidateName);
    election.addCandidate(candidateName);
}

void addPartyCandidate(Election& election) {
    std::string candidateName, partyIndexStr;
    int partyIndex;
    
    std::cout << "\n--- Add Party Candidate ---\n";
    election.displayParties();
    
    clearInputBuffer();
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

void saveElectionData(Election& election) {
    std::string filename;
    clearInputBuffer();
    std::cout << "Enter filename for basic results: ";
    std::getline(std::cin, filename);
    election.saveResultsToFile(filename);
}

void saveCompleteElectionData(Election& election) {
    std::string filename;
    clearInputBuffer();
    std::cout << "Enter filename for complete election data: ";
    std::getline(std::cin, filename);
    election.saveCompleteElectionData(filename);
}

void loadElectionData(Election& election) {
    std::string filename;
    std::cout << "\n--- Load Election Data ---\n";
    clearInputBuffer();
    std::cout << "Enter filename to load from (or press Enter for 'shared_election_data.txt'): ";
    std::getline(std::cin, filename);
    
    if (filename.empty()) {
        filename = "shared_election_data.txt";
    }
    
    bool success = election.loadCompleteElectionData(filename);
    if (success) {
        std::cout << "Election data loaded successfully!\n";
    } else {
        std::cout << "Failed to load election data from " << filename << "\n";
    }
}

void exportToCSV(Election& election) {
    std::string baseFilename;
    std::cout << "\n--- Export to CSV ---\n";
    clearInputBuffer();
    std::cout << "Enter base filename for CSV export (without extension): ";
    std::getline(std::cin, baseFilename);
    election.exportToCSV(baseFilename);
}

int main() {
    Election election("2024 Local Elections");
    int choice;
    
    std::cout << "=== Election Administration System ===\n";
    std::cout << "Setting up: 2024 Local Elections\n";
    std::cout << "Admin access granted.\n\n";
    
    while (true) {
        displayAdminMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                createParty(election);
                break;
            case 2:
                addIndependentCandidate(election);
                break;
            case 3:
                addPartyCandidate(election);
                break;
            case 4:
                election.displayParties();
                break;
            case 5:
                election.displayCandidates();
                break;
            case 6:
                election.displayVoters();
                break;
            case 7:
                election.displayResults();
                break;
            case 8:
                saveElectionData(election);
                break;
            case 9:
                saveCompleteElectionData(election);
                break;
            case 10:
                exportToCSV(election);
                break;
            case 11:
                loadElectionData(election);
                break;
            case 0:
                std::cout << "Election administration session ended.\n";
                std::cout << "Saving complete election state...\n";
                election.saveCompleteElectionData("shared_election_data.txt");
                election.saveCompleteElectionData("admin_session_complete.txt"); // Keep backup
                election.exportToCSV("admin_session_export");
                return 0;
            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    }
    
    return 0;
}
