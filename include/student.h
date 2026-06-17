#ifndef STUDENT_H
#define STUDENT_H
#include <string>
#include <map>

class Student {
public:
    Student(const Student& other);
    static Student* createStudent(const std::string& id, const std::string& name, int age, char gender, int academicYear, int wilaya, const std::string& major);

    //getters
    std::string getName() const;
    int getAge() const;
    std::string getStudentID() const;
    int getAcademicYear() const;
    char getGender() const;
    std::string getWilaya() const;
    std::string getMajor() const;
    int getWilayaNumber() const;
    std::string getAssignedDorm() const;
    std::string getAssignedRoom() const;
    void getAccomodationStatus() const;
    
    //setters
    void setAssignedDorm(const std::string& dorm);
    void setAssignedRoom(const std::string& room);

    //operator overloading
    bool operator==(const Student& other) const;
    
    static bool isValidID(const std::string& id);
    ~Student();
    
private:
    Student(const std::string& id, const std::string& name, int age, char gender, int academicYear, int wilaya, std::string major);
    std::string name;
    int age;
    char gender;
    std::string studentID;
    int academicYear;
    int wilaya;
    static std::map<int, std::string> algeriaWilayas;
    std::string major;
    std::string assignedDorm;
    std::string assignedRoom;
    static int studentCount;
};

#endif