#include "ServiceClient.h"
#include <iostream>

ServiceClient::ServiceClient() 
    : connected(false), defaultElectionId(-1) {
}

ServiceClient::~ServiceClient() {
    disconnect();
}

// Connection management
bool ServiceClient::connectToService() {
    if (connected) {
        std::cout << "[CLIENT] Already connected to ClearBallot service.\n";
        return true;
    }
    
    try {
        // For initial implementation, create a local service instance
        // In future versions, this would connect to a remote service via IPC
        service = std::make_shared<ClearBallotService>();
        
        if (!service->startService()) {
            std::cout << "[CLIENT ERROR] Failed to start ClearBallot service.\n";
            return false;
        }
        
        connected = true;
        std::cout << "[CLIENT] Connected to ClearBallot service successfully.\n";
        
        // Ensure we have a default election for single-election console apps
        ensureDefaultElection();
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "[CLIENT ERROR] Failed to connect to service: " << e.what() << "\n";
        return false;
    }
}

bool ServiceClient::isConnected() const {
    return connected && service && service->isRunning();
}

void ServiceClient::disconnect() {
    if (service && connected) {
        service->stopService();
        std::cout << "[CLIENT] Disconnected from ClearBallot service.\n";
    }
    
    service.reset();
    connected = false;
    defaultElectionId = -1;
}

// Election management
int ServiceClient::createElection(const std::string& title) {
    if (!isConnected()) {
        std::cout << "[CLIENT ERROR] Not connected to service.\n";
        return -1;
    }
    
    return service->createElection(title);
}

bool ServiceClient::electionExists(int electionId) {
    if (!isConnected()) {
        return false;
    }
    
    return service->electionExists(electionId);
}

// Voter operations
ServiceResponse ServiceClient::registerVoter(int electionId, const std::string& firstName,
                                            const std::string& lastName, const std::string& phone,
                                            const std::string& address, const std::string& idStr,
                                            const std::string& ageStr) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    // Use default election if electionId is -1
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    
    VoterRegistrationData voterData(firstName, lastName, phone, address, idStr, ageStr);
    return service->registerVoter(targetElectionId, voterData);
}

ServiceResponse ServiceClient::castVote(int electionId, int voterId, int candidateIndex) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->castVote(targetElectionId, voterId, candidateIndex);
}

ServiceResponse ServiceClient::getCandidates(int electionId) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->getCandidates(targetElectionId);
}

ServiceResponse ServiceClient::getVoters(int electionId) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->getVoters(targetElectionId);
}

ServiceResponse ServiceClient::getElectionResults(int electionId) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->getElectionResults(targetElectionId);
}

ServiceResponse ServiceClient::checkVoterRegistration(int electionId, int voterId) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->checkVoterRegistration(targetElectionId, voterId);
}

// Admin operations
ServiceResponse ServiceClient::addCandidate(int electionId, const std::string& name) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->addCandidate(targetElectionId, name);
}

ServiceResponse ServiceClient::addCandidateWithParty(int electionId, const std::string& name,
                                                   const std::string& partyName) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->addCandidateWithParty(targetElectionId, name, partyName);
}

ServiceResponse ServiceClient::createParty(int electionId, const std::string& partyName) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->createParty(targetElectionId, partyName);
}

ServiceResponse ServiceClient::getParties(int electionId) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->getParties(targetElectionId);
}

// Data persistence operations
ServiceResponse ServiceClient::saveElectionResults(int electionId, const std::string& filename) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->saveElectionResults(targetElectionId, filename);
}

ServiceResponse ServiceClient::saveCompleteElectionData(int electionId, const std::string& filename) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->saveCompleteElectionData(targetElectionId, filename);
}

ServiceResponse ServiceClient::loadElectionData(int electionId, const std::string& filename) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->loadElectionData(targetElectionId, filename);
}

ServiceResponse ServiceClient::exportElectionToCSV(int electionId, const std::string& baseFilename) {
    if (!isConnected()) {
        return createErrorResponse("Not connected to service.");
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->exportElectionToCSV(targetElectionId, baseFilename);
}

// Direct Election access (for compatibility)
std::shared_ptr<Election> ServiceClient::getElection(int electionId) {
    if (!isConnected()) {
        return nullptr;
    }
    
    int targetElectionId = (electionId == -1) ? defaultElectionId : electionId;
    return service->getElection(targetElectionId);
}

// Helper methods
ServiceResponse ServiceClient::createErrorResponse(const std::string& message) const {
    return ServiceResponse(false, "[CLIENT ERROR] " + message);
}

bool ServiceClient::ensureDefaultElection() {
    if (defaultElectionId != -1) {
        return true;  // Already have a default election
    }
    
    if (!isConnected()) {
        return false;
    }
    
    // Create a default election for backward compatibility with existing console apps
    defaultElectionId = service->createElection("2024 Local Elections");
    
    if (defaultElectionId > 0) {
        std::cout << "[CLIENT] Created default election with ID: " << defaultElectionId << "\n";
        
        // Set up default parties and candidates for compatibility
        service->createParty(defaultElectionId, "Democratic Party");
        service->createParty(defaultElectionId, "Republican Party");
        service->createParty(defaultElectionId, "Green Party");
        
        service->addCandidateWithParty(defaultElectionId, "Alice Johnson", "Democratic Party");
        service->addCandidateWithParty(defaultElectionId, "Bob Smith", "Republican Party");
        service->addCandidateWithParty(defaultElectionId, "Carol Green", "Green Party");
        service->addCandidate(defaultElectionId, "David Independent");
        
        return true;
    }
    
    return false;
}
