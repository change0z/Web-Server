#include "InputValidator.h"
#include <iostream>
#include <vector>
#include <functional>

// Simple test framework
class ValidationTestRunner {
private:
    int testsRun = 0;
    int testsPassed = 0;
    
public:
    void runTest(const std::string& testName, std::function<bool()> testFunc) {
        testsRun++;
        std::cout << "Running test: " << testName << "... ";
        
        try {
            if (testFunc()) {
                testsPassed++;
                std::cout << "PASSED\n";
            } else {
                std::cout << "FAILED\n";
            }
        } catch (const std::exception& e) {
            std::cout << "FAILED (Exception: " << e.what() << ")\n";
        }
    }
    
    void printSummary() {
        std::cout << "\n=== Validation Test Summary ===\n";
        std::cout << "Tests run: " << testsRun << "\n";
        std::cout << "Tests passed: " << testsPassed << "\n";
        std::cout << "Tests failed: " << (testsRun - testsPassed) << "\n";
        std::cout << "Success rate: " << (testsRun > 0 ? (testsPassed * 100.0 / testsRun) : 0) << "%\n";
    }
    
    bool allTestsPassed() const {
        return testsRun > 0 && testsPassed == testsRun;
    }
};

// Name Validation Tests
class NameValidationTests {
public:
    static bool testValidNames() {
        // Generate a valid name with exactly 50 characters (all 'A')
        const std::string fiftyCharName(50, 'A');
        return InputValidator::isValidName("John") &&
               InputValidator::isValidName("Alice") &&
               InputValidator::isValidName("Bob") &&
               InputValidator::isValidName("MaryJane") &&
               InputValidator::isValidName("An") && // Minimum 2 chars
               InputValidator::isValidName(fiftyCharName);
    }
    
    static bool testInvalidNames() {
        return !InputValidator::isValidName("") &&          // Empty
               !InputValidator::isValidName("A") &&          // Too short
               !InputValidator::isValidName("John123") &&     // Contains numbers
               !InputValidator::isValidName("John@Smith") &&  // Contains symbols
               !InputValidator::isValidName("John Doe") &&    // Contains space
               !InputValidator::isValidName("VeryLongNameThatExceedsFiftyCharacterLimitTest"); // Too long
    }
    
    static bool testNameBoundaryConditions() {
        return InputValidator::isValidName("Ab") &&  // Exactly 2 chars (minimum)
               InputValidator::isValidName("ThisNameIsExactlyFiftyCharactersLongToTestMax") && // Exactly 50 chars
               !InputValidator::isValidName("A") &&  // One char too short
               !InputValidator::isValidName("ThisNameIsExactlyFiftyOneCharactersLongTestMax"); // One char too long
    }
    
    static bool testNameSpecialCases() {
        return !InputValidator::isValidName("123") &&        // Only numbers
               !InputValidator::isValidName("@#$") &&        // Only symbols
               !InputValidator::isValidName("   ") &&        // Only spaces
               !InputValidator::isValidName("John-Doe") &&   // Hyphen
               !InputValidator::isValidName("O'Connor");     // Apostrophe
    }
};

// Phone Validation Tests
class PhoneValidationTests {
public:
    static bool testValidPhones() {
        return InputValidator::isValidPhoneNumber("1234567890") &&
               InputValidator::isValidPhoneNumber("9876543210") &&
               InputValidator::isValidPhoneNumber("5555551234") &&
               InputValidator::isValidPhoneNumber("0000000000") &&
               InputValidator::isValidPhoneNumber("9999999999");
    }
    
    static bool testInvalidPhones() {
        return !InputValidator::isValidPhoneNumber("") &&           // Empty
               !InputValidator::isValidPhoneNumber("123456789") &&   // Too short
               !InputValidator::isValidPhoneNumber("12345678901") && // Too long
               !InputValidator::isValidPhoneNumber("123-456-7890") && // Contains hyphens
               !InputValidator::isValidPhoneNumber("(123) 456-7890") && // Contains formatting
               !InputValidator::isValidPhoneNumber("123.456.7890") && // Contains dots
               !InputValidator::isValidPhoneNumber("123 456 7890") && // Contains spaces
               !InputValidator::isValidPhoneNumber("abcdefghij");     // Contains letters
    }
    
    static bool testPhoneBoundaryConditions() {
        return InputValidator::isValidPhoneNumber("1234567890") &&   // Exactly 10 digits
               !InputValidator::isValidPhoneNumber("123456789") &&    // 9 digits
               !InputValidator::isValidPhoneNumber("12345678901");    // 11 digits
    }
    
    static bool testPhoneSpecialCases() {
        return !InputValidator::isValidPhoneNumber("12345abc90") &&  // Mixed letters/numbers
               !InputValidator::isValidPhoneNumber("+1234567890") &&  // International format
               !InputValidator::isValidPhoneNumber("1-234-567-890") && // Partial formatting
               !InputValidator::isValidPhoneNumber("          ");     // Only spaces
    }
};

// ID Validation Tests
class IdValidationTests {
public:
    static bool testValidIds() {
        return InputValidator::isValidUniqueId("123456789") &&
               InputValidator::isValidUniqueId("987654321") &&
               InputValidator::isValidUniqueId("555666777") &&
               InputValidator::isValidUniqueId("000000000") &&
               InputValidator::isValidUniqueId("999999999");
    }
    
    static bool testInvalidIds() {
        return !InputValidator::isValidUniqueId("") &&          // Empty
               !InputValidator::isValidUniqueId("12345678") &&   // Too short
               !InputValidator::isValidUniqueId("1234567890") && // Too long
               !InputValidator::isValidUniqueId("123-45-6789") && // Contains hyphens
               !InputValidator::isValidUniqueId("123 456 789") && // Contains spaces
               !InputValidator::isValidUniqueId("12345678a") &&  // Contains letter
               !InputValidator::isValidUniqueId("abcdefghi");    // All letters
    }
    
    static bool testIdBoundaryConditions() {
        return InputValidator::isValidUniqueId("123456789") &&    // Exactly 9 digits
               !InputValidator::isValidUniqueId("12345678") &&     // 8 digits
               !InputValidator::isValidUniqueId("1234567890");     // 10 digits
    }
    
    static bool testIdSpecialCases() {
        return !InputValidator::isValidUniqueId("12345-789") &&   // Partial formatting
               !InputValidator::isValidUniqueId("123.456.789") &&  // Dot formatting
               !InputValidator::isValidUniqueId("12345678@") &&    // Special character
               !InputValidator::isValidUniqueId("         ");      // Only spaces
    }
};

// Age Validation Tests
class AgeValidationTests {
public:
    static bool testValidAges() {
        return InputValidator::isValidAge("18") &&   // Minimum voting age
               InputValidator::isValidAge("25") &&
               InputValidator::isValidAge("45") &&
               InputValidator::isValidAge("65") &&
               InputValidator::isValidAge("99") &&
               InputValidator::isValidAge("100");   // Centenarian
    }
    
    static bool testInvalidAges() {
        return !InputValidator::isValidAge("") &&      // Empty
               !InputValidator::isValidAge("17") &&     // Below voting age
               !InputValidator::isValidAge("0") &&      // Zero
               !InputValidator::isValidAge("-5") &&     // Negative
               !InputValidator::isValidAge("25.5") &&   // Decimal
               !InputValidator::isValidAge("twenty") && // Word
               !InputValidator::isValidAge("25a") &&    // Mixed
               !InputValidator::isValidAge("a25");      // Mixed
    }
    
    static bool testAgeBoundaryConditions() {
        return InputValidator::isValidAge("18") &&      // Exactly minimum
               !InputValidator::isValidAge("17") &&     // One below minimum
               InputValidator::isValidAge("150") &&     // Very old but valid
               !InputValidator::isValidAge("999");      // Unrealistic
    }
    
    static bool testAgeSpecialCases() {
        return !InputValidator::isValidAge("018") &&    // Leading zero
               !InputValidator::isValidAge("18.0") &&   // Decimal zero
               !InputValidator::isValidAge("+18") &&    // Plus sign
               !InputValidator::isValidAge("18 ") &&    // Trailing space
               !InputValidator::isValidAge(" 18");      // Leading space
    }
};

// Address Validation Tests
class AddressValidationTests {
public:
    static bool testValidAddresses() {
        return InputValidator::isValidAddress("123 Main St") &&           // Standard
               InputValidator::isValidAddress("456 Oak Avenue") &&        // Avenue
               InputValidator::isValidAddress("789 Elm Road") &&          // Road
               InputValidator::isValidAddress("12345") &&                 // Just numbers (5+ chars)
               InputValidator::isValidAddress("Apt 5") &&                 // Apartment
               InputValidator::isValidAddress("Suite 100, Main Building"); // Complex
    }
    
    static bool testInvalidAddresses() {
        return !InputValidator::isValidAddress("") &&          // Empty
               !InputValidator::isValidAddress("123") &&        // Too short (3 chars)
               !InputValidator::isValidAddress("12") &&         // Too short (2 chars)
               !InputValidator::isValidAddress("1") &&          // Too short (1 char)
               !InputValidator::isValidAddress("    ");         // Only spaces
    }
    
    static bool testAddressBoundaryConditions() {
        return InputValidator::isValidAddress("12345") &&      // Exactly 5 chars (minimum)
               !InputValidator::isValidAddress("1234") &&       // One char too short
               InputValidator::isValidAddress("Very long address with many details and information");
    }
    
    static bool testAddressSpecialCases() {
        return InputValidator::isValidAddress("123 Main St.") &&     // Period
               InputValidator::isValidAddress("123 Main St, Apt 5") && // Comma
               InputValidator::isValidAddress("123-A Main Street") &&  // Hyphen
               InputValidator::isValidAddress("P.O. Box 123") &&       // P.O. Box
               InputValidator::isValidAddress("123 Main St #5");       // Hash
    }
};

// Comprehensive Validation Tests
class ComprehensiveValidationTests {
public:
    static bool testCompleteValidDataSet() {
        // Test a complete set of valid data
        return InputValidator::isValidName("John") &&
               InputValidator::isValidName("Doe") &&
               InputValidator::isValidPhoneNumber("1234567890") &&
               InputValidator::isValidUniqueId("123456789") &&
               InputValidator::isValidAge("25") &&
               InputValidator::isValidAddress("123 Main Street");
    }
    
    static bool testCompleteInvalidDataSet() {
        // Test a complete set of invalid data
        return !InputValidator::isValidName("") &&        // Invalid name
               !InputValidator::isValidPhoneNumber("123") &&     // Invalid phone
               !InputValidator::isValidUniqueId("12345") &&      // Invalid ID
               !InputValidator::isValidAge("17") &&        // Invalid age
               !InputValidator::isValidAddress("123");     // Invalid address
    }
    
    static bool testSecurityValidation() {
        // Test potential security issues
        return !InputValidator::isValidName("<script>") &&         // HTML/Script
               !InputValidator::isValidName("';DROP TABLE--") &&   // SQL injection attempt
               !InputValidator::isValidPhoneNumber("';SELECT*--") &&     // SQL in phone
               !InputValidator::isValidAddress("../../etc/passwd") && // Path traversal
               !InputValidator::isValidUniqueId("../../../");            // Path traversal in ID
    }
    
    static bool testUnicodeAndSpecialChars() {
        // Test unicode and special characters
        return !InputValidator::isValidName("Jöhn") &&       // Unicode
               !InputValidator::isValidName("José") &&       // Accent
               !InputValidator::isValidPhoneNumber("１２３４５６７８９０") && // Full-width numbers
               !InputValidator::isValidAddress("测试地址");    // Chinese characters
    }
};

int main() {
    std::cout << "=== ClearBallot Input Validation Tests ===\n\n";
    
    ValidationTestRunner runner;
    
    // Name Validation Tests
    std::cout << "--- Name Validation Tests ---\n";
    runner.runTest("Valid Names", NameValidationTests::testValidNames);
    runner.runTest("Invalid Names", NameValidationTests::testInvalidNames);
    runner.runTest("Name Boundary Conditions", NameValidationTests::testNameBoundaryConditions);
    runner.runTest("Name Special Cases", NameValidationTests::testNameSpecialCases);
    
    // Phone Validation Tests
    std::cout << "\n--- Phone Validation Tests ---\n";
    runner.runTest("Valid Phones", PhoneValidationTests::testValidPhones);
    runner.runTest("Invalid Phones", PhoneValidationTests::testInvalidPhones);
    runner.runTest("Phone Boundary Conditions", PhoneValidationTests::testPhoneBoundaryConditions);
    runner.runTest("Phone Special Cases", PhoneValidationTests::testPhoneSpecialCases);
    
    // ID Validation Tests
    std::cout << "\n--- ID Validation Tests ---\n";
    runner.runTest("Valid IDs", IdValidationTests::testValidIds);
    runner.runTest("Invalid IDs", IdValidationTests::testInvalidIds);
    runner.runTest("ID Boundary Conditions", IdValidationTests::testIdBoundaryConditions);
    runner.runTest("ID Special Cases", IdValidationTests::testIdSpecialCases);
    
    // Age Validation Tests
    std::cout << "\n--- Age Validation Tests ---\n";
    runner.runTest("Valid Ages", AgeValidationTests::testValidAges);
    runner.runTest("Invalid Ages", AgeValidationTests::testInvalidAges);
    runner.runTest("Age Boundary Conditions", AgeValidationTests::testAgeBoundaryConditions);
    runner.runTest("Age Special Cases", AgeValidationTests::testAgeSpecialCases);
    
    // Address Validation Tests
    std::cout << "\n--- Address Validation Tests ---\n";
    runner.runTest("Valid Addresses", AddressValidationTests::testValidAddresses);
    runner.runTest("Invalid Addresses", AddressValidationTests::testInvalidAddresses);
    runner.runTest("Address Boundary Conditions", AddressValidationTests::testAddressBoundaryConditions);
    runner.runTest("Address Special Cases", AddressValidationTests::testAddressSpecialCases);
    
    // Comprehensive Tests
    std::cout << "\n--- Comprehensive Validation Tests ---\n";
    runner.runTest("Complete Valid Data Set", ComprehensiveValidationTests::testCompleteValidDataSet);
    runner.runTest("Complete Invalid Data Set", ComprehensiveValidationTests::testCompleteInvalidDataSet);
    runner.runTest("Security Validation", ComprehensiveValidationTests::testSecurityValidation);
    runner.runTest("Unicode and Special Chars", ComprehensiveValidationTests::testUnicodeAndSpecialChars);
    
    runner.printSummary();
    
    return runner.allTestsPassed() ? 0 : 1;
}
