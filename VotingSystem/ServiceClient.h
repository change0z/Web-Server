#pragma once
#include "ClearBallotService.h"
#include <memory>
#include <string>

// Client interface for communicating with ClearBallotService
class ServiceClient {
public:
    ServiceClient();
    ~ServiceClient();
    
    // Connection management
    bool connectToService();
    bool isConnected() const;
    void disconnect();
    
    // Election management
    int createElection(const std::string& title);
    bool electionExists(int electionId);
    
    // Voter operations
    ServiceResponse registerVoter(int electionId, const std::string& firstName,
                                 const std::string& lastName, const std::string& phone,
                                 const std::string& address, const std::string& idStr,
                                 const std::string& ageStr);
    
    ServiceResponse castVote(int electionId, int voterId, int candidateIndex);
    ServiceResponse getCandidates(int electionId);
    ServiceResponse getVoters(int electionId);
    ServiceResponse getElectionResults(int electionId);
    ServiceResponse checkVoterRegistration(int electionId, int voterId);
    
    // Admin operations
    ServiceResponse addCandidate(int electionId, const std::string& name);
    ServiceResponse addCandidateWithParty(int electionId, const std::string& name,
                                         const std::string& partyName);
    ServiceResponse createParty(int electionId, const std::string& partyName);
    ServiceResponse getParties(int electionId);
    
    // Data persistence operations
    ServiceResponse saveElectionResults(int electionId, const std::string& filename);
    ServiceResponse saveCompleteElectionData(int electionId, const std::string& filename);
    ServiceResponse loadElectionData(int electionId, const std::string& filename);
    ServiceResponse exportElectionToCSV(int electionId, const std::string& baseFilename);
    
    // Direct Election access (for compatibility with existing console interfaces)
    std::shared_ptr<Election> getElection(int electionId);
    
private:
    std::shared_ptr<ClearBallotService> service;  // For initial implementation (local service)
    bool connected;
    int defaultElectionId;  // For single-election console apps
    
    // Helper methods
    ServiceResponse createErrorResponse(const std::string& message) const;
    bool ensureDefaultElection();
};
