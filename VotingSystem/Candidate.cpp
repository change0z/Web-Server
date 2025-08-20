#include "Candidate.h"

Candidate::Candidate(const std::string& name) : name(name), party(nullptr), votes(0) {}

Candidate::Candidate(const std::string& name, std::shared_ptr<Party> party) : name(name), party(party), votes(0) {}

std::string Candidate::getName() const {
    return name;
}

std::shared_ptr<Party> Candidate::getParty() const {
    return party;
}

void Candidate::addVote() {
    votes++;
}

int Candidate::getVotes() const {
    return votes;
}