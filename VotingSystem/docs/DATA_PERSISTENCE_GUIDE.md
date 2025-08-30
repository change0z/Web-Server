# Data Persistence Demo Script

## What Gets Saved in the Enhanced Voting System

### 1. Basic Results File (`saveResultsToFile()`)
**Contains:**
- Election title
- Total votes cast  
- Total registered voters
- Candidate names with vote counts and party affiliation

**Example:**
```
Election Results: 2024 Local Elections
================================

Total votes cast: 3
Total registered voters: 3

Alice Johnson (Democratic Party): 1 votes
Bob Smith (Republican Party): 1 votes  
Carol Green (Green Party): 1 votes
David Independent: 0 votes
```

### 2. Complete Election Data (`saveCompleteElectionData()`)
**Contains EVERYTHING:**
- **Election metadata** (title, counts)
- **All parties** (names and members)
- **All candidates** (names, votes, party affiliation)
- **All voters** (full personal details, voting status)
- **Voting summary** (turnout percentage, statistics)

**Example:**
```
COMPLETE_ELECTION_DATA
======================

[ELECTION_INFO]
Title=2024 Local Elections
TotalCandidates=4
TotalVoters=3
TotalParties=3

[PARTIES]
Party0=Democratic Party
Party0_Members=Alice Johnson
Party1=Republican Party
Party1_Members=Bob Smith
Party2=Green Party
Party2_Members=Carol Green

[CANDIDATES]
Candidate0_Name=Alice Johnson
Candidate0_Votes=1
Candidate0_Party=Democratic Party
Candidate1_Name=Bob Smith
Candidate1_Votes=1
Candidate1_Party=Republican Party
Candidate2_Name=Carol Green
Candidate2_Votes=1
Candidate2_Party=Green Party
Candidate3_Name=David Independent
Candidate3_Votes=0
Candidate3_Party=Independent

[VOTERS]
Voter0_FirstName=John
Voter0_LastName=Doe
Voter0_Phone=555-1234
Voter0_Address=123 Main St
Voter0_UniqueId=12345
Voter0_Age=25
Voter0_HasVoted=1
Voter1_FirstName=Jane
Voter1_LastName=Smith
Voter1_Phone=555-5678
Voter1_Address=456 Oak Ave
Voter1_UniqueId=67890
Voter1_Age=30
Voter1_HasVoted=1
Voter2_FirstName=Bob
Voter2_LastName=Wilson
Voter2_Phone=555-9999
Voter2_Address=789 Pine St
Voter2_UniqueId=11111
Voter2_Age=45
Voter2_HasVoted=1

[VOTING_SUMMARY]
TotalVotesCast=3
VoterTurnout=100.00%
```

### 3. CSV Export (`exportToCSV()`)
**Creates 3 separate CSV files:**

#### `election_candidates.csv`
```csv
Index,Name,Party,Votes,VotePercentage
0,Alice Johnson,Democratic Party,1,33.33
1,Bob Smith,Republican Party,1,33.33
2,Carol Green,Green Party,1,33.33
3,David Independent,Independent,0,0.00
```

#### `election_voters.csv`
```csv
UniqueId,FirstName,LastName,Age,Phone,Address,HasVoted
12345,John,Doe,25,555-1234,"123 Main St",Yes
67890,Jane,Smith,30,555-5678,"456 Oak Ave",Yes
11111,Bob,Wilson,45,555-9999,"789 Pine St",Yes
```

#### `election_parties.csv`
```csv
PartyName,MemberCount,Members
Democratic Party,1,"Alice Johnson"
Republican Party,1,"Bob Smith"
Green Party,1,"Carol Green"
```

## Automatic Saving Triggers

### Admin Tool (`ElectionAdmin.exe`)
- **On Exit**: Saves complete election data + CSV export
- **Manual Save**: Options 8, 9, 10 in menu
- **Files Created:**
  - `admin_session_complete.txt` (complete data)
  - `admin_session_export_candidates.csv`
  - `admin_session_export_voters.csv` 
  - `admin_session_export_parties.csv`

### Voter Interface (`VoterInterface.exe`)
- **On Exit**: Saves complete election data
- **Files Created:**
  - `voter_session_complete.txt` (complete data)

## Data Recovery & Audit Trail

All saved files contain:
✅ **Complete voter registry** (names, addresses, ages, IDs)
✅ **Full voting records** (who voted, who didn't)
✅ **Party memberships** (complete candidate affiliations)
✅ **Vote counts** (per candidate with percentages)
✅ **Election statistics** (turnout, participation rates)
✅ **Timestamp information** (when data was saved)

This provides **complete election integrity** and **full audit capability**!

## File Formats Supported
- **TXT**: Human-readable complete election data
- **CSV**: Spreadsheet-compatible for analysis
- **Structured Data**: Easy to parse for election verification

Your voting system now maintains **complete election records** that could be used for:
- Official election certification
- Audit and recount procedures  
- Statistical analysis
- Compliance reporting
- Historical election records
