#include "student_portal.h"
#include "auth_manager.h"
#include "university.h"
#include "student.h"
#include "dorm.h"
#include "room.h"
#include "restaurant.h"
#include <iostream>
#include <algorithm>
#include <limits>

StudentPortal::StudentPortal(University& uni, const std::string& loggedInID) 
    : university(uni), studentID(loggedInID), currentStudent(nullptr) {
    
    try {
        const std::string& numericID = studentID;
        currentStudent = university.findStudentByID(numericID);
    } catch (...) {
        currentStudent = nullptr;
    }
}

void StudentPortal::run() {
    if (!currentStudent) {
        std::cout << "  [Error] Failed to resolve active student profile context tracking token.\n";
        return;
    }

    std::cout << "\nWelcome back, " << currentStudent->getName() << "!\n";

    while (true) {
        std::cout << "\n==================================================\n";
        std::cout << "               STUDENT SERVICE PORTAL             \n";
        std::cout << "==================================================\n";
        std::cout << "  1. View My Personal Profile & Housing Status\n"
                  << "  2. Submit Request for Room Allocation\n"
                  << "  3. View Today's Dining Hall Menu\n"
                  << "  4. View My Pending Requests\n"
                  << "  5. Change My Account Password\n"
                  << "  0. Secure Log Out\n"
                  << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [Invalid] Please select a numerical index option [0-5].\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "  [System] Authorization session destroyed. Logging out securely...\n";
            break;
        }

        switch (choice) {
            case 1:
                handleViewProfile();
                break;
            case 2:
                handleRequestRoom();
                break;
            case 3:
                handleViewMenu();
                break;
            case 4:
                handleViewRequests();
                break;
            case 5:
                handleChangePassword();
                break;
            default:
                std::cout << "  [Invalid] Choice out of bounds selection range [0-5].\n";
        }
    }
}

void StudentPortal::handleViewProfile() {
    std::cout << "\n--- Your Student Profile ---\n";
    std::cout << "  - Name:           " << currentStudent->getName() << "\n"
              << "  - Student ID:     " << currentStudent->getStudentID() << "\n"
              << "  - Academic Year:  " << currentStudent->getAcademicYear() << "\n"
              << "  - Home Province:  " << currentStudent->getWilaya() << "\n"
              << "  - Selected Major: " << currentStudent->getMajor() << "\n";
    currentStudent->getAccomodationStatus();
}

void StudentPortal::handleRequestRoom() {
    std::cout << "\n--- Housing Allocation Request Engine ---\n";
    
    if (university.findDormByName(currentStudent->getAssignedRoom()) != nullptr) {
        std::cout << "  [Notice] You are already assigned to: " 
                  << currentStudent->getAssignedDorm() << ", " 
                  << currentStudent->getAssignedRoom() << ".\n"
                  << "  To switch locations, contact a campus administrator.\n";
        return;
    }
    
    auto& dorms = university.getDormitories();
    if (dorms.empty()) {
        std::cout << "  [System Error] No housing facilities are available at this time.\n";
        return;
    }

    std::cout << "Available Dormitories with vacancies:\n";
    std::vector<Dorm*> validChoices;
    
    for (auto& d : dorms) {
        if (!d.getAvailableRooms().empty()) {
            validChoices.push_back(&d);
            std::cout << "  " << validChoices.size() << ". " << d.getName() << "\n";
        }
    }

    if (validChoices.empty()) {
        std::cout << "  [Notice] All campus dormitory rooms are currently at 100% maximum capacity.\n"
                  << "  Your profile remains on the pending housing waitlist.\n";
        return;
    }

    int dormChoice;
    std::cout << "Select a Dormitory (1-" << validChoices.size() << "): ";
    std::cin >> dormChoice;

    if (std::cin.fail() || dormChoice < 1 || dormChoice > static_cast<int>(validChoices.size())) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "  [Error] Invalid choice selection.\n";
        return;
    }

    RoomRequest newRequest;
    newRequest.studentId = currentStudent->getStudentID();
    newRequest.dormName = validChoices[dormChoice - 1]->getName();

    auto it = std::find(university.getRoomRequests().begin(), university.getRoomRequests().end(), newRequest);

    if(it != university.getRoomRequests().end()) std::cout << "you already have a room request!\n";
    else university.addRoomRequest(newRequest);

}

void StudentPortal::handleViewMenu() {
    Dorm* myDorm = university.findDormByName(currentStudent->getAssignedDorm());
    
    if (!myDorm) {
        std::cout << "\n  [Notice] Dining hall menus are linked directly to assigned housing complexes.\n"
                  << "  Please secure an active room placement to view daily menus.\n";
        return;
    }

    Restaurant* rest = myDorm->getRestaurant();
    if (rest) {
        rest->displayMenu();
    } else {
        std::cout << "  [System Error] Food services catalog missing for " << myDorm->getName() << ".\n";
    }
}

void StudentPortal::handleViewRequests() {
    std::cout << "\n--- My Pending Room Allocation Requests ---\n";
    auto& requests = university.getRoomRequests();
    bool hasRequests = false;

    for (const auto& req : requests) {
        if (req.studentId == currentStudent->getStudentID()) {
            hasRequests = true;
            std::cout << "  • Desired Dorm: " << req.dormName 
                      << " | Status: " << req.status 
                      << " | Submitted On: " << req.date << "\n";
        }
        if (req.status == "APPROVED" && req.studentId == currentStudent->getStudentID()) {
            std::cout << "  [Congratulations] Your room allocation request has been approved!\n"
                      << "  Please contact the housing office for details on your new assignment.\n";
        }
    }

    if (!hasRequests) {
        std::cout << "  You have no pending room allocation requests at this time.\n";
    }
}

void StudentPortal::handleChangePassword() {
    std::cout << "\n--- Change Account Password ---\n";
    std::string newPassword, confirmPassword;
    std::cout << "Enter New Password: ";
    std::cin >> newPassword;
    std::cout << "Confirm New Password: ";
    std::cin >> confirmPassword;

    AuthManager authManager(university);

    if (newPassword != confirmPassword) {
        std::cout << "  [Error] Passwords do not match. Password change aborted.\n";
        return;
    } else if (newPassword.empty()) {
        std::cout << "  [Error] Password cannot be empty. Password change aborted.\n";
        return;
    }
    if (authManager.changePassword(currentStudent->getStudentID(), newPassword)) {
        std::cout << "  [Success] Password updated successfully.\n";
    } else {
        std::cout << "  [Error] Failed to update password. Please try again later.\n";
    }
}