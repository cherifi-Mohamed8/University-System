#include "student.h"
#include "dorm.h"
#include "room.h"
#include <iostream>
#include <regex>

// ────────────────────────────────────────────────────────────────────────
// CONSTRUCTORS & DESTRUCTOR
// ────────────────────────────────────────────────────────────────────────

Student::Student(const std::string& id, const std::string& n, int a, char g, int year, int wilaya, std::string major)
    : name(n), 
      age(a), 
      gender(g), 
      studentID(id), 
      academicYear(year), 
      wilaya(wilaya), 
      major(major), 
      assignedDorm(""), 
      assignedRoom("") 
{
    // Keeping the constructor body empty
}

Student::Student(const Student& other)
    : name(other.name), age(other.age), gender(other.gender), studentID(other.studentID), academicYear(other.academicYear),
      wilaya(other.wilaya), major(other.major), assignedDorm(other.assignedDorm), assignedRoom(other.assignedRoom) {
    // Copy constructor to ensure safe copying of student profiles without losing tracking references
}

Student* Student::createStudent(const std::string& id, const std::string& name, int age, char gender, int academicYear, int wilaya, const std::string& major) {
    if (!Student::isValidID(id)) {
        std::cout << "[ERROR]: Invalid Student ID format. Must be a 12-digit number starting with the BAC year.\n";
        return nullptr;
    }
    return new Student(id, name, age, gender, academicYear, wilaya, major);
}

Student::~Student() {}

std::string Student::getName() const {
    return name;
}

int Student::getAge() const {
    return age;
}

std::string Student::getStudentID() const {
    return studentID;
}

int Student::getAcademicYear() const {
    return academicYear;
}

char Student::getGender() const {
    return gender;
}

std::string Student::getWilaya() const {
    return algeriaWilayas.at(wilaya);
}

std::string Student::getMajor() const {
    return major;
}

int Student::getWilayaNumber() const {
    return wilaya;
}

std::string Student::getAssignedDorm() const {
    return assignedDorm;
}

std::string Student::getAssignedRoom() const {
    return assignedRoom;
}

void Student::setAssignedDorm(const std::string& dorm) {
    assignedDorm = dorm;
}

void Student::setAssignedRoom(const std::string& room) {
    assignedRoom = room;
}

void Student::getAccomodationStatus() const {
    if (!assignedDorm.empty() && !assignedRoom.empty()) {
        std::cout << "  [Housing Placement Status]: ASSIGNED\n"
                  << "   • Building Location: " << assignedDorm << "\n"
                  << "   • Quarter Unit Room: " << assignedRoom << "\n";
    } else {
        std::cout << "  [Housing Placement Status]: UNASSIGNED (Pending Allocation Request)\n";
    }
}

bool Student::operator==(const Student& other) const {
    return this->studentID == other.studentID;
}
    
bool Student::isValidID(const std::string& id){
    const std::regex IDPattern("^20[0-9]{2}[0-9]{8}$");
    return std::regex_match(id, IDPattern);
}

std::map<int, std::string> Student::algeriaWilayas = {
    {1, "Adrar"}, {2, "Chlef"}, {3, "Laghouat"}, {4, "Oum El Bouaghi"}, 
    {5, "Batna"}, {6, "Bejaia"}, {7, "Biskra"}, {8, "Bechar"}, 
    {9, "Blida"}, {10, "Bouira"}, {11, "Tamanrasset"}, {12, "Tebessa"}, 
    {13, "Tlemcen"}, {14, "Tiaret"}, {15, "Tizi Ouzou"}, {16, "Algiers"}, 
    {17, "Djelfa"}, {18, "Jijel"}, {19, "Setif"}, {20, "Saida"}, 
    {21, "Skikda"}, {22, "Sidi Bel Abbes"}, {23, "Annaba"}, {24, "Guelma"}, 
    {25, "Constantine"}, {26, "Medea"}, {27, "Mostaganem"}, {28, "M'Sila"}, 
    {29, "Mascara"}, {30, "Ouargla"}, {31, "Oran"}, {32, "El Bayadh"}, 
    {33, "Illizi"}, {34, "Bordj Bou Arreridj"}, {35, "Boumerdes"}, {36, "El Tarf"}, 
    {37, "Tindouf"}, {38, "Tissemsilt"}, {39, "El Oued"}, {40, "Khenchela"}, 
    {41, "Souk Ahras"}, {42, "Tipaza"}, {43, "Mila"}, {44, "Ain Defla"}, 
    {45, "Naama"}, {46, "Ain Temouchent"}, {47, "Ghardaia"}, {48, "Relizane"},
    {49, "Timimoun"}, {50, "Bordj Badji Mokhtar"}, {51, "Ouled Djellal"}, {52, "In Salah"}, 
    {53, "In Guezzam"}, {54, "Touggourt"}, {55, "Djanet"}, {56, "El M'Ghair"}, 
    {57, "Beni Abbes"}, {58, "El Meniaa"}, {59, "Aflou"}, {60, "El Abiodh Sidi Cheikh"}, 
    {61, "El Aricha"}, {62, "El Kantara"}, {63, "Barika"}, {64, "Bou Saada"}, 
    {65, "Bir El Ater"}, {66, "Ksar El Boukhari"}, {67, "Ksar Chellala"}, {68, "Ain Oussara"}, 
    {69, "Messaad"}
};