#include "university.h"
#include "room.h"
#include <iostream>
#include <algorithm>

University::University() {
    dormitories.reserve(10);
    students.reserve(1000);
}

University::~University() {}

const std::vector<Student>& University::getStudents() const { return students; }
std::vector<Dorm>& University::getDormitories() { return dormitories; }
const std::vector<Dorm>& University::getDormitories() const { return dormitories; }
const std::vector<RoomRequest>& University::getRoomRequests() const { return roomRequests; }

Student* University::findStudentByID(const std::string& id) {
    for (auto& s : students)
        if (s.getStudentID() == id) return &s;
    return nullptr;
}

Dorm* University::findDormByName(const std::string& name) {
    for (auto& d : dormitories)
        if (d.getName() == name) return &d;
    return nullptr;
}

RoomRequest* University::findRequestByID(const std::string& studentID){
    for(auto& req : roomRequests){
        if(req.studentId == studentID) return &req;
    }
    return nullptr;
}

void University::addStudent(const Student& student) {
    students.push_back(student);
}

void University::removeStudent(const std::string& studentID) {
    Student* student = findStudentByID(studentID);
    if (!student) {
        std::cout << "  [Error] Student ID " << studentID << " not found.\n";
        return;
    }

    if (!student->getAssignedRoom().empty()) {
        for (auto& dorm : dormitories) {
            Room* room = dorm.findRoom(student->getAssignedRoom());
            if (room) { room->removeStudent(student); break; }
        }
    }

    students.erase(
        std::remove_if(students.begin(), students.end(),
            [&studentID](const Student& s) { return s.getStudentID() == studentID; }),
        students.end());

    std::cout << "  [Success] Student ID " << studentID << " removed from university roster.\n";
}

void University::addDorm(const std::string& dormName) {
    dormitories.emplace_back(dormName);
}

void University::removeDorm(const std::string& dormName) {
    auto it = std::remove_if(dormitories.begin(), dormitories.end(),
        [&dormName](const Dorm& d) { return d.getName() == dormName; });

    if (it != dormitories.end()) {
        dormitories.erase(it, dormitories.end());
        std::cout << "  [Success] Dormitory '" << dormName << "' removed.\n";
    } else {
        std::cout << "  [Error] Dormitory '" << dormName << "' not found.\n";
    }
}

void University::addNewStudent(const std::string& id, const std::string& name, int age,
                               char gender, int year, int wilaya, const std::string& major) {
    Student* s = Student::createStudent(id, name, age, gender, year, wilaya, major);
    if (s) {
        addStudent(*s);
        delete s;
        std::cout << "  [Success] Student profile created for " << name << " (ID: " << id << ").\n";
    } else {
        std::cout << "  [Error] Failed to create student profile.\n";
    }
}

void University::addRoomRequest(const RoomRequest& request) {
    roomRequests.push_back(request);

}

void University::displayAllStudents() const {
    if (students.empty()) { std::cout << "No registered students in the system.\n"; return; }

    std::cout << "\n--- Master Student Roster ---\n";
    for (const auto& s : students) {
        std::cout << "ID: " << s.getStudentID()
                  << " | Name: " << s.getName()
                  << " | Age: " << s.getAge()
                  << " | Gender: " << s.getGender()
                  << " | Year: " << s.getAcademicYear()
                  << " | Major: " << s.getMajor() << "\n"
                  << "  Home Province: " << s.getWilaya() << "\n";
        s.getAccomodationStatus();
        std::cout << "-------------------------------------------\n";
    }
}

void University::displayAllDormitories() const {
    if (dormitories.empty()) { std::cout << "No dormitory infrastructure set up yet.\n"; return; }

    std::cout << "\n--- Campus Housing Directory ---\n";
    for (const auto& d : dormitories) {
        std::cout << "Dormitory: " << d.getName()
                  << " | Rooms: " << d.getRooms().size()
                  << " | Capacity: " << d.getCapacity() << " students\n";
    }
}

void University::displayAllEmptyRooms() {
    std::cout << "\n--- Empty Rooms ---\n";
    for (auto& dorm : dormitories)
        for (auto* room : dorm.getEmptyRooms())
            std::cout << "Dorm: " << dorm.getName() << " | Room: " << room->getName() << "\n";
}

void University::displayAllFullRooms() {
    std::cout << "\n--- Full Rooms ---\n";
    for (auto& dorm : dormitories)
        for (auto* room : dorm.getFullRooms())
            std::cout << "Dorm: " << dorm.getName() << " | Room: " << room->getName() << "\n";
}

void University::displayAllAvailableRooms() {
    std::cout << "\n--- Available Rooms ---\n";
    for (auto& dorm : dormitories)
        for (auto* room : dorm.getAvailableRooms())
            std::cout << "Dorm: " << dorm.getName() << " | Room: " << room->getName()
                      << " | Occupancy: [" << room->getResidents().size() << "/" << room->getCapacity() << "]\n";
}

void University::editRequest(const RoomRequest& request){
    for(auto& req : roomRequests){
        if(request.studentId == req.studentId) req.status = "APPROVED";
    }
}

std::ostringstream RoomRequest::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::ctime(&now_time);
    return oss;
}