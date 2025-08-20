#include "Election.h"
#include <iostream>
#include <fstream>

Election::Election(const std::string& title) : title(title) {}

void Election::addCandidate(const std::string& name) {
    candidates.push_back(std::make_unique<Candidate>(name));
}

void Election::addCandidate(const std::string& name, std::shared_ptr<Party> party) {
    candidates.push_back(std::make_unique<Candidate>(name, party));
}

std::shared_ptr<Party> Election::createParty(const std::string& partyName) {
    auto party = std::make_shared<Party>(partyName);
    parties.push_back(party);
    return party;
}

void Election::registerVoter(const std::string& name, int id) {
    voters.push_back(std::make_unique<Voter>(name,id));
}

void Election::castVote(int voterID, int candidateIndex) {
    if (candidateIndex < 0 || candidateIndex >= static_cast<int>(candidates.size())) {
        std::cout << "Invalid candidate index. \n";
        return;
    }
    candidates[candidateIndex]->addVote();
    std::cout << "Vote recorded for " << candidates[candidateIndex]->getName() << "\n"; 
}

void Election::displayResults() const {
    std::cout << "\nElection Results for: " << title << "\n";
    for (const auto& c : candidates) {
        std::cout << c->getName();
        if (c->getParty()) {
            std::cout << " (" << c->getParty()->getName() << ")";
        }
        std::cout << ": " << c->getVotes() << " votes\n";
    }
}

void Election::saveResultsToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        std::cout << "Error opening file.\n";
        return;
    }
    file << "Election Results for: " << title << "\n";
    for (const auto& c : candidates) {
        file << c->getName();
        if (c->getParty()) {
            file << " (" << c->getParty()->getName() << ")";
        }
        file << ": " << c->getVotes() << " votes\n";
    }
}