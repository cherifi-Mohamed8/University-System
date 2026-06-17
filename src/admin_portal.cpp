#include "admin_portal.h"
#include "auth_manager.h"
#include "room.h"
#include "csv_manager.h"
#include "restaurant.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <sstream>

AdminPortal::AdminPortal(University& uni) : university(uni) {}

void AdminPortal::run() {
    while (true) {
        std::cout << "\n==================================================\n"
                  << "          UNIVERSITY ADMINISTRATION PORTAL         \n"
                  << "==================================================\n"
                  << "  1.  Add New Student Profile\n"
                  << "  2.  Remove Student Profile\n"
                  << "  3.  Display All Students\n"
                  << "  4.  Create New Dormitory\n"
                  << "  41. Remove Dormitory\n"
                  << "  5.  Display Housing & Room Specs\n"
                  << "  6.  Assign Student to Room\n"
                  << "  7.  Evict Student from Room\n"
                  << "  8.  Edit Dormitory Restaurant Menu\n"
                  << "  81. Display Dormitory Restaurant Menu\n"
                  << "  9.  View Room Allocation Requests\n"
                  << "  0.  Save & Exit\n"
                  << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [Error] Invalid input. Enter a numeric choice.\n";
            continue;
        }

        if (choice == 0) { std::cout << "  [System] Exiting admin terminal...\n"; break; }

        switch (choice) {
            case 1:  handleAddStudent(); break;
            case 2:  handleRemoveStudent(); break;
            case 3:  university.displayAllStudents(); break;
            case 4:  handleAddDorm(); break;
            case 41: handleRemoveDorm(); break;
            case 5:  handleDisplayDetailedDorms(); break;
            case 6:  handleAssignRoom(); break;
            case 7:  handleEvictStudent(); break;
            case 8:  handleEditRestaurantMenu(); break;
            case 81: handleDisplayingMenu(); break;
            case 9:  handleRoomRequests(); break;
            default: std::cout << "  [Error] Unrecognized option. Choose [0-9].\n";
        }
    }
}

void AdminPortal::handleAddStudent() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string name, major, studentID;
    int age, year, wilaya;
    char gender;

    std::cout << "\n--- Create Student Profile ---\n";
    std::cout << "Enter Full Name: ";
    std::getline(std::cin, name);

   std::cout << "Enter Student ID (12-digit): ";
    while (true) {
        if (!(std::cin >> studentID) || !Student::isValidID(studentID)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [Invalid] Enter a valid Student ID: ";
            continue;
        }

        if (university.findStudentByID(studentID) != nullptr) {
            std::cout << "  [Error] Cannot add profile! A student with ID " << studentID << " already exists.\n";
            std::cout << "Enter Student ID (12-digit): ";
            continue;
        }
        
        break;
    }

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

    university.addNewStudent(studentID, name, age, gender, year, wilaya, major);
    AuthManager registerer(university);
    registerer.registerUser(studentID, studentID);
}

void AdminPortal::handleRemoveStudent() {
    std::string id;
    std::cout << "\nEnter Student ID to remove: ";
    if (!(std::cin >> id)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    university.removeStudent(id);
}

void AdminPortal::handleAddDorm() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string dormName;
    std::cout << "\nEnter Dormitory Name: ";
    std::getline(std::cin, dormName);
    if (dormName.empty()) return;

    if (university.findDormByName(dormName)) {
        std::cout << "  [Error] Dormitory '" << dormName << "' already exists.\n";
        return;
    }

    university.addDorm(dormName);

    int blockCount, floors, roomsPerFloor, capacity;
    std::cout << "How many blocks?: ";
    if (!(std::cin >> blockCount) || blockCount <= 0) return;
    std::cout << "Floors per block?: ";
    if (!(std::cin >> floors) || floors <= 0) return;
    std::cout << "Rooms per floor?: ";
    if (!(std::cin >> roomsPerFloor) || roomsPerFloor <= 0) return;
    std::cout << "Capacity per room?: ";
    if (!(std::cin >> capacity) || capacity <= 0) return;

    Dorm* dorm = university.findDormByName(dormName);
    std::ostringstream oss;
    for (int b = 1; b <= blockCount; ++b)
        for (int f = 1; f <= floors; ++f)
            for (int r = 1; r <= roomsPerFloor; ++r) {
                oss.str(""); oss.clear();
                oss << static_cast<char>(64 + b) << f << "-" << std::setfill('0') << std::setw(2) << r;
                dorm->addRoom(Room(oss.str(), capacity));
            }

    std::cout << "  [Success] Dormitory '" << dormName << "' created with "
              << blockCount * floors * roomsPerFloor << " rooms.\n";
}

void AdminPortal::handleRemoveDorm() {
    std::cout << "\n--- Remove Dormitory ---\n";
    for (const auto& d : university.getDormitories())
        std::cout << "  - " << d.getName() << "\n";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string dormName;
    std::cout << "Enter Dormitory Name to remove: ";
    std::getline(std::cin, dormName);
    if (dormName.empty()) return;

    university.removeDorm(dormName);
}

void AdminPortal::handleDisplayDetailedDorms() {
    const auto& dorms = university.getDormitories();
    if (dorms.empty()) { std::cout << "\n[Notice] No dormitories configured.\n"; return; }

    std::cout << "\n================== DETAILED HOUSING SPECS ==================\n";
    for (auto& dorm : university.getDormitories()) {
        std::cout << "\nDormitory: " << dorm.getName()
                  << " | Rooms: " << dorm.getRooms().size()
                  << " | Capacity: " << dorm.getCapacity() << " students\n"
                  << "  Empty: "     << dorm.getEmptyRooms().size()
                  << " | Full: "     << dorm.getFullRooms().size()
                  << " | Available: "<< dorm.getAvailableRooms().size() << "\n";
    }

    std::cout << "Enter dormitory name (or 'ALL'): ";
    std::string choice;
    std::cin >> choice;
    std::cout << "Filter rooms — Empty (E), Full (F), Available (A): ";
    char filter;
    std::cin >> filter;

    if (filter != 'E' && filter != 'F' && filter != 'A') {
        std::cout << "  [Error] Invalid filter.\n";
        return;
    }

    auto printRooms = [&](Dorm& dorm) {
        std::vector<Room*> list = (filter == 'E') ? dorm.getEmptyRooms()
                                : (filter == 'F') ? dorm.getFullRooms()
                                                  : dorm.getAvailableRooms();
        for (const auto* room : list)
            std::cout << "  - " << room->getName()
                      << " (Occupancy: " << room->getResidents().size() << "/" << room->getCapacity() << ")\n";
    };

    std::cout << "=============================================================\n";
    if (choice == "ALL") {
        for (auto& dorm : university.getDormitories()) {
            std::cout << "\n" << dorm.getName() << ":\n";
            printRooms(dorm);
        }
    } else {
        Dorm* dorm = university.findDormByName(choice);
        if (!dorm) { std::cout << "  [Error] Dormitory not found.\n"; return; }
        printRooms(*dorm);
    }
    std::cout << "=============================================================\n";
}

void AdminPortal::handleAssignRoom() {
    std::string id;
    std::cout << "\nEnter Student ID to assign: ";
    if (!(std::cin >> id)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    Student* student = university.findStudentByID(id);
    if (!student) { std::cout << "  [Error] Student ID " << id << " not found.\n"; return; }

    if (!student->getAssignedRoom().empty()) {
        std::cout << "  [Error] Student is already assigned to "
                  << student->getAssignedDorm() << " / " << student->getAssignedRoom() << ".\n";
        return;
    }

    auto& dorms = university.getDormitories();
    if (dorms.empty()) { std::cout << "  [Error] No dormitories configured.\n"; return; }

    std::cout << "\nSelect Dormitory:\n";
    for (size_t i = 0; i < dorms.size(); ++i)
        std::cout << "  " << i + 1 << ". " << dorms[i].getName() << "\n";
    std::cout << "Choice: ";

    size_t dChoice;
    if (!(std::cin >> dChoice) || dChoice < 1 || dChoice > dorms.size()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [Error] Invalid dormitory selection.\n";
        return;
    }

    Dorm& targetDorm = dorms[dChoice - 1];
    std::vector<Room*> vacantRooms = targetDorm.getAvailableRooms();

    if (vacantRooms.empty()) {
        std::cout << "  [Error] " << targetDorm.getName() << " has no vacancy.\n";
        return;
    }

    std::cout << "\nSelect Room in " << targetDorm.getName() << ":\n";
    for (size_t i = 0; i < vacantRooms.size(); ++i)
        std::cout << "  " << i + 1 << ". " << vacantRooms[i]->getName()
                  << " (Slots left: " << vacantRooms[i]->getCapacity() - vacantRooms[i]->getResidents().size() << ")\n";
    std::cout << "Choice: ";

    size_t rChoice;
    if (!(std::cin >> rChoice) || rChoice < 1 || rChoice > vacantRooms.size()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [Error] Invalid room selection.\n";
        return;
    }

    Room* targetRoom = vacantRooms[rChoice - 1];
    targetRoom->assignStudent(student);
    student->setAssignedDorm(targetDorm.getName());

    for(const auto& req: university.getRoomRequests()){
        if(req.studentId == student->getStudentID()){ 
            university.editRequest(req);
            break;
        }
    }

    std::cout << "  [Success] " << student->getName() << " assigned to "
              << targetDorm.getName() << " / " << targetRoom->getName() << ".\n";
}

void AdminPortal::handleEvictStudent() {
    std::string id;
    std::cout << "\nEnter Student ID to evict: ";
    if (!(std::cin >> id)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    Student* student = university.findStudentByID(id);
    if (!student) { std::cout << "  [Error] Student not found.\n"; return; }

    if (student->getAssignedRoom().empty()) {
        std::cout << "  [Notice] " << student->getName() << " has no room assignment.\n";
        return;
    }

    for (auto& dorm : university.getDormitories()) {
        Room* room = dorm.findRoom(student->getAssignedRoom());
        if (room) {
            room->removeStudent(student);
            std::cout << "  [Success] " << student->getName() << " evicted successfully.\n";
            return;
        }
    }

    std::cout << "  [Error] Room record inconsistency — student's room not found.\n";
}

void AdminPortal::handleEditRestaurantMenu() {
    auto& dorms = university.getDormitories();
    if (dorms.empty()) { std::cout << "  [Error] No dormitories configured.\n"; return; }

    std::cout << "\nSelect Dormitory Restaurant:\n";
    for (size_t i = 0; i < dorms.size(); ++i)
        std::cout << "  " << i + 1 << ". " << dorms[i].getName() << "\n";
    std::cout << "Choice: ";

    size_t choice;
    if (!(std::cin >> choice) || choice < 1 || choice > dorms.size()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [Error] Invalid selection.\n";
        return;
    }

    Restaurant* rest = dorms[choice - 1].getRestaurant();
    if (!rest) {
        std::cout << "  [Error] This dormitory does not have an active restaurant initialized.\n";
        return;
    }

    rest->editMenu();

    if (CSVManager::saveAllData(university)) {
        std::cout << "  [Success] Restaurant changes saved successfully.\n";
    } else {
        std::cout << "  [Warning] Modifications made in RAM failed to save to storage files.\n";
    }
}

void AdminPortal::handleDisplayingMenu() {
    const auto& dorms = university.getDormitories();
    if (dorms.empty()) { std::cout << "  [Notice] No dormitories configured to display menus.\n"; return; }

    std::cout << "\nSelect Dormitory to View Menu:\n";
    for (size_t i = 0; i < dorms.size(); ++i)
        std::cout << "  " << i + 1 << ". " << dorms[i].getName() << "\n";
    std::cout << "Choice: ";

    size_t choice;
    if (!(std::cin >> choice) || choice < 1 || choice > dorms.size()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [Error] Invalid choice.\n";
        return;
    }

    const Dorm& targetDorm = dorms[choice - 1];
    Restaurant* rest = targetDorm.getRestaurant();
    if (!rest) {
        std::cout << "  [Error] No restaurant linked to " << targetDorm.getName() << ".\n";
        return;
    }
    
    std::vector<std::string> mealNames = { "BREAKFAST", "LUNCH", "DINNER" };

    std::cout << "\n==================================================\n"
              << "       MENU CONFIGURATION: " << targetDorm.getName() << "\n"
              << "==================================================\n";

    for (int mealType = 0; mealType < 3; ++mealType) {
        std::cout << " [" << mealNames[mealType] << "]:\n";
        
        // Safely extract items from the 2D vector setup using your utility
        auto items = rest->getMenuItems(mealType);
        if (items.empty()) {
            std::cout << "    (No items configured for this meal tier yet)\n";
        } else {
            for (const auto& item : items) {
                std::cout << "    - " << item << "\n";
            }
        }
        std::cout << "--------------------------------------------------\n";
    }
}

void AdminPortal::handleRoomRequests() {
    const auto& requests = university.getRoomRequests();
    if (requests.empty()) { std::cout << "\n[Notice] No pending room requests.\n"; return; }

    std::cout << "\n================== ROOM ALLOCATION REQUESTS ==================\n";
    for (size_t i = 0; i < requests.size(); ++i) {
        const auto& req = requests[i];
        std::cout << "  " << i + 1 << ". Student ID: " << req.studentId
                  << " | Dorm: "   << req.dormName
                  << " | Status: " << req.status
                  << " | Date: "   << req.date << "\n";
    }
    std::cout << "==============================================================\n";
}