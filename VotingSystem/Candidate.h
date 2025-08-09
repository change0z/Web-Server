#pragma once
#include <string>

class Candidate {
    public:
        Candidate(const std::string& name);
        std::string getName() const;
        void addVote();
        int getVotes() const;

    private:
        std::string name;
        int votes;
};