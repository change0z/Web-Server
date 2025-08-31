#include "Voter.h"
#include "../regional/Region.h"

Voter::Voter(const std::string& firstName, const std::string& lastName, 
             const std::string& phoneNumber, const std::string& address, 
             int uniqueId, int age)
    : firstName(firstName), lastName(lastName), phoneNumber(phoneNumber), 
      address(address), uniqueId(uniqueId), age(age), voted(false), assignedRegion(nullptr) {
}

Voter::Voter(const std::string& firstName, const std::string& lastName, 
             const std::string& phoneNumber, const std::string& address, 
             int uniqueId, int age, std::shared_ptr<Region> region)
    : firstName(firstName), lastName(lastName), phoneNumber(phoneNumber), 
      address(address), uniqueId(uniqueId), age(age), voted(false), assignedRegion(region) {
}

std::string Voter::getFirstName() const {
    return firstName;
}

std::string Voter::getLastName() const {
    return lastName;
}

std::string Voter::getFullName() const {
    return firstName + " " + lastName;
}

std::string Voter::getPhoneNumber() const {
    return phoneNumber;
}

std::string Voter::getAddress() const {
    return address;
}

int Voter::getUniqueId() const {
    return uniqueId;
}

int Voter::getAge() const {
    return age;
}

bool Voter::hasVoted() const {
    return voted;
}

void Voter::markAsVoted() {
    voted = true;
}

bool Voter::isEligible() const {
    return age >= 18;
}

std::shared_ptr<Region> Voter::getRegion() const {
    return assignedRegion;
}

void Voter::setRegion(std::shared_ptr<Region> region) {
    assignedRegion = region;
}

bool Voter::canVoteInRegion(std::shared_ptr<Region> region) const {
    return assignedRegion && region && assignedRegion == region;
}