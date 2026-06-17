#ifndef STUDENT_PORTAL_H
#define STUDENT_PORTAL_H
#include "university.h"
#include <string>

class StudentPortal {
public:
    StudentPortal(University& uni, const std::string& loggedInID);
    void run();
    
private:
    University& university;
    std::string studentID;
    Student* currentStudent; 
    void handleViewProfile();
    void handleRequestRoom();
    void handleViewMenu();
    void handleViewRequests();
    void handleChangePassword();
};
#endif