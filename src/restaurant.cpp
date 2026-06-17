#include "restaurant.h"
#include <iostream>
#include <limits>
#include <algorithm>

Restaurant::Restaurant() {
    todaymenu.reserve(3);
    for(auto& meal : todaymenu){
        meal.reserve(5);
    }
}

void Restaurant::displayMenu() const {
    const std::string mealNames[3] = {"Breakfast", "Lunch", "Dinner"};
    bool menuIsEmpty = true;

    std::cout << "\n=========== TODAY'S DINING HALL MENU ===========\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << " [" << mealNames[i] << "]:\n";
        if (todaymenu[i].empty()) {
            std::cout << "   (No items listed for this meal period)\n";
        } else {
            menuIsEmpty = false;
            for (const auto& item : todaymenu[i]) {
                std::cout << "   • " << item << "\n";
            }
        }
        std::cout << "----------------───────────────────────────────\n";
    }
    
    if (menuIsEmpty) {
        std::cout << "  [Notice] The dining services staff has not published a menu yet.\n";
    }
}

const std::vector<std::string>& Restaurant::getMenuItems(int mealType) const {
    return todaymenu[mealType];
}

void Restaurant::addMenuItem(int mealType, const std::string& item) {
    if (todaymenu.size() < 3) {
        todaymenu.resize(3);
    }

    auto& currentMealVector = todaymenu[mealType];
    auto it = std::find(currentMealVector.begin(), currentMealVector.end(), item);
    
    if (it == currentMealVector.end()) {
        currentMealVector.push_back(item);
    } else {
        std::cout << "  [Notice] '" << item << "' is already listed in this meal category.\n";
    }
}

void Restaurant::clearMenu() {
    for (int i = 0; i < 3; ++i) {
        todaymenu[i].clear();
    }
}

void Restaurant::editMenu() {
    while (true) {
        std::cout << "\n--- Edit Restaurant Menu ---\n";
        std::cout << " 1. Add item to Breakfast\n"
                  << " 2. Add item to Lunch\n"
                  << " 3. Add item to Dinner\n"
                  << " 4. Wipe/Clear Today's Menu Entirely\n"
                  << " 0. Cancel / Go Back\n"
                  << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  Invalid input format. Try again.\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "  [System] Returning to previous menu layout.\n";
            break;
        }

        if (choice >= 1 && choice <= 3) {
            int targetMealIndex = choice - 1;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Enter the name of the dish/food item: ";
            std::string dishName;
            std::getline(std::cin, dishName);

            if (!dishName.empty()) {
                if (todaymenu.size() < 3) {
                    todaymenu.resize(3);
                }

                const auto& currentMealItems = todaymenu[targetMealIndex];
                auto it = std::find(currentMealItems.begin(), currentMealItems.end(), dishName);

                if (it != currentMealItems.end()) {
                    std::cout << "  [Error] Cannot add duplicate item! \"" << dishName << "\" is already on this menu.\n";
                } else {
                    addMenuItem(targetMealIndex, dishName);
                    std::cout << "  [Success] \"" << dishName << "\" added to the list.\n";
                }
            }
        } 
        else if (choice == 4) {
            clearMenu();
            std::cout << "  [Success] All menu choices wiped clean.\n";
        } 
        else {
            std::cout << "  Invalid entry selection. Choose [0-4].\n";
        }
    }
}