Student Record Management System (SRMS)

A complete C++ console-based Student Record Management System that supports Admin, Student, and Parent login roles.
All data is stored in plain text files, exactly as shown in the project folder.

# Files Used in This Project

These filenames match exactly the names seen in your images:

File Name - Description
1.admin_login Stores admin username and password
2.students Stores all student records: roll, names, mobile, marks, complaint
3.student_login - Stores student login credentials
4.parent_login - Stores parent login credentials
5.complaints - Auto-generated file containing roll–complaint entries
6.srms.cpp - Main C++ source code
7.srms.exe / srms (Application) - Compiled executable

Example File Contents

admin_login
admin 12345a

# parent_login

1|SureshKumar#001
2|PoojaSharma#002
3|MaheshSingh#003
4|LakshmiNair#004
5|RaviReddy#005

# student_login

1|RohanKumar@001
2|AnitaSharma@002
3|VikramSingh@003
4|MeeraNair@004
5|ArjunReddy@005

# students

1|Rohan Kumar|Suresh Kumar|9876543210|85,78,92,88,91|
2|Anita Sharma|Pooja Sharma|9876501234|76,81,69,74,80|
3|Vikram Singh|Mahesh Singh|9123456780|90,88,85,87,89|
4|Meera Nair|Lakshmi Nair|9988776655|67,72,70,75,73|
5|Arjun Reddy|Ravi Reddy|9876123450|95,94,96,98,97|

# Password Generation Logic

When a student is added:

✔ Student Password
<nameWithoutSpaces>@<last3DigitsOfRoll>

✔ Parent Password
<parentNameWithoutSpaces>#<last3DigitsOfRoll>

# Admin Features

Add new student

View all students

Delete a student (automatically cleans login files)

Update marks for any subject

View complaints

Solve/remove complaints

Change admin password

# Student Features

Login with roll number + password

View their marks and personal details

Raise a complaint

Change their password

# Parent Features

Login using child’s roll number + parent password

View student’s marks

Change parent password

# Data Formats

File: students
roll|studentName|parentName|mobile|m1,m2,m3,m4,m5|complaint

File: student_login
roll|password

File: parent_login
roll|password

File: admin_login
username password

File: complaints

Generated from students who submitted a complaint:

roll|complaint

# How to Compile and Run

1. Compile
   g++ srms.cpp -o srms

2. Run

Windows:

srms.exe

Linux/macOS:

./srms

Ensure the folder has permissions to read/write all text files.

# Program Flow

Main Menu

1. Admin Login
2. Student Login
3. Parent Login
4. Exit

Admin Menu

1. Add Student
2. View Students
3. Delete Student
4. Update Marks
5. View Complaints
6. Solve Complaint
7. Change Admin Password
8. Logout

Student Menu

1. View My Details
2. Raise Complaint
3. Change Password
4. Logout

Parent Menu

1. View Child Details
2. Change Password
3. Logout

🛠️ Key Functional Blocks in Code
Function Purpose
addStudent() - Adds a new student and generates passwords
viewStudents() - Displays all current records
deleteStudent() - Removes student + login records
updateMarks() - Updates all or single subject marks
viewComplaints() - Shows all submitted complaints
solveComplaint() - Removes complaint for a roll number
studentView() - Student profile view
parentView() - Parent/student marks view
changePasswordInFile() - Changes student/parent password
changeAdminPassword() - Changes admin password

# Initial Setup

Create a file named admin_login:

admin 12345a

Place empty files:

students
student_login
parent_login
complaints

The system will fill them as you add students.
