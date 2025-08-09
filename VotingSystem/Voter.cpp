#include "Voter.h"

Voter::Voter(const std::string& name, int id) : name(name), id(id) {}

std::string Voter::getName() const {
    return name;
}

int Voter::getID() const {
    return id;
}