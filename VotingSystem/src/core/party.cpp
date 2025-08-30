#include "party.h"

Party::Party(const std::string& name) : name(name) {}

std::string Party::getName() const {
    return name;
}

void Party::addMember(const std::string& memberName) {
    members.push_back(memberName);
}

const std::vector<std::string>& Party::getMembers() const {
    return members;
}
