#include "ClearBallotService.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

// Global service instance and control variables
std::unique_ptr<ClearBallotService> g_service;
std::atomic<bool> g_serviceRunning{false};

// Signal handler for graceful shutdown
void signalHandler(int signum) {
    std::cout << "\n[SERVICE] Received shutdown signal (" << signum << ")\n";
    std::cout << "[SERVICE] Initiating graceful shutdown...\n";
    
    if (g_service && g_serviceRunning.load()) {
        g_service->stopService();
        g_serviceRunning.store(false);
    }
    
    std::cout << "[SERVICE] ClearBallot service stopped.\n";
    exit(0);
}

void displayServiceInfo() {
    std::cout << "========================================\n";
    std::cout << "    ClearBallot Election Service\n";
    std::cout << "========================================\n";
    std::cout << "Version: 1.0.0 (Service Architecture)\n";
    std::cout << "Build: " << __DATE__ << " " << __TIME__ << "\n";
    std::cout << "Platform: ";
#ifdef _WIN32
    std::cout << "Windows Service\n";
#else
    std::cout << "Linux Daemon\n";
#endif
    std::cout << "========================================\n\n";
}

void displayServiceCommands() {
    std::cout << "[SERVICE] Available commands:\n";
    std::cout << "  status    - Show service status\n";
    std::cout << "  elections - List all active elections\n";
    std::cout << "  stop      - Stop the service\n";
    std::cout << "  help      - Show this help\n";
    std::cout << "  quit/exit - Stop the service\n\n";
}

void displayServiceStatus() {
    if (!g_service) {
        std::cout << "[STATUS] Service not initialized\n";
        return;
    }
    
    std::cout << "[STATUS] ClearBallot Service Status:\n";
    std::cout << "  Running: " << (g_service->isRunning() ? "YES" : "NO") << "\n";
    
    auto electionIds = g_service->getActiveElectionIds();
    std::cout << "  Active Elections: " << electionIds.size() << "\n";
    
    if (!electionIds.empty()) {
        std::cout << "  Election IDs: ";
        for (size_t i = 0; i < electionIds.size(); ++i) {
            std::cout << electionIds[i];
            if (i < electionIds.size() - 1) std::cout << ", ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void displayActiveElections() {
    if (!g_service) {
        std::cout << "[ERROR] Service not initialized\n";
        return;
    }
    
    auto electionIds = g_service->getActiveElectionIds();
    
    if (electionIds.empty()) {
        std::cout << "[INFO] No active elections\n";
        return;
    }
    
    std::cout << "[INFO] Active Elections:\n";
    for (int electionId : electionIds) {
        auto election = g_service->getElection(electionId);
        if (election) {
            std::cout << "  ID " << electionId << ": " << election->getTitle() << "\n";
            std::cout << "    Status: Active\n";
        }
    }
    std::cout << "\n";
}

void processServiceCommand(const std::string& command) {
    if (command == "status") {
        displayServiceStatus();
    }
    else if (command == "elections") {
        displayActiveElections();
    }
    else if (command == "stop" || command == "quit" || command == "exit") {
        std::cout << "[SERVICE] Stopping service...\n";
        g_serviceRunning.store(false);
    }
    else if (command == "help") {
        displayServiceCommands();
    }
    else if (!command.empty()) {
        std::cout << "[ERROR] Unknown command: " << command << "\n";
        std::cout << "Type 'help' for available commands.\n\n";
    }
}

void serviceConsoleLoop() {
    std::string command;
    
    std::cout << "[SERVICE] Console mode active. Type 'help' for commands.\n";
    std::cout << "[SERVICE] Use Ctrl+C or 'stop' to shutdown.\n\n";
    
    while (g_serviceRunning.load()) {
        std::cout << "ClearBallot> ";
        std::getline(std::cin, command);
        
        // Trim whitespace
        size_t start = command.find_first_not_of(" \t");
        if (start == std::string::npos) {
            continue; // Empty command
        }
        size_t end = command.find_last_not_of(" \t");
        command = command.substr(start, end - start + 1);
        
        processServiceCommand(command);
    }
}

int runServiceInConsoleMode() {
    std::cout << "[SERVICE] Starting ClearBallot service in console mode...\n";
    
    // Initialize service
    g_service = std::make_unique<ClearBallotService>();
    
    if (!g_service->startService()) {
        std::cout << "[ERROR] Failed to start ClearBallot service\n";
        return 1;
    }
    
    g_serviceRunning.store(true);
    
    // Set up signal handlers for graceful shutdown
    signal(SIGINT, signalHandler);   // Ctrl+C
    signal(SIGTERM, signalHandler);  // Termination request
#ifdef _WIN32
    signal(SIGBREAK, signalHandler); // Ctrl+Break on Windows
#endif
    
    std::cout << "[SERVICE] ClearBallot service started successfully!\n";
    std::cout << "[SERVICE] Service is ready to accept connections.\n\n";
    
    // Create a demo election for testing
    int demoElectionId = g_service->createElection("Demo Election 2024");
    std::cout << "[SERVICE] Created demo election (ID: " << demoElectionId << ") for testing\n\n";
    
    // Run interactive console
    serviceConsoleLoop();
    
    // Cleanup
    std::cout << "[SERVICE] Shutting down...\n";
    if (g_service) {
        g_service->stopService();
        g_service.reset();
    }
    
    std::cout << "[SERVICE] Shutdown complete.\n";
    return 0;
}

#ifdef _WIN32
// Windows Service functions (placeholder for future implementation)
int installWindowsService() {
    std::cout << "[INFO] Windows Service installation not yet implemented.\n";
    std::cout << "[INFO] Use console mode for now: ClearBallotService.exe\n";
    return 1;
}

int uninstallWindowsService() {
    std::cout << "[INFO] Windows Service uninstallation not yet implemented.\n";
    return 1;
}

int runAsWindowsService() {
    std::cout << "[INFO] Windows Service mode not yet implemented.\n";
    std::cout << "[INFO] Use console mode for now: ClearBallotService.exe\n";
    return 1;
}
#endif

void displayUsage(const char* programName) {
    std::cout << "ClearBallot Election Service\n\n";
    std::cout << "Usage: " << programName << " [option]\n\n";
    std::cout << "Options:\n";
    std::cout << "  (no args)    Run in console mode (default)\n";
    std::cout << "  --console    Run in console mode\n";
    std::cout << "  --help       Show this help message\n";
    std::cout << "  --version    Show version information\n";
#ifdef _WIN32
    std::cout << "  --install    Install as Windows Service (future)\n";
    std::cout << "  --uninstall  Uninstall Windows Service (future)\n";
    std::cout << "  --service    Run as Windows Service (future)\n";
#endif
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "  " << programName << "              # Run in console mode\n";
    std::cout << "  " << programName << " --console    # Run in console mode\n";
#ifdef _WIN32
    std::cout << "  " << programName << " --install    # Install Windows Service\n";
#endif
    std::cout << "\n";
}

int main(int argc, char* argv[]) {
    displayServiceInfo();
    
    // Parse command line arguments
    if (argc > 1) {
        std::string arg = argv[1];
        
        if (arg == "--help" || arg == "-h") {
            displayUsage(argv[0]);
            return 0;
        }
        else if (arg == "--version" || arg == "-v") {
            // Version info already displayed by displayServiceInfo()
            return 0;
        }
        else if (arg == "--console") {
            return runServiceInConsoleMode();
        }
#ifdef _WIN32
        else if (arg == "--install") {
            return installWindowsService();
        }
        else if (arg == "--uninstall") {
            return uninstallWindowsService();
        }
        else if (arg == "--service") {
            return runAsWindowsService();
        }
#endif
        else {
            std::cout << "[ERROR] Unknown option: " << arg << "\n\n";
            displayUsage(argv[0]);
            return 1;
        }
    }
    
    // Default: run in console mode
    return runServiceInConsoleMode();
}
