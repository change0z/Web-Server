#include "Election.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <map>

Election::Election(const std::string& title) : title(title) {}

// Candidate management
void Election::addCandidate(const std::string& name) {
    candidates.push_back(std::make_unique<Candidate>(name));
    std::cout << "Independent candidate '" << name << "' added successfully.\n";
}

void Election::addCandidate(const std::string& name, std::shared_ptr<Party> party) {
    candidates.push_back(std::make_unique<Candidate>(name, party));
    party->addMember(name);
    std::cout << "Candidate '" << name << "' added to party '" << party->getName() << "'.\n";
}

// Party management
std::shared_ptr<Party> Election::createParty(const std::string& partyName) {
    auto party = std::make_shared<Party>(partyName);
    parties.push_back(party);
    std::cout << "Party '" << partyName << "' created successfully.\n";
    return party;
}

void Election::displayParties() const {
    if (parties.empty()) {
        std::cout << "No parties registered.\n";
        return;
    }
    
    std::cout << "\n=== Registered Parties ===\n";
    for (size_t i = 0; i < parties.size(); ++i) {
        std::cout << i + 1 << ". " << parties[i]->getName() 
                  << " (Members: " << parties[i]->getMembers().size() << ")\n";
    }
}

std::shared_ptr<Party> Election::getPartyByIndex(size_t index) const {
    if (index == 0 || index > parties.size()) {
        return nullptr;
    }
    return parties[index - 1];
}

// Region management
std::shared_ptr<Region> Election::createRegion(const std::string& name, const std::string& code) {
    // Check if region with same code already exists
    for (const auto& region : regions) {
        if (region->getCode() == code) {
            std::cout << "Error: Region with code '" << code << "' already exists.\n";
            return nullptr;
        }
    }
    
    auto region = std::make_shared<Region>(name, code);
    regions.push_back(region);
    std::cout << "Region '" << name << "' (Code: " << code << ") created successfully.\n";
    return region;
}

void Election::displayRegions() const {
    if (regions.empty()) {
        std::cout << "No regions created.\n";
        return;
    }
    
    std::cout << "\n=== Election Regions ===\n";
    for (size_t i = 0; i < regions.size(); ++i) {
        std::cout << i + 1 << ". " << regions[i]->getName() 
                  << " (" << regions[i]->getCode() << ")\n";
        std::cout << "   Candidates: " << regions[i]->getCandidates().size()
                  << ", Voters: " << regions[i]->getVoters().size() << "\n";
    }
}

std::shared_ptr<Region> Election::getRegionByIndex(size_t index) const {
    if (index == 0 || index > regions.size()) {
        return nullptr;
    }
    return regions[index - 1];
}

std::shared_ptr<Region> Election::getRegionByCode(const std::string& code) const {
    for (const auto& region : regions) {
        if (region->getCode() == code) {
            return region;
        }
    }
    return nullptr;
}

// Enhanced candidate management with region support
bool Election::addCandidateToRegion(const std::string& name, std::shared_ptr<Region> region) {
    if (!region) {
        std::cout << "Error: Invalid region provided.\n";
        return false;
    }
    
    // Check if candidate already exists in ANY region (global restriction)
    if (globalCandidateRegistry.find(name) != globalCandidateRegistry.end()) {
        std::cout << "Error: Candidate '" << name << "' is already registered in another region.\n";
        std::cout << "Same candidate cannot be enlisted in multiple regions.\n";
        return false;
    }
    
    auto candidate = std::make_shared<Candidate>(name, nullptr, region);
    if (region->addCandidate(candidate)) {
        // Add to global registry
        globalCandidateRegistry[name].insert(region->getCode());
        std::cout << "Independent candidate '" << name << "' added to region '" 
                  << region->getName() << "'.\n";
        return true;
    }
    return false;
}

bool Election::addCandidateToRegion(const std::string& name, std::shared_ptr<Party> party, std::shared_ptr<Region> region) {
    if (!region || !party) {
        std::cout << "Error: Invalid region or party provided.\n";
        return false;
    }
    
    // Check if candidate already exists in ANY region (global restriction)
    if (globalCandidateRegistry.find(name) != globalCandidateRegistry.end()) {
        std::cout << "Error: Candidate '" << name << "' is already registered in another region.\n";
        std::cout << "Same candidate cannot be enlisted in multiple regions.\n";
        return false;
    }
    
    // Check if party already has a candidate in this region
    if (region->hasPartyCandidate(party)) {
        std::cout << "Error: Party '" << party->getName() 
                  << "' already has a candidate in region '" << region->getName() << "'.\n";
        std::cout << "One party cannot have more than one candidate per region.\n";
        return false;
    }
    
    auto candidate = std::make_shared<Candidate>(name, party, region);
    if (region->addCandidate(candidate)) {
        party->addMember(name);
        // Add to global registry
        globalCandidateRegistry[name].insert(region->getCode());
        std::cout << "Candidate '" << name << "' added to party '" << party->getName() 
                  << "' in region '" << region->getName() << "'.\n";
        return true;
    }
    return false;
}

// Voter management - Enhanced with comprehensive validation
bool Election::registerVoter(const std::string& firstName, const std::string& lastName,
                             const std::string& phoneNumber, const std::string& address,
                             const std::string& uniqueIdStr, const std::string& ageStr) {
    
    std::cout << "\n=== Validating Voter Registration Data ===\n";
    
    // Comprehensive input validation
    std::string validationErrors = InputValidator::validateVoterInput(
        firstName, lastName, phoneNumber, address, uniqueIdStr, ageStr);
    
    if (!validationErrors.empty()) {
        std::cout << "\n[VALIDATION FAILED]\n" << validationErrors;
        std::cout << "\nPlease correct the above errors and try again.\n";
        return false;
    }
    
    // Convert validated strings to appropriate types
    int uniqueId, age;
    try {
        uniqueId = std::stoi(InputValidator::trim(uniqueIdStr));
        age = std::stoi(InputValidator::trim(ageStr));
    } catch (const std::exception&) {
        std::cout << "[ERROR] Invalid number format in ID or age.\n";
        return false;
    }
    
    // Check for duplicate voter ID
    if (registeredVoterIds.count(uniqueId) > 0) {
        std::cout << "[ERROR] Voter with ID " << uniqueId << " already exists.\n";
        return false;
    }
    
    // All validations passed - register the voter
    auto voter = std::make_unique<Voter>(
        InputValidator::trim(firstName), 
        InputValidator::trim(lastName), 
        InputValidator::trim(phoneNumber), 
        InputValidator::trim(address), 
        uniqueId, 
        age
    );
    
    voters.push_back(std::move(voter));
    registeredVoterIds.insert(uniqueId);
    
    std::cout << "\n[SUCCESS] Voter '" << firstName << " " << lastName 
              << "' registered successfully with ID: " << uniqueId << "\n";
    std::cout << "Validation passed: All input requirements met.\n";
    return true;
}

// Legacy method for backward compatibility
bool Election::registerVoter(const std::string& firstName, const std::string& lastName,
                             const std::string& phoneNumber, const std::string& address,
                             int uniqueId, int age) {
    // Convert to string format and use the enhanced validation method
    return registerVoter(firstName, lastName, phoneNumber, address, 
                        std::to_string(uniqueId), std::to_string(age));
}

// Regional voter registration
bool Election::registerVoterInRegion(const std::string& firstName, const std::string& lastName,
                                     const std::string& phoneNumber, const std::string& address,
                                     const std::string& uniqueIdStr, const std::string& ageStr,
                                     std::shared_ptr<Region> region) {
    if (!region) {
        std::cout << "Error: Invalid region provided for voter registration.\n";
        return false;
    }
    
    std::cout << "\n=== Validating Voter Registration Data for Region: " << region->getName() << " ===\n";
    
    // Comprehensive input validation
    std::string validationErrors = InputValidator::validateVoterInput(
        firstName, lastName, phoneNumber, address, uniqueIdStr, ageStr);
    
    if (!validationErrors.empty()) {
        std::cout << "\n[VALIDATION FAILED]\n" << validationErrors;
        std::cout << "\nPlease correct the above errors and try again.\n";
        return false;
    }
    
    // Convert validated strings to appropriate types
    int uniqueId, age;
    try {
        uniqueId = std::stoi(InputValidator::trim(uniqueIdStr));
        age = std::stoi(InputValidator::trim(ageStr));
    } catch (const std::exception&) {
        std::cout << "[ERROR] Invalid number format in ID or age.\n";
        return false;
    }
    
    // Check for duplicate voter ID globally
    if (registeredVoterIds.count(uniqueId) > 0) {
        std::cout << "[ERROR] Voter with ID " << uniqueId << " already exists.\n";
        return false;
    }
    
    // Create voter with region assignment
    auto voter = std::make_shared<Voter>(
        InputValidator::trim(firstName), 
        InputValidator::trim(lastName), 
        InputValidator::trim(phoneNumber), 
        InputValidator::trim(address), 
        uniqueId, 
        age,
        region
    );
    
    // Add voter to region
    region->addVoter(voter);
    registeredVoterIds.insert(uniqueId);
    
    std::cout << "\n[SUCCESS] Voter '" << firstName << " " << lastName 
              << "' registered successfully in region '" << region->getName() 
              << "' with ID: " << uniqueId << "\n";
    std::cout << "Validation passed: All input requirements met.\n";
    return true;
}

// Voting functionality
bool Election::castVote(int voterId, int candidateIndex) {
    // Find voter
    Voter* voter = findVoterByUniqueId(voterId);
    if (!voter) {
        std::cout << "Voting failed: Voter with ID " << voterId << " is not registered.\n";
        return false;
    }
    
    // Check if voter has already voted
    if (voter->hasVoted()) {
        std::cout << "Voting failed: Voter " << voter->getFullName() << " has already voted.\n";
        return false;
    }
    
    // Validate candidate index
    if (candidateIndex < 0 || candidateIndex >= static_cast<int>(candidates.size())) {
        std::cout << "Voting failed: Invalid candidate index.\n";
        return false;
    }
    
    // Cast vote
    candidates[candidateIndex]->receiveVote();
    voter->markAsVoted();
    
    std::cout << "Vote cast successfully by " << voter->getFullName() 
              << " for " << candidates[candidateIndex]->getName() << "\n";
    return true;
}

// Regional voting functionality
bool Election::castVoteInRegion(int voterId, int candidateIndex, std::shared_ptr<Region> region) {
    if (!region) {
        std::cout << "Voting failed: Invalid region provided.\n";
        return false;
    }
    
    // Check if voter can vote in this region
    if (!region->canVoterVoteInRegion(voterId)) {
        std::cout << "Voting failed: Voter with ID " << voterId 
                  << " is not registered to vote in region '" << region->getName() << "'.\n";
        std::cout << "Voters can only vote in their assigned region.\n";
        return false;
    }
    
    // Find voter in region
    const auto& votersInRegion = region->getVoters();
    std::shared_ptr<Voter> voter = nullptr;
    for (const auto& v : votersInRegion) {
        if (v->getUniqueId() == voterId) {
            voter = v;
            break;
        }
    }
    
    if (!voter) {
        std::cout << "Voting failed: Voter with ID " << voterId 
                  << " not found in region '" << region->getName() << "'.\n";
        return false;
    }
    
    // Check if voter has already voted
    if (voter->hasVoted()) {
        std::cout << "Voting failed: Voter " << voter->getFullName() << " has already voted.\n";
        return false;
    }
    
    // Get candidates in this region
    const auto& candidatesInRegion = region->getCandidates();
    if (candidateIndex < 0 || candidateIndex >= static_cast<int>(candidatesInRegion.size())) {
        std::cout << "Voting failed: Invalid candidate index for region '" << region->getName() << "'.\n";
        return false;
    }
    
    // Cast vote for the candidate in this region
    candidatesInRegion[candidateIndex]->receiveVote();
    voter->markAsVoted();
    
    std::cout << "Vote cast successfully by " << voter->getFullName() 
              << " for " << candidatesInRegion[candidateIndex]->getName() 
              << " in region '" << region->getName() << "'\n";
    return true;
}

// Display methods
void Election::displayCandidates() const {
    if (candidates.empty()) {
        std::cout << "No candidates registered.\n";
        return;
    }
    
    std::cout << "\n=== Candidates ===\n";
    for (size_t i = 0; i < candidates.size(); ++i) {
        std::cout << i << ". " << candidates[i]->getName();
        if (candidates[i]->getParty()) {
            std::cout << " (" << candidates[i]->getParty()->getName() << ")";
        } else {
            std::cout << " (Independent)";
        }
        std::cout << " - Votes: " << candidates[i]->getVoteCount() << "\n";
    }
}

void Election::displayVoters() const {
    if (voters.empty()) {
        std::cout << "No voters registered.\n";
        return;
    }
    
    std::cout << "\n=== Registered Voters ===\n";
    for (const auto& voter : voters) {
        std::cout << "ID: " << voter->getUniqueId() 
                  << " | " << voter->getFullName() 
                  << " | Age: " << voter->getAge()
                  << " | Voted: " << (voter->hasVoted() ? "Yes" : "No") << "\n";
    }
}

void Election::displayResults() const {
    std::cout << "\n=== Election Results: " << title << " ===\n";
    
    if (candidates.empty()) {
        std::cout << "No candidates in this election.\n";
        return;
    }
    
    // Count total votes
    int totalVotes = 0;
    for (const auto& candidate : candidates) {
        totalVotes += candidate->getVoteCount();
    }
    
    std::cout << "Total votes cast: " << totalVotes << "\n";
    std::cout << "Total registered voters: " << voters.size() << "\n\n";
    
    // Display results sorted by vote count
    std::vector<std::pair<int, std::string>> results;
    for (const auto& candidate : candidates) {
        std::string candidateInfo = candidate->getName();
        if (candidate->getParty()) {
            candidateInfo += " (" + candidate->getParty()->getName() + ")";
        } else {
            candidateInfo += " (Independent)";
        }
        results.emplace_back(candidate->getVoteCount(), candidateInfo);
    }
    
    std::sort(results.rbegin(), results.rend());
    
    for (size_t i = 0; i < results.size(); ++i) {
        double percentage = totalVotes > 0 ? (static_cast<double>(results[i].first) / totalVotes) * 100.0 : 0.0;
        std::cout << i + 1 << ". " << results[i].second 
                  << " - " << results[i].first << " votes (" 
                  << std::fixed << std::setprecision(1) << percentage << "%)\n";
    }
}

// Regional display methods
void Election::displayCandidatesInRegion(std::shared_ptr<Region> region) const {
    if (!region) {
        std::cout << "Error: Invalid region provided.\n";
        return;
    }
    
    region->displayCandidatesInRegion();
}

void Election::displayVotersInRegion(std::shared_ptr<Region> region) const {
    if (!region) {
        std::cout << "Error: Invalid region provided.\n";
        return;
    }
    
    region->displayVotersInRegion();
}

void Election::displayResultsInRegion(std::shared_ptr<Region> region) const {
    if (!region) {
        std::cout << "Error: Invalid region provided.\n";
        return;
    }
    
    std::cout << "\n=== Election Results for Region: " << region->getName() << " ===\n";
    
    const auto& candidatesInRegion = region->getCandidates();
    if (candidatesInRegion.empty()) {
        std::cout << "No candidates in this region.\n";
        return;
    }
    
    // Count total votes in this region
    int totalVotes = 0;
    for (const auto& candidate : candidatesInRegion) {
        totalVotes += candidate->getVoteCount();
    }
    
    std::cout << "Total votes cast in region: " << totalVotes << "\n";
    std::cout << "Total registered voters in region: " << region->getVoters().size() << "\n\n";
    
    // Display results sorted by vote count
    std::vector<std::pair<int, std::string>> results;
    for (const auto& candidate : candidatesInRegion) {
        std::string candidateInfo = candidate->getName();
        if (candidate->getParty()) {
            candidateInfo += " (" + candidate->getParty()->getName() + ")";
        } else {
            candidateInfo += " (Independent)";
        }
        results.emplace_back(candidate->getVoteCount(), candidateInfo);
    }
    
    std::sort(results.rbegin(), results.rend());
    
    for (size_t i = 0; i < results.size(); ++i) {
        double percentage = totalVotes > 0 ? (static_cast<double>(results[i].first) / totalVotes) * 100.0 : 0.0;
        std::cout << i + 1 << ". " << results[i].second 
                  << " - " << results[i].first << " votes (" 
                  << std::fixed << std::setprecision(1) << percentage << "%)\n";
    }
}

void Election::saveResultsToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }
    
    file << "Election Results: " << title << "\n";
    file << "================================\n\n";
    
    int totalVotes = 0;
    for (const auto& candidate : candidates) {
        totalVotes += candidate->getVoteCount();
    }
    
    file << "Total votes cast: " << totalVotes << "\n";
    file << "Total registered voters: " << voters.size() << "\n\n";
    
    for (const auto& candidate : candidates) {
        file << candidate->getName();
        if (candidate->getParty()) {
            file << " (" << candidate->getParty()->getName() << ")";
        } else {
            file << " (Independent)";
        }
        file << ": " << candidate->getVoteCount() << " votes\n";
    }
    
    file.close();
    std::cout << "Results saved to " << filename << "\n";
}

// Complete data persistence
void Election::saveCompleteElectionData(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }
    
    file << "COMPLETE_ELECTION_DATA\n";
    file << "======================\n\n";
    
    // Save election metadata
    file << "[ELECTION_INFO]\n";
    file << "Title=" << title << "\n";
    file << "TotalCandidates=" << candidates.size() << "\n";
    file << "TotalVoters=" << voters.size() << "\n";
    file << "TotalParties=" << parties.size() << "\n\n";
    
    // Save parties
    file << "[PARTIES]\n";
    for (size_t i = 0; i < parties.size(); ++i) {
        file << "Party" << i << "=" << parties[i]->getName() << "\n";
        file << "Party" << i << "_Members=";
        const auto& members = parties[i]->getMembers();
        for (size_t j = 0; j < members.size(); ++j) {
            file << members[j];
            if (j < members.size() - 1) file << ";";
        }
        file << "\n";
    }
    file << "\n";
    
    // Save candidates
    file << "[CANDIDATES]\n";
    for (size_t i = 0; i < candidates.size(); ++i) {
        file << "Candidate" << i << "_Name=" << candidates[i]->getName() << "\n";
        file << "Candidate" << i << "_Votes=" << candidates[i]->getVoteCount() << "\n";
        if (candidates[i]->getParty()) {
            file << "Candidate" << i << "_Party=" << candidates[i]->getParty()->getName() << "\n";
        } else {
            file << "Candidate" << i << "_Party=Independent\n";
        }
    }
    file << "\n";
    
    // Save voters
    file << "[VOTERS]\n";
    for (size_t i = 0; i < voters.size(); ++i) {
        const auto& voter = voters[i];
        file << "Voter" << i << "_FirstName=" << voter->getFirstName() << "\n";
        file << "Voter" << i << "_LastName=" << voter->getLastName() << "\n";
        file << "Voter" << i << "_Phone=" << voter->getPhoneNumber() << "\n";
        file << "Voter" << i << "_Address=" << voter->getAddress() << "\n";
        file << "Voter" << i << "_UniqueId=" << voter->getUniqueId() << "\n";
        file << "Voter" << i << "_Age=" << voter->getAge() << "\n";
        file << "Voter" << i << "_HasVoted=" << (voter->hasVoted() ? "1" : "0") << "\n";
    }
    file << "\n";
    
    // Save voting summary
    file << "[VOTING_SUMMARY]\n";
    int totalVotes = 0;
    for (const auto& candidate : candidates) {
        totalVotes += candidate->getVoteCount();
    }
    file << "TotalVotesCast=" << totalVotes << "\n";
    file << "VoterTurnout=" << std::fixed << std::setprecision(2) 
         << (voters.size() > 0 ? (static_cast<double>(totalVotes) / voters.size()) * 100.0 : 0.0) << "%\n";
    
    file.close();
    std::cout << "Complete election data saved to " << filename << "\n";
}

void Election::exportToCSV(const std::string& baseFilename) const {
    // Export candidates to CSV
    std::ofstream candidatesFile(baseFilename + "_candidates.csv");
    if (candidatesFile.is_open()) {
        candidatesFile << "Index,Name,Party,Votes,VotePercentage\n";
        int totalVotes = 0;
        for (const auto& candidate : candidates) {
            totalVotes += candidate->getVoteCount();
        }
        
        for (size_t i = 0; i < candidates.size(); ++i) {
            double percentage = totalVotes > 0 ? 
                (static_cast<double>(candidates[i]->getVoteCount()) / totalVotes) * 100.0 : 0.0;
            candidatesFile << i << ","
                          << candidates[i]->getName() << ","
                          << (candidates[i]->getParty() ? candidates[i]->getParty()->getName() : "Independent") << ","
                          << candidates[i]->getVoteCount() << ","
                          << std::fixed << std::setprecision(2) << percentage << "\n";
        }
        candidatesFile.close();
        std::cout << "Candidates exported to " << baseFilename << "_candidates.csv\n";
    }
    
    // Export voters to CSV
    std::ofstream votersFile(baseFilename + "_voters.csv");
    if (votersFile.is_open()) {
        votersFile << "UniqueId,FirstName,LastName,Age,Phone,Address,HasVoted\n";
        for (const auto& voter : voters) {
            votersFile << voter->getUniqueId() << ","
                      << voter->getFirstName() << ","
                      << voter->getLastName() << ","
                      << voter->getAge() << ","
                      << voter->getPhoneNumber() << ","
                      << "\"" << voter->getAddress() << "\","  // Quoted for addresses with commas
                      << (voter->hasVoted() ? "Yes" : "No") << "\n";
        }
        votersFile.close();
        std::cout << "Voters exported to " << baseFilename << "_voters.csv\n";
    }
    
    // Export parties to CSV
    std::ofstream partiesFile(baseFilename + "_parties.csv");
    if (partiesFile.is_open()) {
        partiesFile << "PartyName,MemberCount,Members\n";
        for (const auto& party : parties) {
            partiesFile << party->getName() << ","
                       << party->getMembers().size() << ",\"";
            const auto& members = party->getMembers();
            for (size_t i = 0; i < members.size(); ++i) {
                partiesFile << members[i];
                if (i < members.size() - 1) partiesFile << ";";
            }
            partiesFile << "\"\n";
        }
        partiesFile.close();
        std::cout << "Parties exported to " << baseFilename << "_parties.csv\n";
    }
    
    std::cout << "CSV files exported successfully!\n";
}

// Load complete election data from file
bool Election::loadCompleteElectionData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Warning: Could not open file " << filename << " for reading.\n";
        return false;
    }
    
    std::string line;
    std::string currentSection;
    
    // Clear existing data
    candidates.clear();
    voters.clear();
    parties.clear();
    registeredVoterIds.clear();
    
    std::cout << "Loading election data from " << filename << "...\n";
    
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '=' || line == "COMPLETE_ELECTION_DATA") {
            continue;
        }
        
        // Check for section headers
        if (line.front() == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }
        
        // Parse key=value pairs
        size_t equalPos = line.find('=');
        if (equalPos == std::string::npos) continue;
        
        std::string key = line.substr(0, equalPos);
        std::string value = line.substr(equalPos + 1);
        
        if (currentSection == "ELECTION_INFO") {
            if (key == "Title") {
                title = value;
            }
        }
        else if (currentSection == "PARTIES") {
            if (key.find("Party") == 0 && key.find("_Members") == std::string::npos) {
                // Extract party index and create party
                size_t partyIndex = std::stoul(key.substr(5)); // Remove "Party" prefix
                if (partyIndex >= parties.size()) {
                    parties.resize(partyIndex + 1);
                }
                parties[partyIndex] = std::make_unique<Party>(value);
            }
        }
        else if (currentSection == "CANDIDATES") {
            // Parse candidate data
            if (key.find("_Name") != std::string::npos) {
                size_t candidateIndex = std::stoul(key.substr(9, key.find("_") - 9)); // Extract index
                if (candidateIndex >= candidates.size()) {
                    candidates.resize(candidateIndex + 1);
                }
                candidates[candidateIndex] = std::make_unique<Candidate>(value);
            }
            else if (key.find("_Votes") != std::string::npos) {
                size_t candidateIndex = std::stoul(key.substr(9, key.find("_") - 9));
                if (candidateIndex < candidates.size() && candidates[candidateIndex]) {
                    int votes = std::stoi(value);
                    for (int i = 0; i < votes; ++i) {
                        candidates[candidateIndex]->receiveVote();
                    }
                }
            }
            else if (key.find("_Party") != std::string::npos) {
                size_t candidateIndex = std::stoul(key.substr(9, key.find("_") - 9));
                if (candidateIndex < candidates.size() && candidates[candidateIndex] && value != "Independent") {
                    // Find party by name and assign
                    for (const auto& party : parties) {
                        if (party && party->getName() == value) {
                            candidates[candidateIndex]->setParty(party);
                            party->addMember(candidates[candidateIndex]->getName());
                            break;
                        }
                    }
                }
            }
        }
        else if (currentSection == "VOTERS") {
            // Parse voter data - collect all fields for a voter
            static std::map<int, std::map<std::string, std::string>> voterData;
            
            if (key.find("Voter") == 0) {
                size_t underscorePos = key.find('_');
                if (underscorePos != std::string::npos) {
                    int voterIndex = std::stoi(key.substr(5, underscorePos - 5));
                    std::string field = key.substr(underscorePos + 1);
                    voterData[voterIndex][field] = value;
                    
                    // If we have all required fields, create the voter
                    auto& data = voterData[voterIndex];
                    if (data.count("FirstName") && data.count("LastName") && 
                        data.count("Phone") && data.count("Address") && 
                        data.count("UniqueId") && data.count("Age") && 
                        data.count("HasVoted")) {
                        
                        int uniqueId = std::stoi(data["UniqueId"]);
                        int age = std::stoi(data["Age"]);
                        bool hasVoted = (data["HasVoted"] == "1");
                        
                        auto voter = std::make_unique<Voter>(
                            data["FirstName"], data["LastName"], 
                            data["Phone"], data["Address"], 
                            uniqueId, age
                        );
                        
                        if (hasVoted) {
                            voter->markAsVoted();
                        }
                        
                        registeredVoterIds.insert(uniqueId);
                        voters.push_back(std::move(voter));
                    }
                }
            }
        }
    }
    
    file.close();
    
    // Remove null pointers from candidates and parties
    candidates.erase(std::remove(candidates.begin(), candidates.end(), nullptr), candidates.end());
    parties.erase(std::remove(parties.begin(), parties.end(), nullptr), parties.end());
    
    std::cout << "Election data loaded successfully!\n";
    std::cout << "Loaded: " << parties.size() << " parties, " 
              << candidates.size() << " candidates, " 
              << voters.size() << " voters\n";
    
    return true;
}

// Getters
const std::string& Election::getTitle() const {
    return title;
}

// Validation helpers
bool Election::isVoterRegistered(int uniqueId) const {
    return registeredVoterIds.count(uniqueId) > 0;
}

bool Election::hasVoterVoted(int uniqueId) const {
    Voter* voter = const_cast<Election*>(this)->findVoterByUniqueId(uniqueId);
    return voter && voter->hasVoted();
}

// Regional validation helpers
bool Election::isVoterInRegion(int uniqueId, std::shared_ptr<Region> region) const {
    if (!region) return false;
    return region->hasVoter(uniqueId);
}

bool Election::canCandidateBeAddedToRegion(const std::string& candidateName, std::shared_ptr<Party> party, std::shared_ptr<Region> region) const {
    if (!region) return false;
    
    // Check if candidate already exists in ANY region
    if (globalCandidateRegistry.find(candidateName) != globalCandidateRegistry.end()) {
        return false;
    }
    
    // Check if party already has a candidate in this region
    if (party && region->hasPartyCandidate(party)) {
        return false;
    }
    
    return true;
}

// Helper methods
Voter* Election::findVoterByUniqueId(int uniqueId) {
    auto it = std::find_if(voters.begin(), voters.end(),
        [uniqueId](const std::unique_ptr<Voter>& voter) {
            return voter->getUniqueId() == uniqueId;
        });
    return (it != voters.end()) ? it->get() : nullptr;
}

std::shared_ptr<Candidate> Election::findCandidateInRegion(const std::string& candidateName, std::shared_ptr<Region> region) const {
    if (!region) return nullptr;
    
    const auto& candidates = region->getCandidates();
    for (const auto& candidate : candidates) {
        if (candidate->getName() == candidateName) {
            return candidate;
        }
    }
    return nullptr;
}