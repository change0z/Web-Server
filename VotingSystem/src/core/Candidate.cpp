#include "Candidate.h"
#include "../regional/Region.h"

Candidate::Candidate(const std::string& name) : name(name), party(nullptr), votes(0), assignedRegion(nullptr) {}

Candidate::Candidate(const std::string& name, std::shared_ptr<Party> party) 
    : name(name), party(party), votes(0), assignedRegion(nullptr) {}

Candidate::Candidate(const std::string& name, std::shared_ptr<Party> party, std::shared_ptr<Region> region) 
    : name(name), party(party), votes(0), assignedRegion(region) {}

std::string Candidate::getName() const {
    return name;
}

std::shared_ptr<Party> Candidate::getParty() const {
    return party;
}

void Candidate::setParty(std::shared_ptr<Party> party) {
    this->party = party;
}

void Candidate::receiveVote() {
    votes++;
}

int Candidate::getVoteCount() const {
    return votes;
}

std::shared_ptr<Region> Candidate::getRegion() const {
    return assignedRegion;
}

void Candidate::setRegion(std::shared_ptr<Region> region) {
    assignedRegion = region;
}

bool Candidate::isInRegion(std::shared_ptr<Region> region) const {
    return assignedRegion && region && assignedRegion == region;
}