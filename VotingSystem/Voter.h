#pragma once
#include <string>

class Voter {
public:
    Voter(const std::string& firstName, const std::string& lastName, 
          const std::string& phoneNumber, const std::string& address, 
          int uniqueId, int age);

    // Getters
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getFullName() const;
    std::string getPhoneNumber() const;
    std::string getAddress() const;
    int getUniqueId() const;
    int getAge() const;
    bool hasVoted() const;

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
};