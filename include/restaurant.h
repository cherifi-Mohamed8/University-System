#ifndef RESTAURANT_H
#define RESTAURANT_H
#include <vector>
#include <string>

class Restaurant {
public:
    Restaurant();
    void displayMenu() const;  
    const std::vector<std::string>& getMenuItems(int mealChoice) const;
    void addMenuItem(int mealChoice, const std::string& item);
    void clearMenu();
    void editMenu(); 
    
private:
    std::vector<std::vector<std::string>> todaymenu;
};

#endif