# University-System
to compile admin_app.cpp use:
g++ -std=c++17 -Wall -Iinclude .\src\student.cpp .\src\room.cpp .\src\dorm.cpp .\src\restaurant.cpp .\src\university.cpp .\src\csv_manager.cpp .\src\admin_portal.cpp .\src\admin_main.cpp .\src\auth_manager.cpp -o admin_app.exe 

to compile student_portal.cpp use:
g++ -std=c++17 -Wall -Iinclude .\src\student.cpp .\src\room.cpp .\src\dorm.cpp .\src\restaurant.cpp .\src\university.cpp .\src\csv_manager.cpp .\src\auth_manager.cpp .\src\student_portal.cpp .\src\student_portal_main.cpp -o student_portal.exe