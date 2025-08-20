#include "Election.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>

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

// Voter management
bool Election::registerVoter(const std::string& firstName, const std::string& lastName,
                             const std::string& phoneNumber, const std::string& address,
                             int uniqueId, int age) {
    // Age validation
    if (age < 18) {
        std::cout << "Registration failed: Voter must be 18 or older. Current age: " << age << "\n";
        return false;
    }
    
    // Duplicate ID check
    if (registeredVoterIds.count(uniqueId) > 0) {
        std::cout << "Registration failed: Voter with ID " << uniqueId << " already exists.\n";
        return false;
    }
    
    // Register the voter
    auto voter = std::make_unique<Voter>(firstName, lastName, phoneNumber, address, uniqueId, age);
    voters.push_back(std::move(voter));
    registeredVoterIds.insert(uniqueId);
    
    std::cout << "Voter '" << firstName << " " << lastName << "' registered successfully with ID: " << uniqueId << "\n";
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

// Validation helpers
bool Election::isVoterRegistered(int uniqueId) const {
    return registeredVoterIds.count(uniqueId) > 0;
}

bool Election::hasVoterVoted(int uniqueId) const {
    Voter* voter = const_cast<Election*>(this)->findVoterByUniqueId(uniqueId);
    return voter && voter->hasVoted();
}

// Helper methods
Voter* Election::findVoterByUniqueId(int uniqueId) {
    auto it = std::find_if(voters.begin(), voters.end(),
        [uniqueId](const std::unique_ptr<Voter>& voter) {
            return voter->getUniqueId() == uniqueId;
        });
    return (it != voters.end()) ? it->get() : nullptr;
}