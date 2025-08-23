#include "ClearBallotService.h"
#include <iostream>
#include <sstream>

ClearBallotService::ClearBallotService() 
    : nextElectionId(1), serviceRunning(false) {
}

ClearBallotService::~ClearBallotService() {
    if (serviceRunning) {
        stopService();
    }
}

// Service lifecycle
bool ClearBallotService::startService() {
    std::lock_guard<std::mutex> lock(electionsMutex);
    
    if (serviceRunning) {
        std::cout << "[SERVICE] ClearBallot service is already running.\n";
        return true;
    }
    
    serviceRunning = true;
    std::cout << "[SERVICE] ClearBallot service started successfully.\n";
    return true;
}

bool ClearBallotService::stopService() {
    std::lock_guard<std::mutex> lock(electionsMutex);
    
    if (!serviceRunning) {
        std::cout << "[SERVICE] ClearBallot service is not running.\n";
        return true;
    }
    
    // Save all active elections before shutdown
    for (const auto& pair : elections) {
        auto election = pair.second;
        std::string filename = "election_" + std::to_string(pair.first) + "_shutdown.txt";
        election->saveCompleteElectionData(filename);
        std::cout << "[SERVICE] Election " << pair.first << " data saved to " << filename << "\n";
    }
    
    serviceRunning = false;
    std::cout << "[SERVICE] ClearBallot service stopped successfully.\n";
    return true;
}

bool ClearBallotService::isRunning() const {
    std::lock_guard<std::mutex> lock(electionsMutex);
    return serviceRunning;
}

// Election management
int ClearBallotService::createElection(const std::string& title) {
    std::lock_guard<std::mutex> lock(electionsMutex);
    
    int electionId = nextElectionId++;
    elections[electionId] = std::make_shared<Election>(title);
    
    std::cout << "[SERVICE] Created election '" << title << "' with ID: " << electionId << "\n";
    return electionId;
}

bool ClearBallotService::electionExists(int electionId) const {
    std::lock_guard<std::mutex> lock(electionsMutex);
    return elections.find(electionId) != elections.end();
}

std::shared_ptr<Election> ClearBallotService::getElection(int electionId) {
    std::lock_guard<std::mutex> lock(electionsMutex);
    
    auto it = elections.find(electionId);
    if (it != elections.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<Election> ClearBallotService::getElectionSafe(int electionId) {
    if (!validateElectionId(electionId)) {
        return nullptr;
    }
    return getElection(electionId);
}

std::vector<int> ClearBallotService::getActiveElectionIds() const {
    std::lock_guard<std::mutex> lock(electionsMutex);
    
    std::vector<int> ids;
    for (const auto& pair : elections) {
        ids.push_back(pair.first);
    }
    return ids;
}

// Voter operations (delegate to Election)
ServiceResponse ClearBallotService::registerVoter(int electionId, const VoterRegistrationData& voterData) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    // Delegate to existing Election method with validation
    bool success = election->registerVoter(voterData.firstName, voterData.lastName,
                                         voterData.phoneNumber, voterData.address,
                                         voterData.uniqueIdStr, voterData.ageStr);
    
    if (success) {
        return createSuccessResponse("Voter registered successfully.");
    } else {
        return createErrorResponse("Failed to register voter. Check validation requirements.");
    }
}

ServiceResponse ClearBallotService::castVote(int electionId, int voterId, int candidateIndex) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    // Delegate to existing Election method
    bool success = election->castVote(voterId, candidateIndex);
    
    if (success) {
        return createSuccessResponse("Vote cast successfully.");
    } else {
        return createErrorResponse("Failed to cast vote. Check voter ID and candidate selection.");
    }
}

ServiceResponse ClearBallotService::getCandidates(int electionId) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    // For now, return a simple success message
    // In a full implementation, we'd extract candidate data from Election
    return createSuccessResponse("Candidates retrieved.", {"Use displayCandidates() for full list"});
}

ServiceResponse ClearBallotService::getVoters(int electionId) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    return createSuccessResponse("Voters retrieved.", {"Use displayVoters() for full list"});
}

ServiceResponse ClearBallotService::getElectionResults(int electionId) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    return createSuccessResponse("Election results retrieved.", {"Use displayResults() for full results"});
}

ServiceResponse ClearBallotService::checkVoterRegistration(int electionId, int voterId) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    bool isRegistered = election->isVoterRegistered(voterId);
    bool hasVoted = election->hasVoterVoted(voterId);
    
    std::vector<std::string> status;
    status.push_back(isRegistered ? "Registered: Yes" : "Registered: No");
    status.push_back(hasVoted ? "Has Voted: Yes" : "Has Voted: No");
    
    return createSuccessResponse("Voter status retrieved.", status);
}

// Admin operations (delegate to Election)
ServiceResponse ClearBallotService::addCandidate(int electionId, const std::string& name) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    // Delegate to existing Election method
    election->addCandidate(name);
    return createSuccessResponse("Candidate '" + name + "' added successfully.");
}

ServiceResponse ClearBallotService::addCandidateWithParty(int electionId, const std::string& name, 
                                                        const std::string& partyName) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    // First, try to find existing party or create new one
    auto party = election->createParty(partyName);  // This will return existing if already exists
    
    // Add candidate with party
    election->addCandidate(name, party);
    
    return createSuccessResponse("Candidate '" + name + "' added to party '" + partyName + "'.");
}

ServiceResponse ClearBallotService::createParty(int electionId, const std::string& partyName) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    // Delegate to existing Election method
    auto party = election->createParty(partyName);
    
    if (party) {
        return createSuccessResponse("Party '" + partyName + "' created successfully.");
    } else {
        return createErrorResponse("Failed to create party '" + partyName + "'.");
    }
}

ServiceResponse ClearBallotService::getParties(int electionId) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    return createSuccessResponse("Parties retrieved.", {"Use displayParties() for full list"});
}

// Data persistence operations
ServiceResponse ClearBallotService::saveElectionResults(int electionId, const std::string& filename) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    // Delegate to existing Election method
    election->saveResultsToFile(filename);
    return createSuccessResponse("Election results saved to '" + filename + "'.");
}

ServiceResponse ClearBallotService::saveCompleteElectionData(int electionId, const std::string& filename) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    // Delegate to existing Election method
    election->saveCompleteElectionData(filename);
    return createSuccessResponse("Complete election data saved to '" + filename + "'.");
}

ServiceResponse ClearBallotService::loadElectionData(int electionId, const std::string& filename) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    // Delegate to existing Election method
    bool success = election->loadCompleteElectionData(filename);
    
    if (success) {
        return createSuccessResponse("Election data loaded from '" + filename + "'.");
    } else {
        return createErrorResponse("Failed to load election data from '" + filename + "'.");
    }
}

ServiceResponse ClearBallotService::exportElectionToCSV(int electionId, const std::string& baseFilename) {
    auto election = getElectionSafe(electionId);
    if (!election) {
        return createErrorResponse("Election not found.");
    }
    
    // Delegate to existing Election method
    election->exportToCSV(baseFilename);
    return createSuccessResponse("Election data exported to CSV files with base name '" + baseFilename + "'.");
}

// Helper methods
ServiceResponse ClearBallotService::createErrorResponse(const std::string& message) const {
    return ServiceResponse(false, message);
}

ServiceResponse ClearBallotService::createSuccessResponse(const std::string& message) const {
    return ServiceResponse(true, message);
}

ServiceResponse ClearBallotService::createSuccessResponse(const std::string& message, 
                                                        const std::vector<std::string>& data) const {
    ServiceResponse response(true, message);
    response.data = data;
    return response;
}

bool ClearBallotService::validateElectionId(int electionId) const {
    std::lock_guard<std::mutex> lock(electionsMutex);
    return elections.find(electionId) != elections.end();
}
