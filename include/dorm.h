#ifndef DORM_H
#define DORM_H
#include <vector>
#include <string>

class Room;
class Restaurant;

class Dorm {
public:
    Dorm();
    Dorm(const std::string& name);

    //opertor overloading
    bool operator==(const Dorm& other) const;

    //getters
    std::string getName() const;
    int getCapacity() const;
    std::vector<Room>& getRooms();
    const std::vector<Room>& getRooms() const;
    std::vector<Room*> getEmptyRooms();
    std::vector<Room*> getFullRooms();
    std::vector<Room*> getAvailableRooms();
    Restaurant* getRestaurant() const;    
    
    Room* findRoom(const std::string& roomName);
    void addRoom(Room&& room);
    void RoomCheck();            
    ~Dorm();
    
private:
    std::string name;
    std::vector<Room> rooms;
    std::vector<Room> emptyRooms;
    std::vector<Room> fullRooms;
    std::vector<Room> availableRooms;
    int capacity = 0;
    Restaurant* restaurant;
    static int dormCount;
};

#endif