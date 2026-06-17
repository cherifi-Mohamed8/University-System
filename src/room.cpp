#include "room.h"
#include "student.h"
#include <iostream>
#include <algorithm>

Room::Room() : name(""), capacity(0) {
    residents.reserve(4);
}

Room::Room(const std::string& name, int capacity) : name(name), capacity(capacity) {
    residents.reserve(capacity);
}

Room::~Room() {
    residents.clear();
}

std::string Room::getName() const { return name; }
int Room::getCapacity() const { return capacity; }
const std::vector<Student*>& Room::getResidents() const { return residents; }

bool Room::isFull()  const { return static_cast<int>(residents.size()) >= capacity; }
bool Room::isEmpty() const { return residents.empty(); }

void Room::assignStudent(Student* student) {
    if (!student) return;

    if (isFull()) {
        std::cout << "  [Error] Room " << name << " is at full capacity.\n";
        return;
    }

    if (std::find(residents.begin(), residents.end(), student) != residents.end()) {
        std::cout << "  [Warning] Student " << student->getStudentID() << " is already in Room " << name << ".\n";
        return;
    }

    residents.push_back(student);
    student->setAssignedRoom(name);
}

void Room::removeStudent(Student* student) {
    if (!student) return;

    auto it = std::find(residents.begin(), residents.end(), student);
    if (it != residents.end()) {
        residents.erase(it);
        student->setAssignedRoom("");
        student->setAssignedDorm("");
    } else {
        std::cout << "  [Warning] Student " << student->getStudentID()
                  << " is not a resident of Room " << name << ".\n";
    }
}