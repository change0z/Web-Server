#pragma once
#include <string>
#include <memory>
#include "party.h"

class Candidate {
    public:
        Candidate(const std::string& name);
        Candidate(const std::string& name, std::shared_ptr<Party> party);
        
        std::string getName() const;
        std::shared_ptr<Party> getParty() const;
        void setParty(std::shared_ptr<Party> party);
        void receiveVote();
        int getVoteCount() const;

    private:
        std::string name;
        std::shared_ptr<Party> party;
        int votes;
};