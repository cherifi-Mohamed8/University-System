#include "dorm.h"
#include "room.h"
#include "restaurant.h"
#include <iostream>

int Dorm::dormCount = 0;

Dorm::Dorm() : name(""), capacity(0), restaurant(new Restaurant()) { dormCount++; }

Dorm::Dorm(const std::string& name) : name(name), capacity(0), restaurant(new Restaurant()) { dormCount++; }

Dorm::~Dorm() {
    delete restaurant;
}

std::string Dorm::getName() const { return name; }

int Dorm::getCapacity() const {
    int total = 0;
    for (const auto& room : rooms) total += room.getCapacity();
    return total;
}

std::vector<Room>& Dorm::getRooms() { return rooms; }
const std::vector<Room>& Dorm::getRooms() const { return rooms; }

Restaurant* Dorm::getRestaurant() const { return restaurant; }

void Dorm::addRoom(Room&& room) {
    rooms.push_back(std::move(room));
    capacity++;
}

Room* Dorm::findRoom(const std::string& roomName) {
    for (auto& room : rooms) {
        if (room.getName() == roomName) return &room;
    }
    return nullptr;
}

std::vector<Room*> Dorm::getEmptyRooms() {
    std::vector<Room*> result;
    for (auto& room : rooms)
        if (room.isEmpty()) result.push_back(&room);
    return result;
}

std::vector<Room*> Dorm::getFullRooms() {
    std::vector<Room*> result;
    for (auto& room : rooms)
        if (room.isFull()) result.push_back(&room);
    return result;
}

std::vector<Room*> Dorm::getAvailableRooms() {
    std::vector<Room*> result;
    for (auto& room : rooms)
        if (!room.isFull()) result.push_back(&room);
    return result;
}

bool Dorm::operator==(const Dorm& other) const { return name == other.name; }