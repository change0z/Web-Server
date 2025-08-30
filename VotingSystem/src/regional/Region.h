#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

// Forward declarations to avoid circular dependencies
class Candidate;
class Voter;
class Party;

class Region {
public:
    Region(const std::string& name, const std::string& code);

    // Basic getters
    std::string getName() const;
    std::string getCode() const;
    
    // Candidate management for this region
    bool addCandidate(std::shared_ptr<Candidate> candidate);
    bool hasPartyCandidate(std::shared_ptr<Party> party) const;
    const std::vector<std::shared_ptr<Candidate>>& getCandidates() const;
    
    // Voter management for this region
    void addVoter(std::shared_ptr<Voter> voter);
    bool hasVoter(int voterId) const;
    const std::vector<std::shared_ptr<Voter>>& getVoters() const;
    
    // Party presence in region
    void registerParty(std::shared_ptr<Party> party);
    bool hasParty(std::shared_ptr<Party> party) const;
    const std::vector<std::shared_ptr<Party>>& getParties() const;
    
    // Validation methods
    bool canVoterVoteInRegion(int voterId) const;
    bool isCandidateInRegion(std::shared_ptr<Candidate> candidate) const;
    
    // Display methods
    void displayRegionInfo() const;
    void displayCandidatesInRegion() const;
    void displayVotersInRegion() const;

private:
    std::string name;
    std::string code;
    std::vector<std::shared_ptr<Candidate>> candidates;
    std::vector<std::shared_ptr<Voter>> voters;
    std::vector<std::shared_ptr<Party>> parties;
    std::unordered_set<int> voterIds; // For quick lookup
};
