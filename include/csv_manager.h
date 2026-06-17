#ifndef CSV_MANAGER_H
#define CSV_MANAGER_H

#include <string>
#include <vector>
#include "university.h"

class CSVManager {
public:
    CSVManager() = delete;

    //high level functions
    static bool loadAllData(University& university);
    static bool saveAllData(const University& university);
private:
    //files names
    static const std::string STUDENTS_FILE;
    static const std::string DORMS_FILE;
    static const std::string ROOMS_FILE;
    static const std::string MENUS_FILE;
    static const std::string REQUESTS_FILE;

    //functions to load data from files
    static bool loadStudents(University& university);
    static bool loadDormsAndRooms(University& university);
    static bool loadMenus(University& university);
    static bool loadRequests(University& university);

    //functions to save data to files
    static bool saveStudents(const University& university);
    static bool saveDormsAndRooms(const University& university);
    static bool saveMenus(const University& university);
    static bool saveRequests(const University& university);

    //helper functions
    static std::vector<std::string> splitLine(const std::string& line, char delimiter = ',');
    static std::string trim(const std::string& str);
};

#endif