@echo off
echo ========================================
echo   COMPILING...
echo ========================================

g++ -std=c++11 -o library.exe Main.cpp LibrarySystem.cpp Book.cpp Member.cpp Transaction.cpp

if %ERRORLEVEL% EQU 0 (
    echo [OK] Compilation SUCCESS!
    echo.
    echo ========================================
    echo   RUNNING PROGRAM...
    echo ========================================
    echo.
    library.exe
) else (
    echo [ERROR] Compilation FAILED!
    pause
)