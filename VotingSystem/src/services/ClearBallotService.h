#pragma once
#include "../core/Election.h"
#include <memory>
#include <map>
#include <mutex>
#include <string>
#include <vector>

// Service response structure for client communication
struct ServiceResponse {
    bool success;
    std::string message;
    std::vector<std::string> data;
    
    ServiceResponse(bool success = false, const std::string& message = "")
        : success(success), message(message) {}
};

// Voter registration data structure
struct VoterRegistrationData {
    std::string firstName;
    std::string lastName;
    std::string phoneNumber;
    std::string address;
    std::string uniqueIdStr;
    std::string ageStr;
    
    VoterRegistrationData(const std::string& firstName, const std::string& lastName,
                         const std::string& phone, const std::string& address,
                         const std::string& idStr, const std::string& ageStr)
        : firstName(firstName), lastName(lastName), phoneNumber(phone),
          address(address), uniqueIdStr(idStr), ageStr(ageStr) {}
};

// Main service class that manages multiple elections
class ClearBallotService {
public:
    ClearBallotService();
    ~ClearBallotService();
    
    // Service lifecycle
    bool startService();
    bool stopService();
    bool isRunning() const;
    
    // Election management
    int createElection(const std::string& title);
    bool electionExists(int electionId) const;
    std::shared_ptr<Election> getElection(int electionId);
    std::vector<int> getActiveElectionIds() const;
    
    // Voter operations (delegate to Election)
    ServiceResponse registerVoter(int electionId, const VoterRegistrationData& voterData);
    ServiceResponse castVote(int electionId, int voterId, int candidateIndex);
    ServiceResponse getCandidates(int electionId);
    ServiceResponse getVoters(int electionId);
    ServiceResponse getElectionResults(int electionId);
    ServiceResponse checkVoterRegistration(int electionId, int voterId);
    
    // Admin operations (delegate to Election)
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
    
private:
    std::map<int, std::shared_ptr<Election>> elections;
    mutable std::mutex electionsMutex;  // Thread safety for concurrent access
    int nextElectionId;
    bool serviceRunning;
    
    // Helper methods
    ServiceResponse createErrorResponse(const std::string& message) const;
    ServiceResponse createSuccessResponse(const std::string& message) const;
    ServiceResponse createSuccessResponse(const std::string& message, 
                                        const std::vector<std::string>& data) const;
    
    // Validation helpers
    bool validateElectionId(int electionId) const;
    std::shared_ptr<Election> getElectionSafe(int electionId);
};
