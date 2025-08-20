#include <iostream>
#include "Election.h"

int main() {
    Election election("Presidential Election");

    // Create parties
    auto democraticParty = election.createParty("Democratic Party");
    auto republicanParty = election.createParty("Republican Party");

    // Add candidates with parties
    election.addCandidate("Alice", democraticParty);
    election.addCandidate("Bob", republicanParty);
    
    // Add an independent candidate (no party)
    election.addCandidate("Charlie");

    // Register voters
    election.registerVoter("John Doe", 1);
    election.registerVoter("Jane Smith", 2);

    // Cast votes
    election.castVote(1, 0); // John votes for Alice (Democratic)
    election.castVote(2, 1); // Jane votes for Bob (Republican)
    election.castVote(1, 2); // Another vote for Charlie (Independent)

    // Display and save results
    election.displayResults();
    election.saveResultsToFile("results.txt");

    return 0;
}
