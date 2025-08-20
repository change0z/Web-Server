#pragma once
#include <vector>
#include <memory>
#include "Candidate.h"
#include "Voter.h"
#include "party.h"

class Election {
    public:
        Election(const std::string& title);

        void addCandidate(const std::string& name);
        void addCandidate(const std::string& name, std::shared_ptr<Party> party);
        std::shared_ptr<Party> createParty(const std::string& partyName);
        void registerVoter(const std::string& name, int id);
        void castVote(int voterId, int candidateIndex);
        void displayResults() const;
        void saveResultsToFile(const std::string& filename) const;

    private:
        std::string title;
        std::vector<std::unique_ptr<Candidate>> candidates;
        std::vector<std::unique_ptr<Voter>> voters;
        std::vector<std::shared_ptr<Party>> parties;
};