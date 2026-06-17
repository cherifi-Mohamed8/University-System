#ifndef ROOM_H
#define ROOM_H
#include <vector>
#include <string>

class Student;

class Room {
public:
    Room();
    Room(const std::string& name, int capacity);

    //getters
    std::string getName() const;
    const std::vector<Student*>& getResidents() const;
    int getCapacity() const;
    
    //functions to assign and remove students from rooms
    void assignStudent(Student* student);
    void removeStudent(Student* student);
    
    bool isFull() const;
    bool isEmpty() const;
    ~Room();
    
private:
    std::string name;
    int capacity;
    std::vector<Student*> residents;
    bool isFullFlag = false; 
    bool isEmptyFlag = true;
};

#endif