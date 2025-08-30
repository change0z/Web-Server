#pragma once
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "Candidate.h"
#include "Voter.h"
#include "party.h"
#include "../regional/Region.h"
#include "../validation/InputValidator.h"

class Election {
    public:
        Election(const std::string& title);

        // Region management
        std::shared_ptr<Region> createRegion(const std::string& name, const std::string& code);
        void displayRegions() const;
        std::shared_ptr<Region> getRegionByIndex(size_t index) const;
        std::shared_ptr<Region> getRegionByCode(const std::string& code) const;
        
        // Candidate management with region support
        void addCandidate(const std::string& name);  // Legacy method
        void addCandidate(const std::string& name, std::shared_ptr<Party> party);  // Legacy method
        bool addCandidateToRegion(const std::string& name, std::shared_ptr<Region> region);
        bool addCandidateToRegion(const std::string& name, std::shared_ptr<Party> party, std::shared_ptr<Region> region);
        
        // Party management
        std::shared_ptr<Party> createParty(const std::string& partyName);
        void displayParties() const;
        std::shared_ptr<Party> getPartyByIndex(size_t index) const;
        
        // Voter management with region support - Enhanced with validation
        bool registerVoter(const std::string& firstName, const std::string& lastName,
                           const std::string& phoneNumber, const std::string& address,
                           const std::string& uniqueIdStr, const std::string& ageStr);
        
        bool registerVoterInRegion(const std::string& firstName, const std::string& lastName,
                                   const std::string& phoneNumber, const std::string& address,
                                   const std::string& uniqueIdStr, const std::string& ageStr,
                                   std::shared_ptr<Region> region);
        
        // Legacy method for backward compatibility
        bool registerVoter(const std::string& firstName, const std::string& lastName,
                           const std::string& phoneNumber, const std::string& address,
                           int uniqueId, int age);
        
        // Regional voting functionality
        bool castVoteInRegion(int voterId, int candidateIndex, std::shared_ptr<Region> region);
        bool castVote(int voterId, int candidateIndex);  // Legacy method
        
        // Display and reporting
        void displayCandidates() const;  // Shows all candidates globally
        void displayCandidatesInRegion(std::shared_ptr<Region> region) const;
        void displayVoters() const;  // Shows all voters globally
        void displayVotersInRegion(std::shared_ptr<Region> region) const;
        void displayResults() const;  // Shows global results
        void displayResultsInRegion(std::shared_ptr<Region> region) const;
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
        bool isVoterInRegion(int uniqueId, std::shared_ptr<Region> region) const;
        bool canCandidateBeAddedToRegion(const std::string& candidateName, std::shared_ptr<Party> party, std::shared_ptr<Region> region) const;

    private:
        std::string title;
        std::vector<std::unique_ptr<Candidate>> candidates;  // Global candidate list for legacy support
        std::vector<std::unique_ptr<Voter>> voters;  // Global voter list for legacy support
        std::vector<std::shared_ptr<Party>> parties;
        std::vector<std::shared_ptr<Region>> regions;
        std::unordered_set<int> registeredVoterIds; // For quick duplicate checking
        std::unordered_map<std::string, std::unordered_set<std::string>> globalCandidateRegistry; // candidateName -> regions where registered
        
        // Helper methods
        Voter* findVoterByUniqueId(int uniqueId);
        std::shared_ptr<Candidate> findCandidateInRegion(const std::string& candidateName, std::shared_ptr<Region> region) const;
};