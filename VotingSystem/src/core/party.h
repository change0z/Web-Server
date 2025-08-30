#pragma once
#include <string>
#include <vector>

class Party {
    public:
        Party(const std::string& name);

        std::string getName() const;
        void addMember(const std::string& memberName);
        const std::vector<std::string>& getMembers() const;

    private:
        std::string name;
        std::vector<std::string> members;
};