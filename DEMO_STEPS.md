# Attendance Manager Demo Steps

## Run locally

```powershell
cd C:\Users\lenovo\Documents\Playground\da2-attendance-manager
.\run_attendance_manager.ps1
```

## Good demo flow

1. Add student `25BCE1744` with name `Rohan Konduru`
2. Add student `25BCE1745` with name `Anika Sharma`
3. Mark attendance for date `2026-04-03`
4. Enter `P` for the first student and `A` for the second student
5. Open attendance percentage report
6. Open shortage report
7. Open class average
8. View the summary for `25BCE1744`

## Quick automated demo

```powershell
Get-Content .\demo_input.txt | .\attendance_manager.exe
```
