#ifndef ADMIN_PORTAL_H
#define ADMIN_PORTAL_H
#include "university.h"

class AdminPortal {
public:
    AdminPortal(University& uni);
    void run();
private:
    University& university;
    void handleAddStudent();
    void handleRemoveStudent();
    void handleAssignRoom();
    void handleEvictStudent();
    void handleAddDorm();
    void handleRemoveDorm();
    void handleDisplayDetailedDorms();
    void handleEditRestaurantMenu();
    void handleDisplayingMenu();
    void handleRoomRequests();
};
#endif