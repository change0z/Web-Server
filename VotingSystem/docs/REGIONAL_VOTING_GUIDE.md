# Regional Voting System Implementation

## Overview
The voting system has been enhanced with comprehensive regional voting capabilities that enforce strict geographical restrictions while maintaining party flexibility across regions.

## Key Features

### 1. Regional Architecture
- **Regions**: Geographical districts that organize elections locally
- **Regional Isolation**: Each region operates independently for voting purposes
- **Cross-Regional Party Support**: Parties can operate across multiple regions

### 2. Implemented Restrictions

#### Restriction 1: Candidate Regional Exclusivity
- **Rule**: Same candidate cannot be enlisted in multiple regions
- **Enforcement**: Global candidate registry prevents duplicate registrations
- **Example**: If "John Smith" is registered in North District, he cannot be registered in South District

#### Restriction 2: One Candidate Per Party Per Region
- **Rule**: Each party can have only one candidate per region
- **Enforcement**: Regional party-candidate validation before registration
- **Example**: Democratic Party can have one candidate in North, one in South, etc., but not multiple in the same region

#### Restriction 3: Regional Voter Assignment
- **Rule**: Voters can only vote in their assigned region
- **Enforcement**: Region-based voter registration and voting validation
- **Example**: Voters registered in North District cannot vote for candidates in South District

#### Valid Scenarios
- **Multiple parties per region**: One region can have candidates from different parties
- **Same party across regions**: A party can have different candidates in different regions
- **Independent candidates**: Can be registered in any region (subject to uniqueness rule)

## New Classes and Components

### Region Class (`Region.h`, `Region.cpp`)
```cpp
class Region {
    // Manages candidates, voters, and parties within a geographical region
    // Enforces regional restrictions
    // Provides regional-specific display and validation methods
};
```

**Key Methods:**
- `addCandidate()`: Adds candidate with party validation
- `addVoter()`: Registers voter in the region
- `hasPartyCandidate()`: Checks if party already has a candidate
- `canVoterVoteInRegion()`: Validates voter eligibility

### Enhanced Voter Class
**New Features:**
- Region assignment (`assignedRegion`)
- Region-based voting validation
- Constructor with region parameter

### Enhanced Candidate Class
**New Features:**
- Region assignment (`assignedRegion`)
- Region-based registration
- Constructor with region parameter

### Enhanced Election Class
**New Regional Methods:**
- `createRegion()`: Creates new geographical regions
- `addCandidateToRegion()`: Adds candidates to specific regions
- `registerVoterInRegion()`: Registers voters in specific regions
- `castVoteInRegion()`: Handles region-specific voting
- `displayResultsInRegion()`: Shows results for specific regions

## Usage Examples

### Setting Up Regional Elections
```cpp
Election election("2024 Regional Elections");

// Create regions
auto northRegion = election.createRegion("North District", "NORTH");
auto southRegion = election.createRegion("South District", "SOUTH");

// Create parties
auto democraticParty = election.createParty("Democratic Party");
auto republicanParty = election.createParty("Republican Party");

// Add candidates to regions
election.addCandidateToRegion("Alice Johnson", democraticParty, northRegion);
election.addCandidateToRegion("Bob Smith", democraticParty, southRegion);
election.addCandidateToRegion("Carol Wilson", republicanParty, northRegion);
```

### Voter Registration and Voting
```cpp
// Register voters in specific regions
election.registerVoterInRegion("John", "Doe", "1234567890", "123 Main St", 
                               "123456789", "25", northRegion);

// Voting (voters can only vote in their assigned region)
election.castVoteInRegion(123456789, 0, northRegion); // Success
election.castVoteInRegion(123456789, 0, southRegion); // Fails - wrong region
```

## Validation and Error Handling

### Automatic Validation
1. **Duplicate Candidate Prevention**: Global registry prevents same candidate in multiple regions
2. **Party Candidate Limits**: Prevents multiple candidates from same party in same region
3. **Regional Voting Enforcement**: Ensures voters only vote in assigned regions
4. **Input Validation**: Comprehensive validation for all voter/candidate data

### Error Messages
The system provides clear, descriptive error messages:
- "Candidate 'X' is already registered in another region"
- "Party 'Y' already has a candidate in region 'Z'"
- "Voter with ID X is not registered to vote in region 'Y'"

## Demo Applications

### 1. Interactive Regional Demo (`RegionalDemo.exe`)
- Full interactive interface for regional voting system
- Step-by-step setup and testing
- Real-time validation demonstration

### 2. Automated Restrictions Test (`TestRestrictions.exe`)
- Automated testing of all regional restrictions
- Demonstrates valid and invalid scenarios
- Comprehensive validation of business rules

## Integration with Existing System

### Backward Compatibility
- All existing functionality remains unchanged
- Legacy methods still available for non-regional operations
- Existing data structures maintained

### Service Architecture Compatibility
The regional voting system integrates seamlessly with the existing ClearBallot service architecture:
- Service layer can manage multiple regional elections
- Client interfaces can specify regions for operations
- Thread safety maintained for concurrent regional operations

## File Structure
```
VotingSystem/
├── Region.h                 # Regional management header
├── Region.cpp               # Regional management implementation
├── regional_demo.cpp        # Interactive demo application
├── test_restrictions.cpp    # Automated testing
├── Election.h/.cpp          # Enhanced with regional support
├── Candidate.h/.cpp         # Enhanced with regional assignment
├── Voter.h/.cpp             # Enhanced with regional assignment
└── CMakeLists.txt           # Updated build configuration
```

## Build and Run

### Build Commands
```bash
cd VotingSystem
cmake --build . --config Debug
```

### Executables
- `RegionalDemo.exe`: Interactive regional voting demo
- `TestRestrictions.exe`: Automated restrictions testing
- `ElectionAdmin.exe`: Election administration (enhanced with regional support)
- `VoterInterface.exe`: Voter interface (enhanced with regional support)

## Key Benefits

1. **Geographical Organization**: Elections organized by regions/districts
2. **Strict Compliance**: Enforces real-world voting restrictions
3. **Flexible Party Management**: Parties can operate across regions
4. **Scalable Architecture**: Supports any number of regions
5. **Clear Validation**: Comprehensive error handling and user feedback
6. **Backward Compatible**: Existing functionality preserved

## Future Enhancements

1. **Multi-Level Regions**: Support for nested regions (state -> county -> district)
2. **Regional Results Aggregation**: Combined reporting across regions
3. **Regional Admin Roles**: Different administrative permissions by region
4. **Regional Data Persistence**: Save/load regional configurations
5. **Cross-Regional Analytics**: Comparative analysis between regions

This implementation provides a robust, scalable foundation for regional voting systems while maintaining the integrity and restrictions you specified.
