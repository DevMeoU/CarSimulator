#!/bin/bash
@echo off
clear
echo "🚓Running on Linux"
echo "🚀Starting establish server..."
echo "⏭️Moving to server folder..."
cd server
eval g++ -std=c++17 local_server.cpp -ljsoncpp -o local_server.exe && ./local_server.exe
