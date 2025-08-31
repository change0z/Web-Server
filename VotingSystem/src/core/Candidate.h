#pragma once
#include <string>
#include <memory>
#include "party.h"

// Forward declaration
class Region;

class Candidate {
    public:
        Candidate(const std::string& name);
        Candidate(const std::string& name, std::shared_ptr<Party> party);
        
        // Constructor with region assignment
        Candidate(const std::string& name, std::shared_ptr<Party> party, std::shared_ptr<Region> region);
        
        std::string getName() const;
        std::shared_ptr<Party> getParty() const;
        void setParty(std::shared_ptr<Party> party);
        void receiveVote();
        int getVoteCount() const;
        
        // Region-related methods
        std::shared_ptr<Region> getRegion() const;
        void setRegion(std::shared_ptr<Region> region);
        bool isInRegion(std::shared_ptr<Region> region) const;

    private:
        std::string name;
        std::shared_ptr<Party> party;
        int votes;
        std::shared_ptr<Region> assignedRegion;
};