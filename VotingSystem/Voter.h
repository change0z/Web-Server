#pragma once
#include <string>

class Voter {
    public:
        Voter(const std::string& name, int id);
        std::string getName() const;
        int getID() const;

    private:
        std::string name;
        int id;
};