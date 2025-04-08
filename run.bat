@echo off
chcp 65001>nul
setlocal enabledelayedexpansion
:: Tạo biến chứa escape character
for /f %%a in ('echo prompt $E ^| cmd') do set "ESC=%%a"
cls

REM Print header with colors
echo %ESC%[96m╔══════════════════════════════════════════╗%ESC%[0m
echo %ESC%[96m║     🚗 Car Simulator - Windows Setup     ║%ESC%[0m
echo %ESC%[96m╚══════════════════════════════════════════╝%ESC%[0m
echo.

echo %ESC%[92m📡 Checking system requirements...%ESC%[0m
timeout /t 1 /nobreak >nul
echo %ESC%[92m✨ System check passed!%ESC%[0m
echo %ESC%[93m🔄 Initializing Windows environment...%ESC%[0m
timeout /t 1 /nobreak >nul
echo %ESC%[94m🚀 Preparing server components...%ESC%[0m
timeout /t 1 /nobreak >nul

echo.
echo %ESC%[95m╔═══════════════════════╗%ESC%[0m
echo %ESC%[95m║     Server Setup      ║%ESC%[0m
echo %ESC%[95m╚═══════════════════════╝%ESC%[0m
echo %ESC%[96m📂 Accessing server directory...%ESC%[0m
cd server || (
    echo %ESC%[91m❌ Failed to access server directory!%ESC%[0m
    exit /b 1
)

REM Compiler settings
set GCC=C:/strawberry/c/bin/g++.exe
set INCLUDE_FLAGS=-I../behind/include/external/cpp-httplib -I../behind/include/external/nlohmann
set CXX_FLAGS=-std=c++11
set LIB_FLAGS=-lpthread -lws2_32

echo.
echo %ESC%[93m🧹 Cleaning workspace...%ESC%[0m
del /F /Q local_server.exe

echo %ESC%[92m⚙️  Compiling server components...%ESC%[0m
%GCC% %INCLUDE_FLAGS% %CXX_FLAGS% local_server.cpp %LIB_FLAGS% -o local_server.exe

if %ERRORLEVEL% EQU 0 (
    echo %ESC%[92m✅ Server compilation completed successfully!%ESC%[0m
    
    echo.
    echo %ESC%[95m╔════════════════════════╗%ESC%[0m
    echo %ESC%[95m║  Starting Components   ║%ESC%[0m
    echo %ESC%[95m╚════════════════════════╝%ESC%[0m
    echo %ESC%[96m🚀 Initializing server...%ESC%[0m
    start local_server.exe
    
    echo %ESC%[94m⚡ Starting backend services...%ESC%[0m
    cd ../behind
    start make rebuild run
    cd ../
    
    echo.
    echo %ESC%[92m╔═══════════════════════════════════════╗%ESC%[0m
    echo %ESC%[92m║  ✨ Car Simulator is ready to use!    ║%ESC%[0m
    echo %ESC%[92m╚═══════════════════════════════════════╝%ESC%[0m
) else (
    echo %ESC%[91m╔═══════════════════════════════════════╗%ESC%[0m
    echo %ESC%[91m║  ❌ Server compilation failed!         ║%ESC%[0m
    echo %ESC%[91m║  Please check your setup and try again ║%ESC%[0m
    echo %ESC%[91m╚═══════════════════════════════════════╝%ESC%[0m
    exit /b 1
)