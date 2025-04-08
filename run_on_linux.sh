#!/bin/bash

# Set UTF-8 encoding
export LANG=en_US.UTF-8

# Clear screen
clear

# Print header with colors
echo -e "\033[1;36m=== 🚗 Car Simulator - Linux Environment ===\033[0m"
echo

echo -e "\033[1;32m👽 Initializing Linux environment...\033[0m"
sleep 0.5
echo -e "\033[1;33m🚓 Starting Car Simulator...\033[0m"
sleep 0.5
echo -e "\033[1;34m🚀 Establishing server connection...\033[0m"
sleep 0.5

echo -e "\n\033[1;35m=== Server Setup ===\033[0m"
echo -e "\033[1;36m⏭️  Moving to server directory...\033[0m"
cd server || exit

# Compiler settings
GCC="g++"
INCLUDE_FLAGS="-I../behind/include/external/cpp-httplib -I../behind/include/external/nlohmann"
CXX_FLAGS="-std=c++11"
LIB_FLAGS="-lpthread"

echo -e "\n\033[1;33m🛠️  Cleaning previous builds...\033[0m"
rm -f local_server

echo -e "\033[1;32m🛠️  Compiling server...\033[0m"
$GCC $INCLUDE_FLAGS $CXX_FLAGS local_server.cpp $LIB_FLAGS -o local_server

if [ $? -eq 0 ]; then
    echo -e "\033[1;32m✅ Server compilation successful!\033[0m"
    
    echo -e "\n\033[1;35m=== Starting Components ===\033[0m"
    echo -e "\033[1;36m🚀 Launching server...\033[0m"
    ./local_server &
    
    echo -e "\033[1;34m🔧 Initializing backend services...\033[0m"
    cd ../behind || exit
    make clean && make all
    
    echo -e "\n\033[1;32m✨ Car Simulator is ready!\033[0m"
else
    echo -e "\033[1;31m❌ Server compilation failed!\033[0m"
    exit 1
fi