#pragma once
#include <vector>
#include <memory>
#include <unordered_set>
#include "Candidate.h"
#include "Voter.h"
#include "party.h"
#include "InputValidator.h"

class Election {
    public:
        Election(const std::string& title);

        // Candidate management
        void addCandidate(const std::string& name);
        void addCandidate(const std::string& name, std::shared_ptr<Party> party);
        
        // Party management
        std::shared_ptr<Party> createParty(const std::string& partyName);
        void displayParties() const;
        std::shared_ptr<Party> getPartyByIndex(size_t index) const;
        
        // Voter management - Enhanced with validation
        bool registerVoter(const std::string& firstName, const std::string& lastName,
                           const std::string& phoneNumber, const std::string& address,
                           const std::string& uniqueIdStr, const std::string& ageStr);
        
        // Legacy method for backward compatibility
        bool registerVoter(const std::string& firstName, const std::string& lastName,
                           const std::string& phoneNumber, const std::string& address,
                           int uniqueId, int age);
        
        // Voting functionality
        bool castVote(int voterId, int candidateIndex);
        
        // Display and reporting
        void displayCandidates() const;
        void displayVoters() const;
        void displayResults() const;
        void saveResultsToFile(const std::string& filename) const;
        
        // Getters
        const std::string& getTitle() const;
        
        // Complete data persistence
        void saveCompleteElectionData(const std::string& filename) const;
        bool loadCompleteElectionData(const std::string& filename);
        void exportToCSV(const std::string& baseFilename) const;
        
        // Validation helpers
        bool isVoterRegistered(int uniqueId) const;
        bool hasVoterVoted(int uniqueId) const;

    private:
        std::string title;
        std::vector<std::unique_ptr<Candidate>> candidates;
        std::vector<std::unique_ptr<Voter>> voters;
        std::vector<std::shared_ptr<Party>> parties;
        std::unordered_set<int> registeredVoterIds; // For quick duplicate checking
        
        // Helper methods
        Voter* findVoterByUniqueId(int uniqueId);
};