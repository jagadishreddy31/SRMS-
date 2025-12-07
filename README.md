# 📘 Student Record Management System (SRMS)

A simple **C++ console-based application** for managing student records
efficiently.\
This project demonstrates **file handling, authentication, and CRUD
operations** using C++.

## 🚀 Features

### 👨‍🎓 Student Module

-   View student profile\
-   Submit complaints\
-   Check academic details

### 👨‍🏫 Admin Module

-   Add new student records\
-   Delete student records\
-   Update student information\
-   View and manage complaints\
-   Manage login credentials

### 👨‍👩‍👦 Parent Module

-   Parent login system\
-   View student progress

## 🗂️ Project Structure

    srms/
    │── srms.cpp               # Main source code
    │── srms.exe               # Compiled executable (Windows)
    │── students.txt           # Student records
    │── complaints.txt         # Complaints
    │── admin_login.txt        # Admin credentials
    │── student_login.txt      # Student login details
    │── parent_login.txt       # Parent login details

## 🛠️ Technologies Used

-   **C++**
-   **File Handling (fstream)**
-   **Object-Oriented Programming**
-   **Console-Based UI**

## ▶️ How to Run

### Option 1: Run Executable (Windows)

1.  Double-click **srms.exe**
2.  Follow on-screen menu instructions

### Option 2: Compile Manually

``` bash
g++ srms.cpp -o srms
./srms
```

## 🔐 Login System

### Admin Login

Stored in `admin_login.txt`

### Student Login

Stored in `student_login.txt`

### Parent Login

Stored in `parent_login.txt`

## 📄 Data Storage

-   `students.txt` -- stores student info\
-   `complaints.txt` -- stores student complaints

## 📈 Future Enhancements

-   GUI using Qt/GTK or a web interface\
-   Database integration\
-   Email/OTP login\
-   Attendance & grading system

## 🤝 Contributing

Pull requests are welcome!

## 📜 License

MIT License
