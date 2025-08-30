#pragma once
#include <string>
#include <memory>

// Forward declaration
class Region;

class Voter {
public:
    Voter(const std::string& firstName, const std::string& lastName, 
          const std::string& phoneNumber, const std::string& address, 
          int uniqueId, int age);
    
    // Constructor with region assignment
    Voter(const std::string& firstName, const std::string& lastName, 
          const std::string& phoneNumber, const std::string& address, 
          int uniqueId, int age, std::shared_ptr<Region> region);

    // Getters
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getFullName() const;
    std::string getPhoneNumber() const;
    std::string getAddress() const;
    int getUniqueId() const;
    int getAge() const;
    bool hasVoted() const;
    
    // Region-related methods
    std::shared_ptr<Region> getRegion() const;
    void setRegion(std::shared_ptr<Region> region);
    bool canVoteInRegion(std::shared_ptr<Region> region) const;

    // Voting functionality
    void markAsVoted();
    bool isEligible() const;

private:
    std::string firstName;
    std::string lastName;
    std::string phoneNumber;
    std::string address;
    int uniqueId;
    int age;
    bool voted;
    std::shared_ptr<Region> assignedRegion;
};