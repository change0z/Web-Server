#include <iostream>
#include "Election.h"

int main() {
    Election election("Presidential Election");

    // Add candidates
    election.addCandidate("Alice");
    election.addCandidate("Bob");
    election.addCandidate("Charlie");

    // Register voters
    election.registerVoter("John Doe", 1);
    election.registerVoter("Jane Smith", 2);

    // Cast votes
    election.castVote(1, 0); // John votes for Alice
    election.castVote(2, 1); // Jane votes for Bob
    election.castVote(1, 0); // Another vote for Alice

    // Display and save results
    election.displayResults();
    election.saveResultsToFile("results.txt");

    return 0;
}
