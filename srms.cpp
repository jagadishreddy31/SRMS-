// srms.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

/*
  SRMS - Single-file final program
  - Subjects: Maths, Physics, Chemistry, English, Computer
  - Files used:
    students.txt       (roll|name|parentName|parentMobile|m1,m2,m3,m4,m5|complaint)
    student_login.txt  (roll|password)
    parent_login.txt   (roll|password)
    admin_login.txt    (username password)
    complaints.txt     (roll|complaint)
*/

const vector<string> SUBJECTS = {"Maths", "Physics", "Chemistry", "English", "Computer"};
const int NUM_SUBJECTS = 5;

struct Student {
    int roll = 0;
    string name;
    string parentName;
    string parentMobile;
    float marks[NUM_SUBJECTS];
    string complaint;
};

// in-memory students
vector<Student> students;

// -------------------- Helpers --------------------
vector<string> split(const string &s, char delim) {
    vector<string> parts;
    string cur;
    for (char c : s) {
        if (c == delim) {
            parts.push_back(cur);
            cur.clear();
        } else cur.push_back(c);
    }
    parts.push_back(cur);
    return parts;
}

string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

bool isDigits(const string &s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit((unsigned char)c)) return false;
    return true;
}

bool isAlphaOrSpace(const string &s) {
    if (s.empty()) return false;
    for (char c : s) if (!(isalpha((unsigned char)c) || c == ' ')) return false;
    return true;
}

string removeSpaces(const string &s) {
    string out;
    for (char c : s) if (c != ' ') out.push_back(c);
    return out;
}

string pad3(int x) {
    int last3 = x % 1000;
    if (last3 < 10) return "00" + to_string(last3);
    if (last3 < 100) return "0" + to_string(last3);
    return to_string(last3);
}

string generatePassword(const string &name, int roll, char type) {
    string cleaned = removeSpaces(name);
    string special = (type == 'S') ? "@" : "#";
    return cleaned + special + pad3(roll);
}

// safe stoi and stof with checks used where needed

// -------------------- File IO --------------------
void saveStudentsToFiles() {
    // students.txt
    ofstream sOut("students.txt");
    for (auto &st : students) {
        sOut << st.roll << "|" << st.name << "|" << st.parentName << "|" << st.parentMobile << "|";
        // marks as comma separated
        for (int i = 0; i < NUM_SUBJECTS; ++i) {
            sOut << st.marks[i];
            if (i + 1 < NUM_SUBJECTS) sOut << ",";
        }
        sOut << "|" << st.complaint << "\n";
    }
    sOut.close();

    // complaints.txt (rebuild from students complaint field)
    ofstream cOut("complaints.txt");
    for (auto &st : students) {
        if (!trim(st.complaint).empty()) {
            cOut << st.roll << "|" << st.complaint << "\n";
        }
    }
    cOut.close();
}

void loadStudentsFromFiles() {
    students.clear();
    ifstream sIn("students.txt");
    if (!sIn) return; // no file yet
    string line;
    while (getline(sIn, line)) {
        line = trim(line);
        if (line.empty()) continue;
        vector<string> parts = split(line, '|');
        if (parts.size() != 6) continue; // malformed
        string rollStr = trim(parts[0]);
        if (!isDigits(rollStr)) continue;
        Student st;
        st.roll = stoi(rollStr);
        st.name = trim(parts[1]);
        st.parentName = trim(parts[2]);
        st.parentMobile = trim(parts[3]);
        // parse marks
        vector<string> mparts = split(parts[4], ',');
        if (mparts.size() != NUM_SUBJECTS) {
            // if missing, set to 0
            for (int i = 0; i < NUM_SUBJECTS; ++i) st.marks[i] = 0;
        } else {
            for (int i = 0; i < NUM_SUBJECTS; ++i) {
                string ms = trim(mparts[i]);
                try {
                    st.marks[i] = stof(ms);
                } catch (...) { st.marks[i] = 0; }
            }
        }
        st.complaint = parts[5];
        students.push_back(st);
    }
    sIn.close();
}

// -------------------- Login helpers --------------------
bool readAdminCredentials(string &userOut, string &passOut) {
    ifstream f("admin_login.txt");
    if (!f) return false;
    string u, p;
    if (!(f >> u >> p)) return false;
    userOut = u; passOut = p;
    return true;
}

bool adminLogin() {
    string u, p;
    cout << "\nEnter Admin Username: ";
    cin >> u;
    cout << "Enter Password: ";
    cin >> p;
    string user, pass;
    if (!readAdminCredentials(user, pass)) {
        cout << "⚠ admin_login.txt missing or malformed.\n";
        return false;
    }
    return (u == user && p == pass);
}

bool checkLoginFileFor(const string &filename, int roll, const string &password) {
    ifstream f(filename);
    if (!f) return false;
    string line;
    while (getline(f, line)) {
        line = trim(line);
        if (line.empty()) continue;
        vector<string> p = split(line, '|');
        if (p.size() != 2) continue;
        if (!isDigits(trim(p[0]))) continue;
        int r = stoi(trim(p[0]));
        string pw = trim(p[1]);
        if (r == roll && pw == password) return true;
    }
    return false;
}

bool studentLogin(int &rollOut) {
    string rollStr, pass;
    cout << "Enter Roll Number: ";
    cin >> rollStr;
    while (!isDigits(rollStr)) {
        cout << "❌ Invalid roll! Enter numbers only: ";
        cin >> rollStr;
    }
    int roll = stoi(rollStr);
    cout << "Enter Password: ";
    cin >> pass;
    if (checkLoginFileFor("student_login.txt", roll, pass)) {
        rollOut = roll; return true;
    }
    return false;
}

bool parentLogin(int &rollOut) {
    string rollStr, pass;
    cout << "Enter Student Roll Number: ";
    cin >> rollStr;
    while (!isDigits(rollStr)) {
        cout << "❌ Invalid roll! Enter numbers only: ";
        cin >> rollStr;
    }
    int roll = stoi(rollStr);
    cout << "Enter Parent Password: ";
    cin >> pass;
    if (checkLoginFileFor("parent_login.txt", roll, pass)) {
        rollOut = roll; return true;
    }
    return false;
}

// -------------------- Password change --------------------
// For student/parent login files: format roll|password
bool changePasswordInFile(const string &filename, int roll) {
    ifstream f(filename);
    if (!f) { cout << "⚠ File missing: " << filename << "\n"; return false; }
    vector<pair<int,string>> entries;
    string line;
    bool found = false;
    while (getline(f,line)) {
        line = trim(line);
        if (line.empty()) continue;
        vector<string> p = split(line,'|');
        if (p.size()!=2) continue;
        string rstr = trim(p[0]); string pw = trim(p[1]);
        if (!isDigits(rstr)) continue;
        int r = stoi(rstr);
        entries.push_back({r,pw});
    }
    f.close();

    int oldRoll = roll;
    string oldPass; cout << "Enter old password: "; cin >> oldPass;
    // verify
    bool ok = false;
    for (auto &e : entries) if (e.first==oldRoll && e.second==oldPass) { ok = true; break; }
    if (!ok) { cout<<"❌ Old password incorrect!\n"; return false; }

    string newPass; cout << "Enter new password (min 6 chars): "; cin >> newPass;
    while (newPass.length() < 6) { cout << "❌ Too short! Enter again: "; cin >> newPass; }
    for (auto &e : entries) if (e.first==oldRoll) e.second = newPass;

    ofstream out(filename);
    for (auto &e : entries) out << e.first << "|" << e.second << "\n";
    out.close();
    cout << "✔ Password changed successfully!\n";
    return true;
}

// Admin password change (admin_login.txt format: username password)
bool changeAdminPassword() {
    string user, pass;
    if (!readAdminCredentials(user, pass)) { cout << "⚠ admin_login.txt missing or malformed.\n"; return false; }
    string oldP; cout << "Enter old admin password: "; cin >> oldP;
    if (oldP != pass) { cout << "❌ Old password incorrect!\n"; return false; }
    string newP; cout << "Enter new admin password (min 6 chars): "; cin >> newP;
    while (newP.length() < 6) { cout << "❌ Too short! Enter again: "; cin >> newP; }
    ofstream out("admin_login.txt");
    out << user << " " << newP << "\n";
    out.close();
    cout << "✔ Admin password changed successfully!\n";
    return true;
}

// -------------------- Student management --------------------
bool rollExists(int roll) {
    for (auto &s : students) if (s.roll == roll) return true;
    return false;
}

void appendLogin(const string &filename, int roll, const string &password) {
    ofstream f(filename, ios::app);
    f << roll << "|" << password << "\n";
    f.close();
}

void addStudent() {
    Student st;
    string rollStr;
    cout << "\nEnter Roll Number (numbers only): ";
    cin >> rollStr;
    while (!isDigits(rollStr)) { cout << "❌ Invalid! Roll must contain numbers only. Re-enter: "; cin >> rollStr; }
    st.roll = stoi(rollStr);
    if (rollExists(st.roll)) { cout << "❌ Roll already exists. Aborting add.\n"; return; }
    cin.ignore();

    cout << "Enter Student Full Name: ";
    getline(cin, st.name);
    while (!isAlphaOrSpace(st.name)) { cout << "❌ Only alphabets and spaces allowed. Re-enter name: "; getline(cin, st.name); }

    cout << "Enter Parent Full Name: ";
    getline(cin, st.parentName);
    while (!isAlphaOrSpace(st.parentName)) { cout << "❌ Only alphabets and spaces allowed. Re-enter parent name: "; getline(cin, st.parentName); }

    cout << "Enter Parent Mobile (digits only, e.g. 10 digits): ";
    string mob; cin >> mob;
    while (!isDigits(mob) || mob.length() < 7 || mob.length() > 15) {
        cout << "❌ Invalid mobile. Enter digits only (7-15 digits): "; cin >> mob;
    }
    st.parentMobile = mob;

    cout << "Enter marks for subjects (0-100):\n";
    for (int i = 0; i < NUM_SUBJECTS; ++i) {
        float m; cout << SUBJECTS[i] << ": "; cin >> m;
        while (m < 0 || m > 100) { cout << "❌ Invalid marks! Enter again (0-100): "; cin >> m; }
        st.marks[i] = m;
    }
    cin.ignore();
    st.complaint = ""; // initially none

    students.push_back(st);
    saveStudentsToFiles();

    string sPass = generatePassword(st.name, st.roll, 'S');
    string pPass = generatePassword(st.parentName, st.roll, 'P');
    appendLogin("student_login.txt", st.roll, sPass);
    appendLogin("parent_login.txt", st.roll, pPass);

    cout << "\n✔ Student added successfully!\n";
    cout << "Student Password: " << sPass << "\n";
    cout << "Parent  Password: " << pPass << "\n";
}

void printStudentSummary(const Student &st) {
    cout << "Roll: " << st.roll << "\n";
    cout << "Name: " << st.name << "\n";
    cout << "Parent: " << st.parentName << "\n";
    cout << "Parent Mobile: " << (st.parentMobile.empty() ? "N/A" : st.parentMobile) << "\n";
    cout << "Marks:\n";
    for (int i = 0; i < NUM_SUBJECTS; ++i) {
        cout << "  " << SUBJECTS[i] << " : " << st.marks[i] << "\n";
    }
    cout << "Complaint: " << (trim(st.complaint).empty() ? "None" : st.complaint) << "\n";
}

void viewStudents() {
    if (students.empty()) { cout << "No students present.\n"; return; }
    cout << "\n---- STUDENT LIST ----\n";
    for (auto &st : students) {
        cout << st.roll << " | " << st.name << " | " << st.parentName << " | " << st.parentMobile << " | ";
        cout << "Marks:";
        for (int i=0;i<NUM_SUBJECTS;i++) { cout << (i==0? " " : ",") << st.marks[i]; }
        cout << " | Complaint: " << (trim(st.complaint).empty() ? "None" : st.complaint) << "\n";
    }
}

void deleteStudent() {
    string sRoll; cout << "Enter roll number to delete: "; cin >> sRoll;
    if (!isDigits(sRoll)) { cout << "❌ Invalid roll input.\n"; return; }
    int r = stoi(sRoll);
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->roll == r) {
            students.erase(it);
            saveStudentsToFiles();
            // remove from student_login.txt and parent_login.txt and complaints.txt
            auto removeFromFile = [&](const string &fname){
                ifstream f(fname);
                if (!f) return;
                vector<string> lines; string L;
                while (getline(f,L)) {
                    if (trim(L).empty()) continue;
                    vector<string> p = split(L,'|');
                    if (p.size()<2) continue;
                    if (isDigits(trim(p[0])) && stoi(trim(p[0]))==r) continue;
                    lines.push_back(L);
                }
                f.close();
                ofstream out(fname);
                for (auto &x: lines) out<<x<<"\n";
                out.close();
            };
            removeFromFile("student_login.txt");
            removeFromFile("parent_login.txt");
            // complaints file rebuilt by saveStudentsToFiles()
            cout << "✔ Student deleted and related data removed.\n";
            return;
        }
    }
    cout << "❌ Student not found.\n";
}

void updateMarks() {
    string sRoll; cout << "Enter roll number: "; cin >> sRoll;
    if (!isDigits(sRoll)) { cout << "❌ Invalid roll input.\n"; return; }
    int r = stoi(sRoll);
    for (auto &st : students) {
        if (st.roll == r) {
            cout << "Update marks options:\n1. Update all subjects\n2. Update single subject\nChoose: ";
            int ch; cin >> ch;
            if (ch == 1) {
                for (int i = 0; i < NUM_SUBJECTS; ++i) {
                    float m; cout << SUBJECTS[i] << ": "; cin >> m;
                    while (m < 0 || m > 100) { cout << "❌ Invalid marks! Enter again (0-100): "; cin >> m; }
                    st.marks[i] = m;
                }
            } else {
                for (int i = 0; i < NUM_SUBJECTS; ++i) cout << i+1 << ". " << SUBJECTS[i] << "\n";
                cout << "Choose subject number: "; int si; cin >> si;
                if (si < 1 || si > NUM_SUBJECTS) { cout << "❌ Invalid choice.\n"; return; }
                float m; cout << "Enter marks for " << SUBJECTS[si-1] << ": "; cin >> m;
                while (m < 0 || m > 100) { cout << "❌ Invalid marks! Enter again (0-100): "; cin >> m; }
                st.marks[si-1] = m;
            }
            saveStudentsToFiles();
            cout << "✔ Marks updated.\n";
            return;
        }
    }
    cout << "❌ Student not found.\n";
}

// -------------------- Complaints --------------------
void viewComplaints() {
    bool any = false;
    cout << "\n--- Complaints ---\n";
    for (auto &st : students) {
        if (!trim(st.complaint).empty()) {
            cout << st.roll << " | " << st.complaint << "\n";
            any = true;
        }
    }
    if (!any) cout << "✔ No complaints found.\n";
}

void solveComplaint() {
    // remove by roll from complaints (and student record)
    bool any = false; for (auto &st : students) if (!trim(st.complaint).empty()) { any = true; break; }
    if (!any) { cout << "✔ No complaints to solve.\n"; return; }
    string sRoll; cout << "Enter roll number to mark complaint solved/remove: "; cin >> sRoll;
    if (!isDigits(sRoll)) { cout << "❌ Invalid roll input.\n"; return; }
    int r = stoi(sRoll);
    for (auto &st : students) {
        if (st.roll == r) {
            if (trim(st.complaint).empty()) { cout << "❌ No complaint found for this roll.\n"; return; }
            st.complaint = "";
            saveStudentsToFiles();
            cout << "✔ Complaint removed for roll " << r << ".\n";
            return;
        }
    }
    cout << "❌ Student not found.\n";
}

// -------------------- Student / Parent actions --------------------
void studentView(int roll) {
    for (auto &st : students) {
        if (st.roll == roll) {
            cout << "\n=============================\n";
            cout << "       🧑‍🎓 STUDENT PORTAL\n";
            cout << "=============================\n";
            printStudentSummary(st);
            return;
        }
    }
    cout << "❌ Student record not found.\n";
}

void parentView(int roll) {
    for (auto &st : students) {
        if (st.roll == roll) {
            cout << "\n=============================\n";
            cout << "        PARENT VIEW\n";
            cout << "=============================\n";
            printStudentSummary(st);
            return;
        }
    }
    cout << "❌ Student record not found.\n";
}

void raiseComplaintWithConfirm(int roll) {
    cin.ignore();
    cout << "Enter your complaint (single line):\n";
    string complaintText;
    getline(cin, complaintText);
    complaintText = trim(complaintText);
    if (complaintText.empty()) { cout << "❌ Complaint cannot be empty.\n"; return; }

    cout << "\nYou entered:\n\"" << complaintText << "\"\n";
    cout << "Submit complaint? (Y/N): ";
    char ch; cin >> ch;
    if (ch == 'Y' || ch == 'y') {
        for (auto &st : students) {
            if (st.roll == roll) {
                st.complaint = complaintText;
                saveStudentsToFiles();
                cout << "✔ Complaint submitted.\n";
                return;
            }
        }
        cout << "❌ Student not found.\n";
    } else {
        cout << "Complaint cancelled.\n";
    }
}

// -------------------- Main menu & flow --------------------
void loadAll() {
    loadStudentsFromFiles();
    // Note: student_login.txt and parent_login.txt are handled on demand
}

void mainMenu() {
    loadAll();
    while (true) {
        cout << "\n============================\n";
        cout << "     STUDENT RECORD SYSTEM\n";
        cout << "============================\n";
        cout << "1. Admin Login\n2. Student Login\n3. Parent Login\n4. Exit\n";
        cout << "Enter choice: ";
        int c; if (!(cin >> c)) { cin.clear(); string dummy; getline(cin,dummy); continue; }

        if (c == 1) {
            if (!adminLogin()) { cout << "❌ Wrong admin credentials!\n"; continue; }
            // admin menu
            while (true) {
                cout << "\n--- ADMIN MENU ---\n";
                cout << "1. Add Student\n2. View Students\n3. Delete Student\n4. Update Marks\n5. View Complaints\n6. Solve Complaint\n7. Change Admin Password\n8. Logout\nEnter choice: ";
                int a; cin >> a;
                if (a == 1) addStudent();
                else if (a == 2) viewStudents();
                else if (a == 3) deleteStudent();
                else if (a == 4) updateMarks();
                else if (a == 5) viewComplaints();
                else if (a == 6) solveComplaint();
                else if (a == 7) changeAdminPassword();
                else break;
            }
        }
        else if (c == 2) {
            int roll;
            if (!studentLogin(roll)) { cout << "❌ Invalid student credentials!\n"; continue; }
            while (true) {
                cout << "\n--- STUDENT MENU ---\n";
                cout << "1. View My Details\n2. Raise Complaint\n3. Change Password\n4. Logout\nChoice: ";
                int s; cin >> s;
                if (s == 1) studentView(roll);
                else if (s == 2) raiseComplaintWithConfirm(roll);
                else if (s == 3) changePasswordInFile("student_login.txt", roll);
                else break;
            }
        }
        else if (c == 3) {
            int roll;
            if (!parentLogin(roll)) { cout << "❌ Invalid parent credentials!\n"; continue; }
            while (true) {
                cout << "\n--- PARENT MENU ---\n";
                cout << "1. View Child Details\n2. Change Password\n3. Logout\nChoice: ";
                int p; cin >> p;
                if (p == 1) parentView(roll);
                else if (p == 2) changePasswordInFile("parent_login.txt", roll);
                else break;
            }
        }
        else if (c == 4) {
            cout << "Goodbye!\n";
            break;
        }
        
    }
}

int main() {
    mainMenu();
    return 0;
}
