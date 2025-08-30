#include "Region.h"
#include "../core/Candidate.h"
#include "../core/Voter.h"
#include "../core/party.h"
#include <iostream>
#include <algorithm>

Region::Region(const std::string& name, const std::string& code) 
    : name(name), code(code) {}

std::string Region::getName() const {
    return name;
}

std::string Region::getCode() const {
    return code;
}

bool Region::addCandidate(std::shared_ptr<Candidate> candidate) {
    if (!candidate) {
        std::cout << "Error: Cannot add null candidate to region.\n";
        return false;
    }
    
    // Check if candidate already exists in this region
    for (const auto& existingCandidate : candidates) {
        if (existingCandidate->getName() == candidate->getName()) {
            std::cout << "Error: Candidate '" << candidate->getName() 
                      << "' is already registered in region '" << name << "'.\n";
            return false;
        }
    }
    
    // Check if party already has a candidate in this region
    if (candidate->getParty() && hasPartyCandidate(candidate->getParty())) {
        std::cout << "Error: Party '" << candidate->getParty()->getName() 
                  << "' already has a candidate in region '" << name << "'.\n";
        return false;
    }
    
    candidates.push_back(candidate);
    
    // Register the party in this region if not already registered
    if (candidate->getParty()) {
        registerParty(candidate->getParty());
    }
    
    std::cout << "Candidate '" << candidate->getName() 
              << "' successfully registered in region '" << name << "'.\n";
    return true;
}

bool Region::hasPartyCandidate(std::shared_ptr<Party> party) const {
    if (!party) return false;
    
    for (const auto& candidate : candidates) {
        if (candidate->getParty() && candidate->getParty()->getName() == party->getName()) {
            return true;
        }
    }
    return false;
}

const std::vector<std::shared_ptr<Candidate>>& Region::getCandidates() const {
    return candidates;
}

void Region::addVoter(std::shared_ptr<Voter> voter) {
    if (!voter) {
        std::cout << "Error: Cannot add null voter to region.\n";
        return;
    }
    
    int voterId = voter->getUniqueId();
    if (voterIds.find(voterId) != voterIds.end()) {
        std::cout << "Error: Voter with ID " << voterId 
                  << " is already registered in region '" << name << "'.\n";
        return;
    }
    
    voters.push_back(voter);
    voterIds.insert(voterId);
    
    std::cout << "Voter '" << voter->getFullName() 
              << "' assigned to region '" << name << "'.\n";
}

bool Region::hasVoter(int voterId) const {
    return voterIds.find(voterId) != voterIds.end();
}

const std::vector<std::shared_ptr<Voter>>& Region::getVoters() const {
    return voters;
}

void Region::registerParty(std::shared_ptr<Party> party) {
    if (!party || hasParty(party)) {
        return; // Already registered or null party
    }
    
    parties.push_back(party);
}

bool Region::hasParty(std::shared_ptr<Party> party) const {
    if (!party) return false;
    
    for (const auto& existingParty : parties) {
        if (existingParty->getName() == party->getName()) {
            return true;
        }
    }
    return false;
}

const std::vector<std::shared_ptr<Party>>& Region::getParties() const {
    return parties;
}

bool Region::canVoterVoteInRegion(int voterId) const {
    return hasVoter(voterId);
}

bool Region::isCandidateInRegion(std::shared_ptr<Candidate> candidate) const {
    if (!candidate) return false;
    
    for (const auto& existingCandidate : candidates) {
        if (existingCandidate->getName() == candidate->getName()) {
            return true;
        }
    }
    return false;
}

void Region::displayRegionInfo() const {
    std::cout << "\n=== Region: " << name << " (" << code << ") ===\n";
    std::cout << "Parties: " << parties.size() << "\n";
    std::cout << "Candidates: " << candidates.size() << "\n";
    std::cout << "Registered Voters: " << voters.size() << "\n";
}

void Region::displayCandidatesInRegion() const {
    std::cout << "\n=== Candidates in " << name << " ===\n";
    if (candidates.empty()) {
        std::cout << "No candidates registered in this region.\n";
        return;
    }
    
    for (size_t i = 0; i < candidates.size(); ++i) {
        std::cout << i + 1 << ". " << candidates[i]->getName();
        if (candidates[i]->getParty()) {
            std::cout << " (" << candidates[i]->getParty()->getName() << ")";
        } else {
            std::cout << " (Independent)";
        }
        std::cout << " - Votes: " << candidates[i]->getVoteCount() << "\n";
    }
}

void Region::displayVotersInRegion() const {
    std::cout << "\n=== Voters in " << name << " ===\n";
    if (voters.empty()) {
        std::cout << "No voters registered in this region.\n";
        return;
    }
    
    for (const auto& voter : voters) {
        std::cout << "- " << voter->getFullName() 
                  << " (ID: " << voter->getUniqueId() << ")";
        if (voter->hasVoted()) {
            std::cout << " [VOTED]";
        }
        std::cout << "\n";
    }
}
