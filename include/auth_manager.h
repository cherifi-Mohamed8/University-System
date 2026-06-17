#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H
#include "university.h"
#include <string>
#include <vector>

struct AuthRecord {
    std::string studentID;
    std::string hashedPW;
};

class AuthManager {
public:
    AuthManager(University& uni);
    std::string runAuthLoop(); 
    bool registerUser(const std::string& id, const std::string& password);
    bool changePassword(const std::string& id, const std::string& newPassword);
    
private:
    University& university;
    std::vector<AuthRecord> authRecords;
    void loadAuth();
    void saveAuth();
    std::string hashPassword(const std::string& pw);
    bool idExists(const std::string& id);
    std::string doRegister();
    std::string doSignIn();
};

#endif