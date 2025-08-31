# ClearBallot Quick Start Guide

*Get started with ClearBallot in 5 minutes*

## 🚀 **Choose Your Approach**

### **Option 1: Service-Oriented (Recommended for New Projects)**
```cpp
#include "ServiceClient.h"

int main() {
    ServiceClient client;
    client.connectToService();
    
    int electionId = client.createElection("My Election 2024");
    client.createParty(electionId, "Demo Party");
    client.addCandidate(electionId, "John Smith");
    
    client.registerVoter(electionId, "Jane", "Doe", "1234567890", 
                        "123 Main St", "123456789", "25");
    client.castVote(electionId, 123456789, 0);
    
    auto results = client.getElectionResults(electionId);
    std::cout << results.message << std::endl;
    
    client.disconnect();
    return 0;
}
```

### **Option 2: Legacy Direct (Simple Integration)**
```cpp
#include "Election.h"

int main() {
    Election election("My Election 2024");
    
    auto party = election.createParty("Demo Party");
    election.addCandidate("John Smith", party);
    
    election.registerVoter("Jane", "Doe", "1234567890", 
                          "123 Main St", "123456789", "25");
    election.castVote(123456789, 0);
    
    election.displayResults();
    return 0;
}
```

## 🎯 **When to Use Which?**

**Use Service-Oriented if you need:**
- Multiple elections
- Web interface (future)
- Thread safety
- Enterprise deployment

**Use Legacy Direct if you want:**
- Simple integration
- Educational use
- Rapid prototyping
- Minimal overhead

## 📖 **Next Steps**

- **[Architecture Guide](ARCHITECTURE_GUIDE.md)**: Complete documentation
- **[Examples](../examples/)**: More code examples
- **[API Reference](API_REFERENCE.md)**: Method details

---

*Both approaches are fully supported and maintained. Choose based on your project needs.*
