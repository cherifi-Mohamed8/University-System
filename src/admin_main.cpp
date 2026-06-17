#include "university.h"
#include "csv_manager.h"
#include "admin_portal.h"
#include <iostream>

int main() {
    University university;

    if (!CSVManager::loadAllData(university)) {
        std::cerr << "[CRITICAL ERROR] Failed to load data records from file storage.\n";
        std::cerr << "Exiting application context block safely.\n";
        return 1;
    }

    std::cout << "==================================================\n";
    std::cout << "      UNIVERSITY ADMINISTRATIVE UTILITY SYSTEM     \n";
    std::cout << "==================================================\n";
    std::cout << "  Status: SECURE LOCALHOST INITIALIZED\n";
    std::cout << "  Active Student Profiles: " << university.getStudents().size() << "\n";
    std::cout << "  Managed Campus Dorms   : " << university.getDormitories().size() << "\n";
    std::cout << "==================================================\n";

    AdminPortal portal(university);

    portal.run();

    if (CSVManager::saveAllData(university)) {
        std::cout << "\n[System Shutdown] Active changes saved. Connection terminated smoothly.\n";
    } else {
        std::cerr << "\n[CRITICAL WARNING] App closed but system failed to write modifications back to data disk!\n";
    }

    return 0;
}