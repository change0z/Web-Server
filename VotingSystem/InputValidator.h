#pragma once
#include <string>
#include <regex>

class InputValidator {
public:
    struct ValidationConfig {
        static const size_t PHONE_NUMBER_LENGTH = 10;
        static const size_t UNIQUE_ID_LENGTH = 9;
        static const size_t MIN_NAME_LENGTH = 2;
        static const size_t MAX_NAME_LENGTH = 50;
        static const size_t MIN_ADDRESS_LENGTH = 5;
        static const size_t MAX_ADDRESS_LENGTH = 500;
        static const int MIN_AGE = 18;
        static const int MAX_AGE = 120;
    };

    // Name validation - alphabets and spaces only
    static bool isValidName(const std::string& name);
    
    // Phone validation - exactly 10 digits
    static bool isValidPhoneNumber(const std::string& phone);
    
    // ID validation - exactly 9 digits
    static bool isValidUniqueId(const std::string& id);
    
    // Age validation
    static bool isValidAge(int age);
    static bool isValidAge(const std::string& ageStr);
    
    // Address validation - longer character limit
    static bool isValidAddress(const std::string& address);
    
    // Comprehensive validation with detailed error messages
    static std::string validateVoterInput(const std::string& firstName, 
                                        const std::string& lastName,
                                        const std::string& phone,
                                        const std::string& address,
                                        const std::string& idStr,
                                        const std::string& ageStr);

    // Individual field validation with specific error messages
    static std::string validateName(const std::string& name, const std::string& fieldName);
    static std::string validatePhoneNumber(const std::string& phone);
    static std::string validateUniqueId(const std::string& id);
    static std::string validateAddress(const std::string& address);
    static std::string validateAge(const std::string& ageStr);

    // Utility function for trimming whitespace
    static std::string trim(const std::string& str);

private:
    static bool containsOnlyAlphabetsAndSpaces(const std::string& str);
    static bool containsOnlyDigits(const std::string& str);
    static bool isExactLength(const std::string& str, size_t length);
    static bool isValidLength(const std::string& str, size_t min, size_t max);
};
