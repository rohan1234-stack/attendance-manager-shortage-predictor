# Attendance Manager - Shortage Predictor

This DA2 project is a menu-driven C++ mini system built with OOP concepts, STL, and file persistence.

## Features

- add students with duplicate register number validation
- mark attendance per date using `P` or `A`
- view attendance summary for a student
- generate attendance percentage report
- list shortage students below `75%`
- calculate class average attendance percentage
- save data using `fstream`

## Build locally

```powershell
g++ main.cpp -std=c++17 -o attendance_manager.exe
.\attendance_manager.exe
```

## Files used

- `students.txt`
- `attendance.txt`

## Notes

- `em++` is required later for WebAssembly hosting
- recommended deployment command:

```powershell
em++ main.cpp -o index.html
```
