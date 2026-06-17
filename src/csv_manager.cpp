#include "csv_manager.h"
#include "university.h"
#include "room.h"
#include "restaurant.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

const std::string CSVManager::STUDENTS_FILE = "data/students_profiles.csv";
const std::string CSVManager::DORMS_FILE    = "data/dorms.csv";
const std::string CSVManager::ROOMS_FILE    = "data/rooms.csv";
const std::string CSVManager::MENUS_FILE    = "data/menus.csv";
const std::string CSVManager::REQUESTS_FILE = "data/room_requests.csv";

std::string CSVManager::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

std::vector<std::string> CSVManager::splitLine(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(line);
    while (std::getline(stream, token, delimiter))
        tokens.push_back(trim(token));
    return tokens;
}

bool CSVManager::loadAllData(University& university) {
    std::cout << "[System] Restoring environment configurations...\n";
    if (!loadStudents(university))     return false;
    if (!loadDormsAndRooms(university)) return false;
    if (!loadMenus(university))        return false;
    if (!loadRequests(university))     return false;
    std::cout << "[System] Database synced successfully!\n\n";
    return true;
}

bool CSVManager::saveAllData(const University& university) {
    std::cout << "\n[System] Committing safe-state to storage disk...\n";
    if (!saveStudents(university))     return false;
    if (!saveDormsAndRooms(university)) return false;
    if (!saveMenus(university))        return false;
    if (!saveRequests(university))     return false;
    std::cout << "[System] Shutdown state locked safely.\n";
    return true;
}

bool CSVManager::loadStudents(University& university) {
    std::ifstream file(STUDENTS_FILE);
    if (!file.is_open()) {
        std::cerr << "[Warning] " << STUDENTS_FILE << " missing. Starting blank.\n";
        return true;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> row = splitLine(line);
        if (row.size() < 7) continue;

        try {
            std::string id     = row[0];
            std::string name   = row[1];
            int age            = std::stoi(row[2]);
            char gender        = row[3].empty() ? 'U' : row[3][0];
            int year           = std::stoi(row[4]);
            int wilaya         = std::stoi(row[5]);
            std::string major  = row[6];

            Student* s = Student::createStudent(id, name, age, gender, year, wilaya, major);
            if (s) {
                university.addStudent(*s);
                delete s;
            } else {
                std::cerr << "[Warning] Skipping student with invalid ID: " << id << "\n";
            }
        } catch (...) {
            std::cerr << "[Error] Corrupted row in students CSV. Skipping.\n";
        }
    }
    return true;
}

bool CSVManager::loadDormsAndRooms(University& university) {
    std::ifstream dormFile(DORMS_FILE);
    if (dormFile.is_open()) {
        std::string line;
        while (std::getline(dormFile, line)) {
            if (line.empty()) continue;
            university.addDorm(trim(line));
        }
    }

    std::ifstream roomFile(ROOMS_FILE);
    if (!roomFile.is_open()) return true;

    std::string line;
    while (std::getline(roomFile, line)) {
        if (line.empty()) continue;
        std::vector<std::string> row = splitLine(line);
        if (row.size() < 3) continue;

        const std::string& dormName = row[0];
        const std::string& roomName = row[1];
        int capacity = std::stoi(row[2]);

        Dorm* dorm = university.findDormByName(dormName);
        if (!dorm) continue;

        dorm->addRoom(Room(roomName, capacity));

        Room* room = dorm->findRoom(roomName);
        if (!room) continue;

        for (size_t i = 3; i < row.size(); ++i) {
            if (row[i].empty()) continue;
            Student* student = university.findStudentByID(row[i]);
            if (student) {
                room->assignStudent(student);
                student->setAssignedDorm(dormName);
            }
        }
    }
    return true;
}

bool CSVManager::loadMenus(University& university) {
    std::ifstream file(MENUS_FILE);
    if (!file.is_open()) return true;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> row = splitLine(line);
        if (row.size() < 3) continue;

        Dorm* dorm = university.findDormByName(row[0]);
        if (dorm) {
            Restaurant* rest = dorm->getRestaurant();
            if (rest) {
                int mealType = std::stoi(row[1]);
                if (mealType >= 0 && mealType < 3) {
                    rest->addMenuItem(mealType, row[2]);
                }
            }
        }
    }
    return true;
}

bool CSVManager::loadRequests(University& university) {
    std::ifstream file(REQUESTS_FILE);
    if (!file.is_open()) return true;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> row = splitLine(line);
        if (row.size() < 4) continue;

        RoomRequest req;
        req.studentId = row[0];
        req.dormName  = row[1];
        req.status    = row[2];
        req.date      = row[3];
        university.addRoomRequest(req);
    }
    return true;
}

bool CSVManager::saveStudents(const University& university) {
    std::ofstream file(STUDENTS_FILE, std::ios::trunc);
    if (!file.is_open()) return false;

    for (const auto& s : university.getStudents()) {
        file << s.getStudentID() << ","
             << s.getName()      << ","
             << s.getAge()       << ","
             << s.getGender()    << ","
             << s.getAcademicYear() << ","
             << s.getWilayaNumber() << ","
             << s.getMajor()     << "\n";
    }
    return true;
}

bool CSVManager::saveDormsAndRooms(const University& university) {
    std::ofstream dFile(DORMS_FILE, std::ios::trunc);
    std::ofstream rFile(ROOMS_FILE, std::ios::trunc);
    if (!dFile.is_open() || !rFile.is_open()) return false;

    for (const auto& dorm : university.getDormitories()) {
        dFile << dorm.getName() << "\n";

        for (const auto& room : dorm.getRooms()) {
            rFile << dorm.getName() << "," << room.getName() << "," << room.getCapacity();
            for (const auto* resident : room.getResidents())
                rFile << "," << resident->getStudentID();
            rFile << "\n";
        }
    }
    return true;
}

bool CSVManager::saveMenus(const University& university) {
    std::ofstream file(MENUS_FILE, std::ios::trunc);
    if (!file.is_open()) return false;

    for (const auto& dorm : university.getDormitories()) {
        Restaurant* rest = dorm.getRestaurant();
        if (!rest) continue;
        for (int mealType = 0; mealType < 3; ++mealType) {
            for (const auto& item : rest->getMenuItems(mealType))
                file << dorm.getName() << "," << mealType << "," << item << "\n";
        }
    }
    return true;
}

bool CSVManager::saveRequests(const University& university) {
    std::ofstream file(REQUESTS_FILE, std::ios::trunc);
    if (!file.is_open()) return false;

    for (const auto& req : university.getRoomRequests()) {
        file << req.studentId << ","
             << req.dormName  << ","
             << req.status    << ","
             << req.date      << "\n";
    }
    return true;
}