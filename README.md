# Student Record Management System (C++)

## About the Project

I built this Student Record Management System in C++ as a menu-driven console application. It lets me add, update, delete and display student information, and it stores everything in a text file so the records are still there the next time I run the program.

My goal was to build a small but complete application that uses file handling for persistent storage, so that record management stays reliable and nothing is lost when the program closes.

## What I Implemented

- **Add student:** I take the roll number, name, age, course and marks, and reject duplicate roll numbers.
- **Update student:** I can change any field of an existing record. Pressing Enter keeps the old value.
- **Delete student:** I remove a record by roll number after asking for confirmation.
- **Display students:** I show all records in a neat table, sorted by roll number.
- **Search student:** I look up a single record by roll number.
- **Persistent storage:** I load the records from `students.txt` when the program starts and save them after every add, update or delete.

## Concepts I Used

- File handling with `ifstream` and `ofstream`
- A `struct` for the student record and a `vector` to hold the records in memory
- Separate functions for each operation to keep the code organised
- A menu loop for smooth navigation
- Input validation for numbers, empty fields and duplicate roll numbers
- Formatted output with `iomanip`

## How I Store the Data

Each student is saved on one line in `students.txt`, with the fields separated by `|`:

```
roll|name|age|course|marks
102|Rahul|22|BCA|95
```

The file is created automatically the first time a record is saved. I remove the `|` character from names and courses so it cannot break the file format.

## Validation Rules I Added

- Roll numbers must be unique positive integers
- Age must be between 1 and 120
- Marks must be between 0 and 100
- Name and course cannot be empty

## How to Run

```
g++ -std=c++17 student_manager.cpp -o student_manager
./student_manager
```

On Windows (MinGW), I run `student_manager.exe` instead.

## Menu

```
===== Student Management System =====
1. Add student
2. Update student
3. Delete student
4. Display all students
5. Search by roll number
6. Exit
```

## Result

The final application lets me add, update, delete and display student information, and the data stays saved between runs, so the records are reliable.

## Future Improvements

- Search by name
- Sort by marks or name
- Export records to CSV
- Store multiple subjects and grades for each student
