$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$exePath = Join-Path $projectRoot "attendance_manager.exe"
$sourcePath = Join-Path $projectRoot "main.cpp"

Write-Host "Compiling Attendance Manager..."
g++ $sourcePath -std=c++17 -o $exePath

Write-Host "Starting Attendance Manager..."
& $exePath
