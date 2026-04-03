/*
Project Title: Attendance Manager - Shortage Predictor
Student Name: Rohan Konduru
Register No: 25BCE1744
Course: Structured Object Oriented Programming
*/

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Student {
private:
    string regNo;
    string name;
    map<string, char> attendanceByDate;

public:
    Student() = default;

    Student(const string &reg, const string &studentName)
        : regNo(reg), name(studentName) {}

    string getRegNo() const {
        return regNo;
    }

    string getName() const {
        return name;
    }

    bool markAttendance(const string &date, char status) {
        if (attendanceByDate.count(date) > 0) {
            return false;
        }

        attendanceByDate[date] = status;
        return true;
    }

    void setAttendanceRecord(const string &date, char status) {
        attendanceByDate[date] = status;
    }

    int getTotalClasses() const {
        return static_cast<int>(attendanceByDate.size());
    }

    int getPresentCount() const {
        int presentCount = 0;

        for (const auto &entry : attendanceByDate) {
            if (entry.second == 'P') {
                presentCount++;
            }
        }

        return presentCount;
    }

    double getAttendancePercentage() const {
        int totalClasses = getTotalClasses();

        if (totalClasses == 0) {
            return 0.0;
        }

        return (static_cast<double>(getPresentCount()) * 100.0) / totalClasses;
    }

    const map<string, char> &getAttendanceRecords() const {
        return attendanceByDate;
    }
};

class AttendanceManager {
private:
    vector<Student> students;
    const string studentsFile = "students.txt";
    const string attendanceFile = "attendance.txt";

    Student *findStudentByRegNo(const string &regNo) {
        for (auto &student : students) {
            if (student.getRegNo() == regNo) {
                return &student;
            }
        }

        return nullptr;
    }

    const Student *findStudentByRegNo(const string &regNo) const {
        for (const auto &student : students) {
            if (student.getRegNo() == regNo) {
                return &student;
            }
        }

        return nullptr;
    }

    bool isValidName(const string &name) const {
        return !name.empty();
    }

    bool isValidDateFormat(const string &date) const {
        if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
            return false;
        }

        for (int index = 0; index < static_cast<int>(date.size()); index++) {
            if (index == 4 || index == 7) {
                continue;
            }

            if (!isdigit(static_cast<unsigned char>(date[index]))) {
                return false;
            }
        }

        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));

        return month >= 1 && month <= 12 && day >= 1 && day <= 31;
    }

    bool isValidStatus(char status) const {
        return status == 'P' || status == 'A';
    }

    string trim(const string &value) const {
        size_t first = value.find_first_not_of(" \t\r\n");
        size_t last = value.find_last_not_of(" \t\r\n");

        if (first == string::npos) {
            return "";
        }

        return value.substr(first, last - first + 1);
    }

public:
    AttendanceManager() {
        loadStudents();
        loadAttendance();
    }

    void addStudent() {
        string regNo;
        string name;

        cout << "Enter register number: ";
        getline(cin, regNo);
        regNo = trim(regNo);

        cout << "Enter student name: ";
        getline(cin, name);
        name = trim(name);

        if (regNo.empty() || !isValidName(name)) {
            cout << "Invalid input. Register number and name cannot be empty.\n";
            return;
        }

        if (findStudentByRegNo(regNo) != nullptr) {
            cout << "Duplicate register number is not allowed.\n";
            return;
        }

        students.emplace_back(regNo, name);
        saveStudents();
        cout << "Student added successfully.\n";
    }

    void markAttendanceForDate() {
        if (students.empty()) {
            cout << "No students available. Add students first.\n";
            return;
        }

        string date;
        cout << "Enter attendance date (YYYY-MM-DD): ";
        getline(cin, date);
        date = trim(date);

        if (!isValidDateFormat(date)) {
            cout << "Invalid date format. Use YYYY-MM-DD.\n";
            return;
        }

        for (auto &student : students) {
            char status;
            cout << "Mark attendance for " << student.getRegNo() << " - "
                 << student.getName() << " (P/A): ";
            cin >> status;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            status = static_cast<char>(toupper(static_cast<unsigned char>(status)));

            if (!isValidStatus(status)) {
                cout << "Invalid attendance value. Only P or A is allowed.\n";
                return;
            }

            if (!student.markAttendance(date, status)) {
                cout << "Attendance for date " << date << " is already marked for "
                     << student.getRegNo() << ".\n";
                return;
            }
        }

        saveAttendance();
        cout << "Attendance saved successfully for " << date << ".\n";
    }

    void viewStudentSummary() const {
        string regNo;
        cout << "Enter register number to view summary: ";
        getline(cin, regNo);
        regNo = trim(regNo);

        const Student *student = findStudentByRegNo(regNo);
        if (student == nullptr) {
            cout << "Student not found.\n";
            return;
        }

        cout << "\nAttendance Summary\n";
        cout << "RegNo: " << student->getRegNo() << '\n';
        cout << "Name: " << student->getName() << '\n';
        cout << "Present Count: " << student->getPresentCount() << '\n';
        cout << "Total Classes: " << student->getTotalClasses() << '\n';
        cout << fixed << setprecision(2);
        cout << "Attendance %: " << student->getAttendancePercentage() << "\n";

        cout << "Daily Records:\n";
        if (student->getAttendanceRecords().empty()) {
            cout << "No attendance records available.\n";
            return;
        }

        for (const auto &entry : student->getAttendanceRecords()) {
            cout << "  " << entry.first << " -> " << entry.second << '\n';
        }
    }

    void showAttendanceReport() const {
        if (students.empty()) {
            cout << "No students available.\n";
            return;
        }

        cout << "\nAttendance Report\n";
        cout << left << setw(15) << "RegNo" << setw(25) << "Name" << "Attendance %\n";
        cout << string(55, '-') << '\n';
        cout << fixed << setprecision(2);

        for (const auto &student : students) {
            cout << left << setw(15) << student.getRegNo()
                 << setw(25) << student.getName()
                 << student.getAttendancePercentage() << '\n';
        }
    }

    void showShortageList() const {
        if (students.empty()) {
            cout << "No students available.\n";
            return;
        }

        bool foundShortage = false;
        cout << "\nStudents Below 75% Attendance\n";
        cout << fixed << setprecision(2);

        for (const auto &student : students) {
            if (student.getAttendancePercentage() < 75.0) {
                foundShortage = true;
                cout << student.getRegNo() << " - " << student.getName()
                     << " -> " << student.getAttendancePercentage() << "%\n";
            }
        }

        if (!foundShortage) {
            cout << "No shortage students found.\n";
        }
    }

    void showClassAverage() const {
        if (students.empty()) {
            cout << "No students available.\n";
            return;
        }

        double totalPercentage = 0.0;
        for (const auto &student : students) {
            totalPercentage += student.getAttendancePercentage();
        }

        double average = totalPercentage / static_cast<double>(students.size());
        cout << fixed << setprecision(2);
        cout << "Class Average Attendance %: " << average << '\n';
    }

    void loadStudents() {
        students.clear();
        ifstream file(studentsFile);

        if (!file.is_open()) {
            return;
        }

        string line;
        while (getline(file, line)) {
            if (trim(line).empty()) {
                continue;
            }

            stringstream parser(line);
            string regNo;
            string name;

            getline(parser, regNo, ',');
            getline(parser, name);

            if (!trim(regNo).empty() && !trim(name).empty()) {
                students.emplace_back(trim(regNo), trim(name));
            }
        }
    }

    void loadAttendance() {
        ifstream file(attendanceFile);

        if (!file.is_open()) {
            return;
        }

        string line;
        while (getline(file, line)) {
            if (trim(line).empty()) {
                continue;
            }

            stringstream parser(line);
            string regNo;
            string date;
            string statusText;

            getline(parser, regNo, ',');
            getline(parser, date, ',');
            getline(parser, statusText);

            Student *student = findStudentByRegNo(trim(regNo));
            if (student == nullptr || trim(statusText).empty()) {
                continue;
            }

            char status = static_cast<char>(toupper(static_cast<unsigned char>(trim(statusText)[0])));
            if (isValidStatus(status) && isValidDateFormat(trim(date))) {
                student->setAttendanceRecord(trim(date), status);
            }
        }
    }

    void saveStudents() const {
        ofstream file(studentsFile);

        if (!file.is_open()) {
            cout << "Error: unable to save students file.\n";
            return;
        }

        for (const auto &student : students) {
            file << student.getRegNo() << ',' << student.getName() << '\n';
        }
    }

    void saveAttendance() const {
        ofstream file(attendanceFile);

        if (!file.is_open()) {
            cout << "Error: unable to save attendance file.\n";
            return;
        }

        for (const auto &student : students) {
            for (const auto &entry : student.getAttendanceRecords()) {
                file << student.getRegNo() << ',' << entry.first << ',' << entry.second << '\n';
            }
        }
    }

    void displayMenu() const {
        cout << "\n===== Attendance Manager - Shortage Predictor =====\n";
        cout << "1. Add student\n";
        cout << "2. Mark attendance for a date\n";
        cout << "3. View student attendance summary\n";
        cout << "4. View attendance % report\n";
        cout << "5. View students below 75%\n";
        cout << "6. View class average attendance %\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
    }
};

int main() {
    AttendanceManager manager;
    int choice = 0;

    do {
        manager.displayMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid menu input. Enter a number between 1 and 7.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            manager.addStudent();
            break;
        case 2:
            manager.markAttendanceForDate();
            break;
        case 3:
            manager.viewStudentSummary();
            break;
        case 4:
            manager.showAttendanceReport();
            break;
        case 5:
            manager.showShortageList();
            break;
        case 6:
            manager.showClassAverage();
            break;
        case 7:
            cout << "Exiting Attendance Manager.\n";
            break;
        default:
            cout << "Invalid choice. Please select between 1 and 7.\n";
            break;
        }
    } while (choice != 7);

    return 0;
}
