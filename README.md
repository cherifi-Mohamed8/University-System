# University-System

A robust C++ console-based application designed to streamline administrative and student-facing tasks for university housing. The system includes separate portals for administrators and students, managing everything from room allocation and student profiles to daily dining hall menus and secure authentication.

## 🚀 Key Features

### 👤 Administrative Portal
* **Student Management:** Register and track student profiles.
* **Housing & Rooms:** Create and manage dormitories, track real-time room capacity, and handle student room assignments or evictions.
* **Dining Services:** Manage and update daily menus (Breakfast, Lunch, Dinner) per dormitory.
* **Allocation Requests:** Review, approve, or manage pending student housing applications.

### 🎓 Student Portal
* **Profile Tracking:** View personal student profile and current housing status.
* **Housing Requests:** Submit requests for room allocation.
* **Dining Hall:** View real-time daily menus for their assigned or selected dormitory.
* **Account Security:** Secure login and password management.

## 🛠 Tech Stack
* **Language:** C++
* **Storage:** Persistent CSV-based data storage (`data/` directory).
* **Architecture:** Object-Oriented Programming (OOP) with modularized components for authentication, data handling, and domain logic.

## 📋 Project Structure
* **Entry Points:** `admin_main.cpp` and `student_portal_main.cpp` provide separate interfaces for users.
* **Portals:** `admin_portal.cpp` and `student_portal.cpp` manage the user interaction logic and menus.
* **Core Management:** `auth_manager.cpp` (authentication/hashing), `csv_manager.cpp` (persistence), and `university.cpp` (central campus logic).
* **Models:** `student.cpp`, `dorm.cpp`, `room.cpp`, and `restaurant.cpp` encapsulate the domain entities.

## ⚙️ Installation & Usage

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/cherifi-Mohamed8/University-System
    ```

2.  **Database Setup:**
    Ensure a `data/` directory exists in the root folder containing the required CSV files:
    * `students_profiles.csv`
    * `dorms.csv`
    * `rooms.csv`
    * `menus.csv`
    * `room_requests.csv`
    * `students_auth.csv`

3.  **Compile the Project:**
    Using `g++` (or your preferred C++ compiler):
    ```bash
    g++ -std=c++17 -Wall -Iinclude .\src\student.cpp .\src\room.cpp .\src\dorm.cpp .\src\restaurant.cpp .\src\university.cpp .\src\csv_manager.cpp .\src\admin_portal.cpp .\src\admin_main.cpp .\src\auth_manager.cpp -o admin_app.exe
    
    g++ -std=c++17 -Wall -Iinclude .\src\student.cpp .\src\room.cpp .\src\dorm.cpp .\src\restaurant.cpp .\src\university.cpp .\src\csv_manager.cpp .\src\auth_manager.cpp .\src\student_portal.cpp .\src\student_portal_main.cpp -o student_portal.exe
    ```

4.  **Run the Applications:**
    * **Admin System:** `./admin_app.exe`
    * **Student System:** `./student_portal.exe`

## 💡 Notes
* **Data Integrity:** The application relies on specific file structures in the `data/` folder; ensure the application has read/write permissions for these files to persist user changes effectively.

---
*Developed by Cherifi Mohamed Redha.*

to compile admin_app.cpp use:
g++ -std=c++17 -Wall -Iinclude .\src\student.cpp .\src\room.cpp .\src\dorm.cpp .\src\restaurant.cpp .\src\university.cpp .\src\csv_manager.cpp .\src\admin_portal.cpp .\src\admin_main.cpp .\src\auth_manager.cpp -o admin_app.exe 

to compile student_portal.cpp use:
g++ -std=c++17 -Wall -Iinclude .\src\student.cpp .\src\room.cpp .\src\dorm.cpp .\src\restaurant.cpp .\src\university.cpp .\src\csv_manager.cpp .\src\auth_manager.cpp .\src\student_portal.cpp .\src\student_portal_main.cpp -o student_portal.exe