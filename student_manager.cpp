// Student Record Management System (C++)
// Menu-driven console app: add, update, delete, display, search.
// Records are stored persistently in students.txt (one record per line, '|' separated).

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const string DATA_FILE = "students.txt";

struct Student {
    int roll;
    string name;
    int age;
    string course;
    double marks;
};

// ---------- File handling ----------
vector<Student> loadStudents() {
    vector<Student> students;
    ifstream in(DATA_FILE);
    if (!in) return students;  // first run: no file yet

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string roll, name, age, course, marks;
        if (getline(ss, roll, '|') && getline(ss, name, '|') && getline(ss, age, '|') &&
            getline(ss, course, '|') && getline(ss, marks, '|')) {
            try {
                students.push_back({stoi(roll), name, stoi(age), course, stod(marks)});
            } catch (...) {
                // skip corrupted line
            }
        }
    }
    return students;
}

void saveStudents(const vector<Student>& students) {
    ofstream out(DATA_FILE, ios::trunc);
    if (!out) {
        cout << "Error: could not write to " << DATA_FILE << "\n";
        return;
    }
    for (const auto& s : students)
        out << s.roll << '|' << s.name << '|' << s.age << '|' << s.course << '|' << s.marks << '\n';
}

// ---------- Input helpers ----------
string readLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

string readRequired(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        // strip '|' so it cannot break the file format
        s.erase(remove(s.begin(), s.end(), '|'), s.end());
        if (!s.empty()) return s;
        cout << "This field cannot be empty.\n";
    }
}

bool parseInt(const string& s, int& out) {
    try {
        size_t pos;
        out = stoi(s, &pos);
        return pos == s.size();
    } catch (...) {
        return false;
    }
}

bool parseDouble(const string& s, double& out) {
    try {
        size_t pos;
        out = stod(s, &pos);
        return pos == s.size();
    } catch (...) {
        return false;
    }
}

// If allowBlank is true, an empty input returns false (meaning "keep current").
bool readInt(const string& prompt, int lo, int hi, int& out, bool allowBlank = false) {
    while (true) {
        string s = readLine(prompt);
        if (s.empty() && allowBlank) return false;
        if (parseInt(s, out) && out >= lo && out <= hi) return true;
        cout << "Enter a whole number between " << lo << " and " << hi << ".\n";
    }
}

bool readDouble(const string& prompt, double lo, double hi, double& out, bool allowBlank = false) {
    while (true) {
        string s = readLine(prompt);
        if (s.empty() && allowBlank) return false;
        if (parseDouble(s, out) && out >= lo && out <= hi) return true;
        cout << "Enter a number between " << lo << " and " << hi << ".\n";
    }
}

int findIndex(const vector<Student>& students, int roll) {
    for (size_t i = 0; i < students.size(); i++)
        if (students[i].roll == roll) return static_cast<int>(i);
    return -1;
}

// ---------- Operations ----------
void addStudent(vector<Student>& students) {
    cout << "\n--- Add Student ---\n";
    int roll;
    readInt("Roll number: ", 1, 999999999, roll);
    if (findIndex(students, roll) != -1) {
        cout << "A student with roll number " << roll << " already exists.\n";
        return;
    }
    Student s;
    s.roll = roll;
    s.name = readRequired("Name: ");
    readInt("Age: ", 1, 120, s.age);
    s.course = readRequired("Course: ");
    readDouble("Marks (0-100): ", 0, 100, s.marks);

    students.push_back(s);
    saveStudents(students);
    cout << "Student added successfully.\n";
}

void updateStudent(vector<Student>& students) {
    cout << "\n--- Update Student ---\n";
    int roll;
    readInt("Roll number of student to update: ", 1, 999999999, roll);
    int idx = findIndex(students, roll);
    if (idx == -1) {
        cout << "No student found with that roll number.\n";
        return;
    }
    Student& s = students[idx];
    cout << "Press Enter to keep the current value.\n";

    string name = readLine("Name [" + s.name + "]: ");
    name.erase(remove(name.begin(), name.end(), '|'), name.end());
    if (!name.empty()) s.name = name;

    int age;
    if (readInt("Age [" + to_string(s.age) + "]: ", 1, 120, age, true)) s.age = age;

    string course = readLine("Course [" + s.course + "]: ");
    course.erase(remove(course.begin(), course.end(), '|'), course.end());
    if (!course.empty()) s.course = course;

    double marks;
    ostringstream cur;
    cur << s.marks;
    if (readDouble("Marks [" + cur.str() + "]: ", 0, 100, marks, true)) s.marks = marks;

    saveStudents(students);
    cout << "Student updated successfully.\n";
}

void deleteStudent(vector<Student>& students) {
    cout << "\n--- Delete Student ---\n";
    int roll;
    readInt("Roll number of student to delete: ", 1, 999999999, roll);
    int idx = findIndex(students, roll);
    if (idx == -1) {
        cout << "No student found with that roll number.\n";
        return;
    }
    string confirm = readLine("Delete " + students[idx].name + " (Roll " + to_string(roll) + ")? (y/n): ");
    if (confirm == "y" || confirm == "Y") {
        students.erase(students.begin() + idx);
        saveStudents(students);
        cout << "Student deleted successfully.\n";
    } else {
        cout << "Deletion cancelled.\n";
    }
}

void displayStudents(const vector<Student>& students) {
    cout << "\n--- All Students ---\n";
    if (students.empty()) {
        cout << "No records found.\n";
        return;
    }
    vector<Student> sorted = students;
    sort(sorted.begin(), sorted.end(), [](const Student& a, const Student& b) { return a.roll < b.roll; });

    cout << left << setw(10) << "Roll" << setw(22) << "Name" << setw(6) << "Age" << setw(16) << "Course"
         << setw(8) << "Marks" << "\n";
    cout << string(62, '-') << "\n";
    for (const auto& s : sorted) {
        cout << left << setw(10) << s.roll << setw(22) << s.name << setw(6) << s.age << setw(16) << s.course
             << setw(8) << fixed << setprecision(1) << s.marks << "\n";
    }
    cout << "\nTotal students: " << sorted.size() << "\n";
}

void searchStudent(const vector<Student>& students) {
    cout << "\n--- Search Student ---\n";
    int roll;
    readInt("Roll number: ", 1, 999999999, roll);
    int idx = findIndex(students, roll);
    if (idx == -1) {
        cout << "No student found with that roll number.\n";
        return;
    }
    const Student& s = students[idx];
    cout << "Roll:   " << s.roll << "\nName:   " << s.name << "\nAge:    " << s.age << "\nCourse: " << s.course
         << "\nMarks:  " << fixed << setprecision(1) << s.marks << "\n";
}

// ---------- Menu ----------
int main() {
    vector<Student> students = loadStudents();

    while (true) {
        cout << "\n===== Student Management System =====\n"
             << "1. Add student\n"
             << "2. Update student\n"
             << "3. Delete student\n"
             << "4. Display all students\n"
             << "5. Search by roll number\n"
             << "6. Exit\n";
        string choice = readLine("Enter your choice (1-6): ");

        if (cin.eof()) break;
        if (choice == "1") addStudent(students);
        else if (choice == "2") updateStudent(students);
        else if (choice == "3") deleteStudent(students);
        else if (choice == "4") displayStudents(students);
        else if (choice == "5") searchStudent(students);
        else if (choice == "6") {
            cout << "Goodbye!\n";
            break;
        } else cout << "Invalid choice. Please enter a number from 1 to 6.\n";
    }
    return 0;
}
