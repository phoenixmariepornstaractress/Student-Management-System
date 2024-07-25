#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <set>

class Student {
public:
    int id;
    std::string name;
    std::unordered_map<std::string, std::vector<int>> subjectGrades;
    std::vector<std::string> attendanceDates;

    Student(int id, std::string name) : id(id), name(name) {}

    void addGrade(const std::string& subject, int grade) {
        subjectGrades[subject].push_back(grade);
    }

    void addGrades(const std::string& subject, const std::vector<int>& newGrades) {
        subjectGrades[subject].insert(subjectGrades[subject].end(), newGrades.begin(), newGrades.end());
    }

    void updateGrade(const std::string& subject, int oldGrade, int newGrade) {
        auto& grades = subjectGrades[subject];
        auto it = std::find(grades.begin(), grades.end(), oldGrade);
        if (it != grades.end()) {
            *it = newGrade;
        }
    }

    void deleteGrade(const std::string& subject, int grade) {
        auto& grades = subjectGrades[subject];
        auto it = std::remove(grades.begin(), grades.end(), grade);
        if (it != grades.end()) {
            grades.erase(it, grades.end());
        }
    }

    double getAverageGrade(const std::string& subject) const {
        auto it = subjectGrades.find(subject);
        if (it == subjectGrades.end() || it->second.empty()) return 0.0;
        int sum = 0;
        for (int grade : it->second) {
            sum += grade;
        }
        return static_cast<double>(sum) / it->second.size();
    }

    double getOverallAverageGrade() const {
        int sum = 0;
        int count = 0;
        for (const auto& pair : subjectGrades) {
            for (int grade : pair.second) {
                sum += grade;
                ++count;
            }
        }
        return count == 0 ? 0.0 : static_cast<double>(sum) / count;
    }

    void markAttendance(const std::string& date) {
        attendanceDates.push_back(date);
    }

    void deleteAttendance(const std::string& date) {
        auto it = std::remove(attendanceDates.begin(), attendanceDates.end(), date);
        if (it != attendanceDates.end()) {
            attendanceDates.erase(it, attendanceDates.end());
        }
    }

    void updateName(const std::string& newName) {
        name = newName;
    }

    void printReport() const {
        std::cout << "Report for Student ID: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;
        for (const auto& pair : subjectGrades) {
            std::cout << "Subject: " << pair.first << ", Grades: ";
            for (int grade : pair.second) {
                std::cout << grade << " ";
            }
            std::cout << "Average: " << std::fixed << std::setprecision(2) << getAverageGrade(pair.first) << std::endl;
        }
        std::cout << "Overall Average Grade: " << std::fixed << std::setprecision(2) << getOverallAverageGrade() << std::endl;
        std::cout << "Attendance Dates: ";
        for (const std::string& date : attendanceDates) {
            std::cout << date << " ";
        }
        std::cout << std::endl;
    }

    bool hasPerfectAttendance(const std::vector<std::string>& allDates) const {
        std::set<std::string> datesSet(attendanceDates.begin(), attendanceDates.end());
        return std::all_of(allDates.begin(), allDates.end(), [&datesSet](const std::string& date) {
            return datesSet.find(date) != datesSet.end();
        });
    }
};

class StudentManagementSystem {
private:
    std::vector<Student> students;

    std::string getCurrentDate() const {
        time_t t = time(0);
        struct tm * now = localtime(&t);
        std::ostringstream oss;
        oss << (now->tm_year + 1900) << '-'
            << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << '-'
            << std::setw(2) << std::setfill('0') << now->tm_mday;
        return oss.str();
    }

public:
    void addStudent(int id, const std::string& name) {
        for (const Student& student : students) {
            if (student.id == id) {
                std::cout << "Student ID " << id << " already exists." << std::endl;
                return;
            }
        }
        students.push_back(Student(id, name));
    }

    void removeStudent(int id) {
        auto it = std::remove_if(students.begin(), students.end(),
                                 [id](const Student& student) { return student.id == id; });
        if (it != students.end()) {
            students.erase(it, students.end());
            std::cout << "Student ID " << id << " removed successfully." << std::endl;
        } else {
            std::cout << "Student ID not found." << std::endl;
        }
    }

    void updateStudentName(int id, const std::string& newName) {
        for (Student& student : students) {
            if (student.id == id) {
                student.updateName(newName);
                std::cout << "Student ID " << id << " name updated successfully." << std::endl;
                return;
            }
        }
        std::cout << "Student ID not found." << std::endl;
    }

    void addGrade(int id, const std::string& subject, int grade) {
        for (Student& student : students) {
            if (student.id == id) {
                student.addGrade(subject, grade);
                return;
            }
        }
        std::cout << "Student ID not found." << std::endl;
    }

    void addGrades(int id, const std::string& subject, const std::vector<int>& newGrades) {
        for (Student& student : students) {
            if (student.id == id) {
                student.addGrades(subject, newGrades);
                return;
            }
        }
        std::cout << "Student ID not found." << std::endl;
    }

    void updateGrade(int id, const std::string& subject, int oldGrade, int newGrade) {
        for (Student& student : students) {
            if (student.id == id) {
                student.updateGrade(subject, oldGrade, newGrade);
                return;
            }
        }
        std::cout << "Student ID not found." << std::endl;
    }

    void deleteGrade(int id, const std::string& subject, int grade) {
        for (Student& student : students) {
            if (student.id == id) {
                student.deleteGrade(subject, grade);
                return;
            }
        }
        std::cout << "Student ID not found." << std::endl;
    }

    void markAttendance(int id, const std::string& date) {
        for (Student& student : students) {
            if (student.id == id) {
                student.markAttendance(date);
                return;
            }
        }
        std::cout << "Student ID not found." << std::endl;
    }

    void deleteAttendance(int id, const std::string& date) {
        for (Student& student : students) {
            if (student.id == id) {
                student.deleteAttendance(date);
                return;
            }
        }
        std::cout << "Student ID not found." << std::endl;
    }

    void generateReport(int id) {
        for (const Student& student : students) {
            if (student.id == id) {
                student.printReport();
                return;
            }
        }
        std::cout << "Student ID not found." << std::endl;
    }

    void viewAllStudents() {
        if (students.empty()) {
            std::cout << "No students available." << std::endl;
            return;
        }

        std::cout << "All Students:" << std::endl;
        for (const Student& student : students) {
            std::cout << "ID: " << student.id << ", Name: " << student.name << std::endl;
        }
    }

    void searchStudentByName(const std::string& name) {
        bool found = false;
        for (const Student& student : students) {
            if (student.name == name) {
                std::cout << "Student found - ID: " << student.id << ", Name: " << student.name << std::endl;
                found = true;
            }
        }
        if (!found) {
            std::cout << "Student not found." << std::endl;
        }
    }

    void getTopStudentInSubject(const std::string& subject) {
        Student* topStudent = nullptr;
        double highestAvg = 0.0;

        for (Student& student : students) {
            double avg = student.getAverageGrade(subject);
            if (avg > highestAvg) {
                highestAvg = avg;
                topStudent = &student;
            }
        }

        if (topStudent) {
            std::cout << "Top Student in " << subject << " - ID: " << topStudent->id << ", Name: " << topStudent->name << ", Average Grade: " << highestAvg << std::endl;
        } else {
            std::cout << "No grades recorded for subject " << subject << std::endl;
        }
    }

    void exportData(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for export." << std::endl;
            return;
        }

        for (const Student& student : students) {
            file << student.id << "," << student.name << "\n";
            for (const auto& pair : student.subjectGrades) {
                file << pair.first;
                for (int grade : pair.second) {
                    file << "," << grade;
                }
                file << "\n";
            }
            for (const std::string& date : student.attendanceDates) {
                file << "attendance," << date << "\n";
            }
        }
        file.close();
        std::cout << "Data exported successfully to " << filename << std::endl;
    }

    void importData(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for import." << std::endl;
            return;
        }

        students.clear();
        std::string line;
        Student* currentStudent = nullptr;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string part;
            std::vector<std::string> parts;
            while (std::getline(iss, part, ',')) {
                parts.push_back(part);
            }

            if (parts.size() < 2) continue;

            if (parts[0] == "attendance") {
                if (currentStudent) {
                    currentStudent->attendanceDates.push_back(parts[1]);
                }
            } else if (parts[1].empty()) {
                int id = std::stoi(parts[0]);
                std::string name = parts[1];
                students.push_back(Student(id, name));
                currentStudent = &students.back();
            } else {
                if (currentStudent) {
                    std::string subject = parts[0];
                    std::vector<int> grades;
                    for (size_t i = 1; i < parts.size(); ++i) {
                        grades.push_back(std::stoi(parts[i]));
                    }
                    currentStudent->addGrades(subject, grades);
                }
            }
        }
        file.close();
        std::cout << "Data imported successfully from " << filename << std::endl;
    }

    void sortStudentsByName() {
        std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.name < b.name;
        });
        std::cout << "Students sorted by name." << std::endl;
    }

    void sortStudentsByID() {
        std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.id < b.id;
        });
        std::cout << "Students sorted by ID." << std::endl;
    }

    void displayAverageGrades() const {
        for (const Student& student : students) {
            std::cout << "ID: " << student.id << ", Name: " << student.name << ", Overall Average Grade: " << student.getOverallAverageGrade() << std::endl;
        }
    }

    void displayStudentsBelowThreshold(int threshold) const {
        for (const Student& student : students) {
            for (const auto& pair : student.subjectGrades) {
                for (int grade : pair.second) {
                    if (grade < threshold) {
                        std::cout << "Student ID: " << student.id << ", Name: " << student.name << " has a grade below " << threshold << " in subject " << pair.first << std::endl;
                        break;
                    }
                }
            }
        }
    }

    void displayStudentsWithNoGrades() const {
        for (const Student& student : students) {
            if (student.subjectGrades.empty()) {
                std::cout << "Student ID: " << student.id << ", Name: " << student.name << " has no grades recorded." << std::endl;
            }
        }
    }

    void generateReportsForAllStudents() {
        for (const Student& student : students) {
            student.printReport();
        }
    }

    void listPerfectAttendanceStudents() const {
        if (students.empty()) {
            std::cout << "No students available." << std::endl;
            return;
        }

        std::vector<std::string> allDates;
        for (const Student& student : students) {
            for (const std::string& date : student.attendanceDates) {
                if (std::find(allDates.begin(), allDates.end(), date) == allDates.end()) {
                    allDates.push_back(date);
                }
            }
        }

        std::cout << "Students with perfect attendance:" << std::endl;
        for (const Student& student : students) {
            if (student.hasPerfectAttendance(allDates)) {
                std::cout << "ID: " << student.id << ", Name: " << student.name << std::endl;
            }
        }
    }

    void generateSummaryStatistics() const {
        std::cout << "Total number of students: " << students.size() << std::endl;
        double totalAverageGrade = 0.0;
        double totalAttendance = 0.0;
        int gradeCount = 0;
        int attendanceCount = 0;

        for (const Student& student : students) {
            for (const auto& pair : student.subjectGrades) {
                for (int grade : pair.second) {
                    totalAverageGrade += grade;
                    ++gradeCount;
                }
            }
            totalAttendance += student.attendanceDates.size();
            ++attendanceCount;
        }

        double overallAverageGrade = gradeCount == 0 ? 0.0 : totalAverageGrade / gradeCount;
        double averageAttendance = attendanceCount == 0 ? 0.0 : totalAttendance / attendanceCount;

        std::cout << "Average grade across all subjects: " << std::fixed << std::setprecision(2) << overallAverageGrade << std::endl;
        std::cout << "Average attendance: " << std::fixed << std::setprecision(2) << averageAttendance << std::endl;
    }
};

int main() {
    StudentManagementSystem sms;
    sms.addStudent(1, "John Doe");
    sms.addStudent(2, "Jane Smith");
    sms.addGrade(1, "Math", 90);
    sms.addGrade(1, "Science", 85);
    sms.addGrade(2, "Math", 80);
    sms.addGrade(2, "Science", 88);
    sms.markAttendance(1, sms.getCurrentDate());

    std::cout << "Students sorted by name:" << std::endl;
    sms.sortStudentsByName();
    sms.viewAllStudents();

    std::cout << "Average grades for all students:" << std::endl;
    sms.displayAverageGrades();

    std::cout << "Students with grades below 70:" << std::endl;
    sms.displayStudentsBelowThreshold(70);

    std::cout << "Students with no grades recorded:" << std::endl;
    sms.displayStudentsWithNoGrades();

    std::cout << "Generating reports for all students:" << std::endl;
    sms.generateReportsForAllStudents();

    std::cout << "Listing students with perfect attendance:" << std::endl;
    sms.listPerfectAttendanceStudents();

    std::cout << "Generating summary statistics:" << std::endl;
    sms.generateSummaryStatistics();

    return 0;
}
