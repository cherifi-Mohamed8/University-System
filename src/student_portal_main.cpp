#include "university.h"
#include "csv_manager.h"
#include "auth_manager.h"
#include "student_portal.h"
#include <iostream>

int main() {
    University university;

    if (!CSVManager::loadAllData(university)) {
        std::cerr << "[CRITICAL ERROR] Failed to load data records from file storage.\n";
        return 1;
    }

    AuthManager securityGate(university);
    std::string validatedStudentID = securityGate.runAuthLoop();

    if (validatedStudentID.empty()) {
        std::cout << "[System] Connection closed by user request. Goodbye.\n";
        return 0;
    }

    StudentPortal portal(university, validatedStudentID);
    portal.run();

    if (CSVManager::saveAllData(university)) {
        std::cout << "\n[Session Closed] Your data changes have been locked safely to storage disk.\n";
    } else {
        std::cerr << "\n[CRITICAL WARNING] Modifications made but system failed to commit files to disk!\n";
    }

    return 0;
}