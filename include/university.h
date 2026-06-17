#ifndef UNIVERSITY_H
#define UNIVERSITY_H
#include <vector>
#include <string>
#include <functional>
#include <chrono>
#include <ctime>
#include <sstream>
#include "student.h"
#include "dorm.h"

struct RoomRequest {
    static std::ostringstream getCurrentDateTime();

    std::string studentId;
    std::string dormName;
    std::string status = "PENDING";
    std::string date = getCurrentDateTime().str();

    bool operator==(const RoomRequest& other) const {
    return this->studentId == other.studentId && this->dormName == other.dormName;
    }
};

class University {
public:
    University();

    void addNewStudent(const std::string& studentID, const std::string& name, int age, char gender, int academicYear, int wilaya, const std::string& major);
    void addStudent(const Student& student);
    void removeStudent(const std::string& studentID);
    void addDorm(const std::string& dormName);
    void removeDorm(const std::string& dormName);
    void addRoomRequest(const RoomRequest& request);

    //getters
    const std::vector<Student>& getStudents() const;
    const std::vector<Dorm>& getDormitories() const;
    std::vector<Dorm>& getDormitories();
    const std::vector<RoomRequest>& getRoomRequests() const;

    //functions to search for objects (students, dorms, requests...)
    Student* findStudentByID(const std::string& studentID);
    Dorm* findDormByName(const std::string& name);
    RoomRequest* findRequestByID(const std::string& studentID); 

    //functions to display universtiy utilities
    void displayAllStudents() const;
    void displayAllDormitories() const;
    void displayAllEmptyRooms();
    void displayAllFullRooms();
    void displayAllAvailableRooms();
    
    void editRequest(const RoomRequest& request); 
    ~University();
private:
    std::vector<Student> students;
    std::vector<Dorm> dormitories;
    std::vector<RoomRequest> roomRequests;
};

#endif