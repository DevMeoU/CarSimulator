#!/bin/bash
@echo off
eval chcp 65001>nul
clear
echo "👽 Running on Linux"
echo "🚓 Starting Car simulator..."
echo "🚀 Starting establish server..."
echo "⏭️ Moving to server folder..."
cd server

GCC="C:/strawberry/c/bin/g++.exe"
INCLUDE_FLAGS="-I../behind/include/external/cpp-httplib -I../behind/include/external/nlohmann"
CXX_FLAGS="-std=c++11"
LIB_FLAGS="-lpthread -lws2_32"

echo "🛠️ Cleaning up..."
eval rm -rf local_server.exe

echo "🛠️ Compiling server..."
echo "$GCC" $INCLUDE_FLAGS $CXX_FLAGS local_server.cpp $LIB_FLAGS -o local_server.exe
eval "$GCC" $INCLUDE_FLAGS $CXX_FLAGS local_server.cpp $LIB_FLAGS -o local_server.exe