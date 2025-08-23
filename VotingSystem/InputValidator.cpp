#include "InputValidator.h"
#include <regex>
#include <sstream>
#include <algorithm>
#include <cctype>

bool InputValidator::isValidName(const std::string& name) {
    std::string trimmedName = trim(name);
    return !trimmedName.empty() && 
           isValidLength(trimmedName, ValidationConfig::MIN_NAME_LENGTH, ValidationConfig::MAX_NAME_LENGTH) &&
           containsOnlyAlphabetsAndSpaces(trimmedName);
}

bool InputValidator::isValidPhoneNumber(const std::string& phone) {
    std::string trimmedPhone = trim(phone);
    return isExactLength(trimmedPhone, ValidationConfig::PHONE_NUMBER_LENGTH) &&
           containsOnlyDigits(trimmedPhone);
}

bool InputValidator::isValidUniqueId(const std::string& id) {
    std::string trimmedId = trim(id);
    return isExactLength(trimmedId, ValidationConfig::UNIQUE_ID_LENGTH) &&
           containsOnlyDigits(trimmedId);
}

bool InputValidator::isValidAge(int age) {
    return age >= ValidationConfig::MIN_AGE && age <= ValidationConfig::MAX_AGE;
}

bool InputValidator::isValidAge(const std::string& ageStr) {
    std::string trimmedAge = trim(ageStr);
    if (!containsOnlyDigits(trimmedAge) || trimmedAge.empty()) {
        return false;
    }
    
    try {
        int age = std::stoi(trimmedAge);
        return isValidAge(age);
    } catch (const std::exception&) {
        return false;
    }
}

bool InputValidator::isValidAddress(const std::string& address) {
    std::string trimmedAddress = trim(address);
    return isValidLength(trimmedAddress, ValidationConfig::MIN_ADDRESS_LENGTH, ValidationConfig::MAX_ADDRESS_LENGTH);
}

std::string InputValidator::validateVoterInput(const std::string& firstName, 
                                             const std::string& lastName,
                                             const std::string& phone,
                                             const std::string& address,
                                             const std::string& idStr,
                                             const std::string& ageStr) {
    std::ostringstream errors;
    
    // Validate each field and collect all errors
    std::string firstNameError = validateName(firstName, "First name");
    if (!firstNameError.empty()) {
        errors << firstNameError << "\n";
    }
    
    std::string lastNameError = validateName(lastName, "Last name");
    if (!lastNameError.empty()) {
        errors << lastNameError << "\n";
    }
    
    std::string phoneError = validatePhoneNumber(phone);
    if (!phoneError.empty()) {
        errors << phoneError << "\n";
    }
    
    std::string addressError = validateAddress(address);
    if (!addressError.empty()) {
        errors << addressError << "\n";
    }
    
    std::string idError = validateUniqueId(idStr);
    if (!idError.empty()) {
        errors << idError << "\n";
    }
    
    std::string ageError = validateAge(ageStr);
    if (!ageError.empty()) {
        errors << ageError << "\n";
    }
    
    return errors.str();
}

std::string InputValidator::validateName(const std::string& name, const std::string& fieldName) {
    std::string trimmedName = trim(name);
    
    if (trimmedName.empty()) {
        return "[ERROR] " + fieldName + " cannot be empty.";
    }
    
    if (trimmedName.length() < ValidationConfig::MIN_NAME_LENGTH) {
        return "[ERROR] " + fieldName + " must be at least " + 
               std::to_string(ValidationConfig::MIN_NAME_LENGTH) + " characters long.";
    }
    
    if (trimmedName.length() > ValidationConfig::MAX_NAME_LENGTH) {
        return "[ERROR] " + fieldName + " must not exceed " + 
               std::to_string(ValidationConfig::MAX_NAME_LENGTH) + " characters.";
    }
    
    if (!containsOnlyAlphabetsAndSpaces(trimmedName)) {
        return "[ERROR] " + fieldName + " must contain only alphabetic characters and spaces.";
    }
    
    return ""; // No errors
}

std::string InputValidator::validatePhoneNumber(const std::string& phone) {
    std::string trimmedPhone = trim(phone);
    
    if (trimmedPhone.empty()) {
        return "[ERROR] Phone number cannot be empty.";
    }
    
    if (trimmedPhone.length() != ValidationConfig::PHONE_NUMBER_LENGTH) {
        return "[ERROR] Phone number must be exactly " + 
               std::to_string(ValidationConfig::PHONE_NUMBER_LENGTH) + " digits long.";
    }
    
    if (!containsOnlyDigits(trimmedPhone)) {
        return "[ERROR] Phone number must contain only numeric digits.";
    }
    
    return ""; // No errors
}

std::string InputValidator::validateUniqueId(const std::string& id) {
    std::string trimmedId = trim(id);
    
    if (trimmedId.empty()) {
        return "[ERROR] Unique ID cannot be empty.";
    }
    
    if (trimmedId.length() != ValidationConfig::UNIQUE_ID_LENGTH) {
        return "[ERROR] Unique ID must be exactly " + 
               std::to_string(ValidationConfig::UNIQUE_ID_LENGTH) + " digits long.";
    }
    
    if (!containsOnlyDigits(trimmedId)) {
        return "[ERROR] Unique ID must contain only numeric digits.";
    }
    
    return ""; // No errors
}

std::string InputValidator::validateAddress(const std::string& address) {
    std::string trimmedAddress = trim(address);
    
    if (trimmedAddress.empty()) {
        return "[ERROR] Address cannot be empty.";
    }
    
    if (trimmedAddress.length() < ValidationConfig::MIN_ADDRESS_LENGTH) {
        return "[ERROR] Address must be at least " + 
               std::to_string(ValidationConfig::MIN_ADDRESS_LENGTH) + " characters long.";
    }
    
    if (trimmedAddress.length() > ValidationConfig::MAX_ADDRESS_LENGTH) {
        return "[ERROR] Address must not exceed " + 
               std::to_string(ValidationConfig::MAX_ADDRESS_LENGTH) + " characters.";
    }
    
    return ""; // No errors
}

std::string InputValidator::validateAge(const std::string& ageStr) {
    std::string trimmedAge = trim(ageStr);
    
    if (trimmedAge.empty()) {
        return "[ERROR] Age cannot be empty.";
    }
    
    if (!containsOnlyDigits(trimmedAge)) {
        return "[ERROR] Age must be a valid number (digits only).";
    }
    
    try {
        int age = std::stoi(trimmedAge);
        if (age < ValidationConfig::MIN_AGE) {
            return "[ERROR] You must be at least " + 
                   std::to_string(ValidationConfig::MIN_AGE) + " years old to register.";
        }
        if (age > ValidationConfig::MAX_AGE) {
            return "[ERROR] Age must not exceed " + 
                   std::to_string(ValidationConfig::MAX_AGE) + " years.";
        }
    } catch (const std::exception&) {
        return "[ERROR] Age contains invalid characters or is too large.";
    }
    
    return ""; // No errors
}

// Private helper methods
bool InputValidator::containsOnlyAlphabetsAndSpaces(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isalpha(c) || std::isspace(c);
    });
}

bool InputValidator::containsOnlyDigits(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isdigit(c);
    });
}

bool InputValidator::isExactLength(const std::string& str, size_t length) {
    return str.length() == length;
}

bool InputValidator::isValidLength(const std::string& str, size_t min, size_t max) {
    return str.length() >= min && str.length() <= max;
}

std::string InputValidator::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}
