#include "auth_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>

AuthManager::AuthManager(University& uni) : university(uni) {
    loadAuth();
}

//this is a simple hashing function I've created to securly store passwords in a csv file
std::string AuthManager::hashPassword(const std::string& pw) {
    unsigned long hash = 5381;
    for (char c : pw) {
        hash = ((hash << 5) + hash) + c;
    }
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
}

bool AuthManager::idExists(const std::string& id) {
    for (const auto& record : authRecords) {
        if (record.studentID == id) {
            return true;
        }
    }
    return false;
}

void AuthManager::loadAuth() {
    authRecords.clear();
    std::ifstream file("data/students_auth.csv");
    if (!file.is_open()) {
        return; 
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string id, hash;
        
        if (std::getline(ss, id, ',') && std::getline(ss, hash, ',')) {
            AuthRecord record{id, hash};
            authRecords.push_back(record);
        }
    }
    file.close();
}

void AuthManager::saveAuth() {
    std::ofstream file("data/students_auth.csv", std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "[Error] Failed to write security credentials storage disk!\n";
        return;
    }

    for (const auto& record : authRecords) {
        file << record.studentID << "," << record.hashedPW << "\n";
    }
    file.close();
}

bool AuthManager::registerUser(const std::string& id, const std::string& password) {
    if (idExists(id)) {
        return false;
    }

    std::string protectedHash = hashPassword(password);
    authRecords.push_back(AuthRecord{id, protectedHash});
    
    saveAuth(); 
    return true;
}

bool AuthManager::changePassword(const std::string& id, const std::string& newPassword) {
    for (auto& record : authRecords) {
        if (record.studentID == id) {
            record.hashedPW = hashPassword(newPassword);
            saveAuth();
            return true;
        }
    }
    return false;
}

std::string AuthManager::doRegister() {
    
    std::string id, password, confirmPassword;
    std::cout << "\n--- New Student Registration ---\n";
    std::cout << "Enter custom or generated Student ID (12 digits : [Year of BAC] + your distinct 8 digit code): ";
    std::cin >> id;

    if (idExists(id)) {
        std::cout << "  [Error] Registration blocked: Student ID already registered!\n";
        return "";
    }

    std::cout << "Create Security Password: ";
    std::cin >> password;
    std::cout << "Confirm Password: ";
    std::cin >> confirmPassword;

    if (password != confirmPassword) {
        std::cout << "  [Error] Passwords mismatch! Registration canceled.\n";
        return "";
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string name, major;
    int age, year, wilaya;
    char gender;

    std::cout << "\n--- Create Student Profile ---\n";
    std::cout << "Enter Full Name: ";
    std::getline(std::cin, name);

    std::cout << "Enter Age: ";
    while (!(std::cin >> age) || age <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [Invalid] Enter a positive integer: ";
    }

    std::cout << "Enter Academic Year (1-5): ";
    while (!(std::cin >> year) || year < 1 || year > 5) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [Invalid] Enter a value between 1 and 5: ";
    }

    std::cout << "Enter Gender (M/F): ";
    while (!(std::cin >> gender) || (gender != 'M' && gender != 'F')) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [Invalid] Enter 'M' or 'F': ";
    }

    std::cout << "Enter Home Province (Wilaya) [1-69]: ";
    while (!(std::cin >> wilaya) || wilaya < 1 || wilaya > 69) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [Invalid] Enter a value between 1 and 69: ";
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter Academic Major: ";
    std::getline(std::cin, major);

    university.addNewStudent(id, name, age, gender, year, wilaya, major);

    if (registerUser(id, password)) {
        std::cout << "  [Success] Profile credentials generated! Welcome aboard.\n";
    } else {
        std::cout << "  [Error] Registration failed during data save.\n";
        return "";
    }

    return id;
}

std::string AuthManager::doSignIn() {
    std::string id, password;
    std::cout << "\n--- Secure Sign In Portal ---\n";
    std::cout << "Enter Student ID: ";
    std::cin >> id;
    std::cout << "Enter Password: ";
    std::cin >> password;

    std::string incomingHash = hashPassword(password);

    for (const auto& record : authRecords) {
        if (record.studentID == id && record.hashedPW == incomingHash) {
            std::cout << "  [Access Granted] Authorization token matched!\n";
            return id;
        }
    }

    std::cout << "  [Access Denied] Invalid ID or Password sequence match.\n";
    return "";
}

std::string AuthManager::runAuthLoop() {
    while (true) {
        std::cout << "======================================\n";
        std::cout << "       UNIVERSITY STUDENT PORTAL      \n";
        std::cout << "======================================\n";
        std::cout << "  1. Register New Account\n"
                  << "  2. Sign In to Existing Profile\n"
                  << "  0. Exit System\n"
                  << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  Invalid type input. Try again.\n\n";
            continue;
        }

        if (choice == 0) {
            return "";
        }
        else if (choice == 1) {
            return doRegister();
        } 
        else if (choice == 2) {
            std::string loggedInID = doSignIn();
            if (!loggedInID.empty()) {
                return loggedInID;
            }
        } 
        else {
            std::cout << "  Invalid option index selection. Choose [0-2].\n\n";
        }
    }
}