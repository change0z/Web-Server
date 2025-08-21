# Voting System Demo Instructions

## Overview
The voting system is now split into two separate applications:

1. **ElectionAdmin.exe** - For election officials to set up elections
2. **VoterInterface.exe** - For voters to register and cast votes

## Demo Workflow

### Step 1: Election Setup (Admin)
Run `ElectionAdmin.exe` to set up the election:

```
.\Debug\ElectionAdmin.exe
```

**Admin tasks:**
1. Create parties (Democratic, Republican, Green)
2. Add candidates to parties
3. Add independent candidates
4. Review candidate list
5. Save election configuration

### Step 2: Voter Experience
Run `VoterInterface.exe` for the voter experience:

```
.\Debug\VoterInterface.exe
```

**Voter tasks:**
1. Register to vote (with full validation)
2. View available candidates
3. Cast vote securely
4. Check registration status
5. View election results

## Key Features

### Election Admin System:
- ✅ Create political parties
- ✅ Add party-affiliated candidates
- ✅ Add independent candidates
- ✅ Monitor voter registration
- ✅ View real-time results
- ✅ Save election data

### Voter Interface:
- ✅ Complete voter registration with validation
- ✅ Age verification (18+ only)
- ✅ Unique ID enforcement
- ✅ Secure vote casting
- ✅ One vote per person limit
- ✅ Registration status checking

## Security Features:
- Age validation (must be 18+)
- Unique voter ID enforcement
- Double voting prevention
- Complete audit trail
- Secure vote recording

## Demo Scenario:

### Admin Setup:
1. Create "Democratic Party", "Republican Party", "Green Party"
2. Add "Alice Johnson" to Democratic Party
3. Add "Bob Smith" to Republican Party  
4. Add "Carol Green" to Green Party
5. Add "David Wilson" as Independent
6. Save configuration

### Voter Demo:
1. Register voter: "John Doe", ID: 12345, Age: 25
2. Register voter: "Jane Smith", ID: 67890, Age: 30
3. Try to register underage voter (should fail)
4. Try to register duplicate ID (should fail)
5. Cast votes for different candidates
6. Try to vote twice (should fail)
7. View final results

This demonstrates a complete, secure voting system with proper separation of concerns!
