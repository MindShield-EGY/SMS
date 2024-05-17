#include <iostream>
  #include <sstream>
  #include <fstream>
  #include <string>
  #include <stdlib.h>
  #include <array>
  #include <vector>
  #include <cmath>
  #include <cstdlib>   // For system function
  #include <chrono>    // For delay
  #include <thread>    // For delay
using namespace std; ///MASRKAI
/////////////////////////////////

const int MAX_COURSES = 100; // Maximum number of courses per student
const int MAX_STUDENTS = 100000;


void ClearTerminal() {  // Ergonomics
 #ifdef _WIN32
     system("cls");
  #else
     system("clear");
 #endif
 }                       ///MASRKAI
/////////////////////////////////

#ifdef _WIN32       //--System Integrity & Cross platform support
 #include <windows.h>
 #define RESET_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
 #else
 #define RESET_COLOR "\033[0m"
 #endif                                                                                                                   ///MASRKAI
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Function to print colored and bold text
void printColor(const string& color, const string& text, bool bold = false) {
 #define RED     "\033[31m" //Defining Red
 #define GREEN   "\033[32m" //Defining Green
 #define YELLOW  "\033[33m" //Defining Yellow
 #define BLUE    "\033[34m" //Defining Blue
 #define MAGENTA "\033[35m" //Defining Magenta
 #define CYAN    "\033[36m" //Defining Cyan
 #define BOLD    "\033[1m"  //Defining Bold

                cout << (bold ? BOLD : "") << color << text << RESET_COLOR; } ///MASRKAI
////////////////////////////////////////////////////////////////////////////////////////

class LimiterBase {

 protected:
    virtual void drawFrame(char c1, char c2, char c3, int loops ,int milliseconds) {
         for (int i = 0 ; i <= loops ; ++i) {
            cout << " " << c1;
            this_thread::sleep_for(chrono::milliseconds(milliseconds));
            cout << "\b\b"; // Erase the character
            cout.flush();
            cout << " " << c2;
            this_thread::sleep_for(chrono::milliseconds(milliseconds));
            cout << "\b\b"; // Erase the character
            cout.flush();
            cout << " " << c3;
            this_thread::sleep_for(chrono::milliseconds(milliseconds));
            cout << "\b\b"; // Erase the character

            // ! Erasing animation
            this_thread::sleep_for(chrono::milliseconds(100));
            cout << "\b\b";
            cout.flush(); }
            this_thread::sleep_for(chrono::milliseconds(milliseconds)); }

  public:
    virtual void start() = 0; };                                                  //MASRKAI
///////////////////////////////////////////////////////////////////////////////////////////

class Limiter : public LimiterBase {
  public:
    void start() override {
        printColor(RED, "Limiter is Active", true);
        drawFrame('-', '/', '|', 9, 80);
        ClearTerminal(); } };                                    ///MASRKAI
//////////////////////////////////////////////////////////////////////////

class CLimiter : public LimiterBase {
  public:
    void start() override {
        drawFrame('-', '/', '|',9, 50);
        ClearTerminal(); } };



// Course class to store course information
class Course {
private:
    string courseId;
    string courseName;
    double creditHours;
    double percentageGrade;
public:
    Course(const string& courseId, const string& courseName, double creditHours)
            : courseId(courseId), courseName(courseName), creditHours(creditHours), percentageGrade(-1) {}

    void setGrade(double percentageGrade) {
        this->percentageGrade = percentageGrade;
    }

    double getGrade() const {
        return percentageGrade;
    }

    string getCourseId() const {
        return courseId;
    }


    double getCreditHours() const {
        return creditHours;
    }

    string getLetterGrade() const {
        if (percentageGrade >= 97 && percentageGrade <= 100) return "A+";
        else if (percentageGrade >= 93 && percentageGrade < 97) return "A";
        else if (percentageGrade >= 89 && percentageGrade < 93) return "A-";
        else if (percentageGrade >= 84 && percentageGrade < 89) return "B+";
        else if (percentageGrade >= 80 && percentageGrade < 84) return "B";
        else if (percentageGrade >= 76 && percentageGrade < 80) return "B-";
        else if (percentageGrade >= 73 && percentageGrade < 76) return "C+";
        else if (percentageGrade >= 70 && percentageGrade < 73) return "C";
        else if (percentageGrade >= 67 && percentageGrade < 70) return "C-";
        else if (percentageGrade >= 64 && percentageGrade < 67) return "D+";
        else if (percentageGrade >= 60 && percentageGrade < 64) return "D";
        else if (percentageGrade < 0 || percentageGrade > 100) return "null"; // If percentage is negative or greater than 100, return "null"
        else return "F";
    }

    double getGradePoints() const {
        string letterGrade = getLetterGrade();
        if (letterGrade == "A+" || letterGrade == "A") return 4.0;
        else if (letterGrade == "A-") return 3.7;
        else if (letterGrade == "B+") return 3.3;
        else if (letterGrade == "B") return 3.0;
        else if (letterGrade == "B-") return 2.7;
        else if (letterGrade == "C+") return 2.3;
        else if (letterGrade == "C") return 2.0;
        else if (letterGrade == "C-") return 1.7;
        else if (letterGrade == "D+") return 1.3;
        else if (letterGrade == "D") return 1.0;
        else if (letterGrade == "F") return 0.0;
        else return 0.0; // Return 0.0 for "null" grade
    }

    void displayInfo() const {
        cout << "Course ID: " << courseId << "\tCourse Name: " << courseName
             << "\tCredit Hours: " << creditHours << "\tGrade: " << getLetterGrade()
             << " (" << getGradePoints() << " points)" << endl;
    }

    void editCourseInfo(const string& newCourseName, double newCreditHours) {
        courseName = newCourseName;
        creditHours = newCreditHours;
        cout << "Course details updated." << endl;
    }
};

// User class to store user information
class User {
protected:
    int id;
    string name;
public:
    User(int id, const string& name) : id(id), name(name) {}

    void displayInfo() const {
        cout << "ID: " << id << "\tName: " << name << endl;
    }

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }
};

// Student class derived from User
class Student : public User {
private:
    Course* courses[MAX_COURSES]; // Array to store courses
    int courseCount;
public:
    friend class Administrator;
    Student(int id, const string& name) : User(id, name), courseCount(0) {}

    void addCourse(const string& courseId, const string& courseName, double creditHours) {
        if (courseCount < MAX_COURSES) {
            courses[courseCount++] = new Course(courseId, courseName, creditHours);
        }
    }

    void removeCourse(const string& courseId) {
        for (int i = 0; i < courseCount; ++i) {
            if (courses[i]->getCourseId() == courseId) {
                delete courses[i];
                for (int j = i; j < courseCount - 1; ++j) {
                    courses[j] = courses[j + 1];
                }
                courseCount--;
                cout << "Course with ID " << courseId << " removed for student with ID " << getId() << "." << endl;
                return;
            }
        }
        cout << "Course with ID " << courseId << " not found for student with ID " << getId() << "." << endl;
    }

    void inputGrade(const string& courseId, double percentageGrade) {
        for (int i = 0; i < courseCount; ++i) {
            if (courses[i]->getCourseId() == courseId) {
                courses[i]->setGrade(percentageGrade);
                cout << "Grade added for course ID " << courseId << " for student with ID " << getId() << "." << endl;
                return;
            }
        }
        cout << "Course with ID " << courseId << " not found for student with ID " << getId() << "." << endl;
    }

    double calculateGPA() const {
        double totalPoints = 0;
        double totalCreditHours = 0;
        for (int i = 0; i < courseCount; ++i) {
            if (courses[i]->getGrade() >= 0) { // Only consider graded courses for GPA calculation
                totalPoints += courses[i]->getGradePoints() * courses[i]->getCreditHours();
                totalCreditHours += courses[i]->getCreditHours();
            }
        }
        if (totalCreditHours > 0) {
            return totalPoints / totalCreditHours;
        }
        return 0; // Return 0 if no courses
    }

    void displayCourses() const {
        for (int i = 0; i < courseCount; ++i) {
            courses[i]->displayInfo();
        }
    }

    void editUserInfo(const string& newName) {
        name = newName;
        cout << "\nStudent details updated." << endl;
    }

    void editCourseInfo(const string& courseId, const string& newCourseName, double newCreditHours) {
        for (int i = 0; i < courseCount; ++i) {
            if (courses[i]->getCourseId() == courseId) {
                courses[i]->editCourseInfo(newCourseName, newCreditHours);
                cout << "Course details updated for student with ID " << getId() << "." << endl;
                return;
            }
        }
        cout << "Course with ID " << courseId << " not found for student with ID " << getId() << "." << endl;
    }

    int getCourseIndex(const string& courseId) const {
        for (int i = 0; i < courseCount; ++i) {
            if (courses[i]->getCourseId() == courseId) {
                return i;
            }
        }
        return -1; // Course not found
    }

    ~Student() {
        for (int i = 0; i < courseCount; ++i) {
            delete courses[i];
        }
    }
};

// Administrator class derived from User
class Administrator : public User {
private:
    pair<string, pair<string, double>> courseInfo[MAX_COURSES]; // Array to store course information linked with course ID
    Student* students[MAX_STUDENTS];
    int studentCount;
    int courseInfoCount;
public:
    Administrator(int id, const string& name) : User(id, name), studentCount(0), courseInfoCount(0) {}

    void addCourseInfo(const string& courseId, const string& courseName, double creditHours) {
        if (courseInfoCount < MAX_COURSES) {
            courseInfo[courseInfoCount++] = make_pair(courseId, make_pair(courseName, creditHours));
        }
    }

    pair<string, double> getCourseInfo(const string& courseId) {
        for (int i = 0; i < courseInfoCount; ++i) {
            if (courseInfo[i].first == courseId) {
                return courseInfo[i].second;
            }
        }
        return make_pair("", 0.0); // Return empty pair if course not found
    }

    void addStudent(int id, const string& name) {
        int x = 0;
        for (int i = 0; i < studentCount; ++i) {
            if (students[i]->getId() == id) {
                cout << "Duplicated id (This data won't be saved)" << endl;
                x = 1;
            }
        }
        if (studentCount < MAX_STUDENTS && x == 0) {
            students[studentCount++] = new Student(id, name);
            cout << "Student added with ID " << id << ".\nStudent name is " << name << "." << endl;
            x = 1;
        }
    }

    void removeStudent(int id) {
        for (int i = 0; i < studentCount; ++i) {
            if (students[i]->getId() == id) {
                delete students[i];
                for (int j = i; j < studentCount - 1; ++j) {
                    students[j] = students[j + 1];
                }
                studentCount--;
                cout << "Student with ID " << id << " removed." << endl;
                return;
            }
        }
        cout << "Student with ID " << id << " not found." << endl;
    }

    void removeStudentFromCourse(int studentId, const string& courseId) {
        for (int i = 0; i < studentCount; ++i) {
            if (students[i]->getId() == studentId) {
                students[i]->removeCourse(courseId);
                return;
            }
        }
        cout << "Student with ID " << studentId << " not found." << endl;
    }

    Student* findStudent(int id) {
        for (int i = 0; i < studentCount; ++i) {
            if (students[i]->getId() == id) {
                return students[i];
            }
        }
        return nullptr;
    }

    void addCourseToStudent(int studentId, const string& courseId) {
        Student* student = findStudent(studentId);
        if (student) {
            pair<string, double> courseInfo = getCourseInfo(courseId);
            if (!courseInfo.first.empty()) {
                student->addCourse(courseId, courseInfo.first, courseInfo.second);
                cout << "Course " << courseInfo.first << " added for student ID " << studentId << "." << endl;
            }
            else {
                cout << "Course with ID " << courseId << " not found." << endl;
            }
        }
        else {
            cout << "Student with ID " << studentId << " not found." << endl;
        }
    }

    void displayStudents() const {
        for (int i = 0; i < studentCount; ++i) {
            students[i]->displayInfo();
            students[i]->displayCourses();
            cout << "GPA: " << students[i]->calculateGPA() << endl;
        }
    }

    // Bubble sort implementation to rank students based on GPA
    void rankStudents() {
        for (int i = 0; i < studentCount - 1; ++i) {
            for (int j = 0; j < studentCount - i - 1; ++j) {
                if (students[j]->calculateGPA() < students[j + 1]->calculateGPA()) {
                    // Swap students
                    Student* temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }

        cout << "\n  Rank\tStudent ID\tStudent Name\tGPA" << endl;
        cout << "------------------------------------------------" << endl;
        for (int i = 0; i < studentCount; ++i) {
            cout << "   " << i + 1 << "\t " << students[i]->getId() << "\t" << students[i]->getName() << "\t";
            cout << students[i]->calculateGPA() << endl;
        }
    }

    void addCourse(const string& courseId, const string& courseName, double creditHours) {
        addCourseInfo(courseId, courseName, creditHours);
        cout << "Course " << courseName << " added with ID " << courseId << "." << endl;
    }

    void removeCourse(const string& courseId) {
        // Remove course from courseInfo
        int indexToRemove = -1;
        for (int i = 0; i < courseInfoCount; ++i) {
            if (courseInfo[i].first == courseId) {
                indexToRemove = i;
                break;
            }
        }
        if (indexToRemove != -1) {
            for (int i = indexToRemove; i < courseInfoCount - 1; ++i) {
                courseInfo[i] = courseInfo[i + 1];
            }
            courseInfoCount--;
        }

        // Remove course from all students
        for (int i = 0; i < studentCount; ++i) {
            students[i]->removeCourse(courseId);
        }

        cout << "Course with ID " << courseId << " removed." << endl;
    }

    void removeAllStudents() {
        for (int i = 0; i < studentCount; ++i) {
            delete students[i];
        }
        studentCount = 0;
        cout << "All students removed." << endl;
    }

    void removeAllCourses() {
        courseInfoCount = 0;
        for (int i = 0; i < studentCount; ++i) {
            students[i]->removeCourse(courseInfo[i].first);
        }
        cout << "All courses removed." << endl;
    }

    // Edit course details
    void editCourseDetails(const string& courseId, const string& newCourseName, double newCreditHours) {
        for (int i = 0; i < courseInfoCount; ++i) {
            if (courseInfo[i].first == courseId) {
                // Update course info
                courseInfo[i].second.first = newCourseName;
                courseInfo[i].second.second = newCreditHours;
                // Update course details for all enrolled students
                for (int j = 0; j < studentCount; ++j) {
                    students[j]->editCourseInfo(courseId, newCourseName, newCreditHours);
                }
                cout << "\nCourse details updated." << endl;
                return;
            }
        }
        cout << "Course with ID " << courseId << " not found." << endl;
    }

    // Display enrolled students for a specific course
    void displayEnrolledStudents(const string& courseId) const {
        cout << "Enrolled students for course with ID " << courseId << ":" << endl;
        bool found = false;
        for (int i = 0; i < studentCount; ++i) {
            if (students[i]->getCourseIndex(courseId) != -1) {
                found = true;
                cout << "Student ID: " << students[i]->getId() << "\tName: " << students[i]->getName() << endl;
            }
        }
        if (!found) {
            cout << "No students enrolled in course with ID " << courseId << "." << endl;
        }
    }

    void readDataFromFile(const string& filename) {
        ifstream infile(filename);
        if (infile.is_open()) {
            string line;
            while (getline(infile, line)) {
                istringstream iss(line);
                char type;
                iss >> type;
                if (type == 'S') {
                    int id;
                    string name;
                    iss >> id;
                    getline(iss, name);
                    addStudent(id, name);
                }
                else if (type == 'C') {
                    string courseId, courseName;
                    double creditHours;
                    iss >> courseId >> courseName >> creditHours;
                    addCourseInfo(courseId, courseName, creditHours);
                }
                else if (type == 'E') {
                    int studentId;
                    string courseId;
                    iss >> studentId >> courseId;
                    addCourseToStudent(studentId, courseId);
                }
                else if (type == 'G') {
                    int studentId;
                    string courseId;
                    double percentageGrade;
                    iss >> studentId >> courseId >> percentageGrade;
                    findStudent(studentId)->inputGrade(courseId, percentageGrade);
                }
            }
            infile.close();
        }
        else {
            cerr << "Unable to open file for reading.";
        }
    }

    void writeDataToFile(const string& filename, Administrator& admin) {
        ofstream outfile(filename);
        if (outfile.is_open()) {
            // Write course info
            for (int i = 0; i < admin.courseInfoCount; ++i) {
                outfile << "C " << admin.courseInfo[i].first << " " << admin.courseInfo[i].second.first << " " << admin.courseInfo[i].second.second << endl;
            }

            // Write students and their courses
            for (int i = 0; i < admin.studentCount; ++i) {
                outfile << "S " << admin.students[i]->getId() << " " << admin.students[i]->getName() << endl;
                for (int j = 0; j < admin.students[i]->courseCount; ++j) {
                    outfile << "E " << admin.students[i]->getId() << " " << admin.students[i]->courses[j]->getCourseId() << endl;
                    double grade = admin.students[i]->courses[j]->getGrade();
                    if (grade >= 0) {
                        outfile << "G " << admin.students[i]->getId() << " " << admin.students[i]->courses[j]->getCourseId() << " " << grade << endl;
                    }
                }
            }

            outfile.close();
            cout << "Data successfully written to file: " << filename << endl;
        } else {
            cerr << "Unable to open file for writing: " << filename << endl;
        }
    }


    ~Administrator() {
        for (int i = 0; i < studentCount; ++i) {
            delete students[i];
        }
    }
};


void displayMenu() {
 printColor(BLUE ,"1. List all students\n",false);
 printColor(BLUE ,"2. Add new student\n",false);
 printColor(BLUE ,"3. Register subjects for a student\n",false);
 printColor(BLUE ,"4. Change subjects for a student\n",false);
 printColor(BLUE ,"5. Add new subject code\n",false);
 printColor(BLUE ,"6. View enrolled courses for a student\n",false);
 printColor(BLUE ,"7. Calculate GPA for a student\n",false);
 printColor(YELLOW ,"8. Clean Terminal\n",false);
 printColor(RED ,"9. Exit\n",false);       }  




void menu_of_options(){
    printColor(GREEN,"\n***** Welcome to Student Managment System *****\n\n",true);

    printColor(BLUE, "1. Add Student\n", false);
    printColor(BLUE, "2. Add Course\n", false);
    printColor(BLUE, "3. Enroll Student To A Course\n", false);
    printColor(BLUE, "4. Input Grade for Course\n\n", false);

    printColor(YELLOW, "6.  Edit Course Details\n", false);
    printColor(YELLOW, "7.  Find Specific Student\n", false);
    printColor(YELLOW, "5.  Edit Student Details\n", false);
    printColor(YELLOW, "8.  Display Enrolled Students for a Specific Course\n", false);
    printColor(YELLOW, "9.  Display Detailed Students info\n", false);
    printColor(YELLOW, "10. Rank Students by GPA\n\n", false);

    printColor(RED, "11. Remove Course\n", false);
    printColor(RED, "12. Remove Student\n", false);
    printColor(RED, "13. Remove Student from Course\n", false);
    printColor(RED, "14. Remove All Students\n", false);
    printColor(RED, "15. Remove All Courses\n", false);
    printColor(RED ,"16. Exit\n",true);
    cout << "\n";
}

int main() {
    Administrator admin1(1, "Program Director");

    admin1.readDataFromFile("C:\\Users\\awaad\\OneDrive\\Documents\\PROJECT\\project.txt");

    int choice, studentId;
    double grade;
    string studentName, courseId, newCourseName;
    double newCreditHours;

    do {  
        menu_of_options();
        cout << "Enter your choice: ";
        cin >> choice;
         if (cin.fail()) {
            cin.clear();
            cin.ignore(256, '\n');
            cout << "Invalid input. Please enter a number (1-9): ";
            ClearTerminal();
            continue;}

        switch (choice) {
            case 1:
                cout << "Enter Student ID: ";
                cin >> studentId;
                cin.ignore();
                cout << "Enter Student Name(First Name, Last Name): ";
                getline(cin, studentName);
                admin1.addStudent(studentId, studentName);
                break;

            case 2:
                cout << "Enter Course ID: ";
                cin >> courseId;
                cin.ignore();
                cout << "Enter Course Name: ";
                getline(cin, studentName);
                cout << "Enter Credit Hours: ";
                double creditHours;
                cin >> creditHours;
                admin1.addCourse(courseId, studentName, creditHours);
                break;

            case 3:
                cout << "Enter Student ID: ";
                cin >> studentId;
                cin.ignore();
                if (admin1.findStudent(studentId)) {
                    cout << "Enter Course ID: ";
                    getline(cin, courseId);
                    admin1.addCourseToStudent(studentId, courseId);
                }
                else {
                    cout << "Student with ID " << studentId << " not found." << endl;
                }
                break;

            case 4:
                cout << "Enter Student ID: ";
                cin >> studentId;
                cin.ignore();
                if (admin1.findStudent(studentId)) {
                    cout << "Enter Course ID: ";
                    cin >> courseId;
                    cout << "Enter Grade (Percentage): ";
                    cin >> grade;
                    admin1.findStudent(studentId)->inputGrade(courseId, grade);
                }
                else {
                    cout << "Student with ID " << studentId << " not found." << endl;
                }
                break;

            case 5:
                cout << "Enter Student ID to edit details: ";
                cin >> studentId;
                cin.ignore();
                if (admin1.findStudent(studentId)) {
                    cout << "Enter new Student Name(First Name, Last Name): ";
                    getline(cin, studentName);
                    admin1.findStudent(studentId)->editUserInfo(studentName);
                }
                else {
                    cout << "Student with ID " << studentId << " not found." << endl;
                }
                break;

            case 6:
                cout << "Enter Course ID to edit details: ";
                cin >> courseId;
                cin.ignore();
                if (admin1.getCourseInfo(courseId).first != "") {
                    cout << "Enter new Course Name: ";
                    getline(cin, newCourseName);
                    cout << "Enter new Credit Hours: ";
                    cin >> newCreditHours;
                    admin1.editCourseDetails(courseId, newCourseName, newCreditHours);
                }
                else {
                    cout << "Course with ID " << courseId << " not found." << endl;
                }
                break;

            case 7:
                cout << "Enter Student ID: ";
                cin >> studentId;
                {
                    Student* foundStudent = admin1.findStudent(studentId);
                    if (foundStudent) {
                        cout << "Student found:" << endl;
                        foundStudent->displayInfo();
                        foundStudent->displayCourses();
                        cout << "GPA: " << foundStudent->calculateGPA() << endl;
                    }
                    else {
                        cout << "Student with ID " << studentId << " not found." << endl;
                    }
                }
                break;

            case 8:
                cout << "Enter Course ID: ";
                cin >> courseId;
                admin1.displayEnrolledStudents(courseId);
                break;

            case 9:
                admin1.displayStudents();
                break;

            case 10:
                admin1.rankStudents();
                break;

            case 11:
                cout << "Enter Student ID to remove: ";
                cin >> studentId;
                admin1.removeStudent(studentId);
                break;


            case 12:
                cout << "Enter Course ID to remove: ";
                cin >> courseId;
                admin1.removeCourse(courseId);
                break;


            case 13:
                cout << "Enter Student ID: ";
                cin >> studentId;
                cin.ignore();
                cout << "Enter Course ID to remove: ";
                getline(cin, courseId);
                admin1.removeStudentFromCourse(studentId, courseId);
                break;

            case 14:
                admin1.removeAllStudents();
                break;

            case 15:
                admin1.removeAllCourses();
                break;

            case 16:
                cout << "Exiting..." << endl;
                break;

            default:
                cout << "Invalid choice! please try again" << endl;
                break;
        }

    } while (choice != 16);

    admin1.writeDataToFile("C:\\Users\\awaad\\OneDrive\\Documents\\PROJECT\\project.txt", admin1);

    return 0;
}
