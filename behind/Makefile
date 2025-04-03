ifeq ($(OS),Windows_NT)
$(info Running on Windows)
CXX := C:\Strawberry\c\bin\g++.exe
else
CXX := g++
endif

WINDOWS_LIB = -I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt" \
			  -I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um" \
			  -I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared" \
			  -I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\cppwinrt" \
			  -I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\winrt"

CXXFLAGS := -std=c++17 -Wall -I./include -I./include/external/cpp-httplib -I./include/external/nlohmann -D_WIN32_WINNT=0x0600

ifeq ($(OS),Windows_NT)
	LDFLAGS := -lstdc++ -lpthread -lws2_32
else
	LDFLAGS := -lstdc++ -lpthread
endif

SRC_DIR := src
BUILD_DIR := build

SRCS := $(filter-out $(SRC_DIR)/create_ui.cpp $(SRC_DIR)/main_integrated.cpp, $(wildcard $(SRC_DIR)/*.cpp)) \
	$(SRC_DIR)/EnvironmentThread.cpp \
	$(SRC_DIR)/MainThread.cpp \
	$(SRC_DIR)/KeyboardThread.cpp \
	$(SRC_DIR)/ServerThread.cpp \
	$(SRC_DIR)/main.cpp

OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

EXEC := car_simulator.exe

all: $(BUILD_DIR)/$(EXEC)

$(BUILD_DIR)/$(EXEC): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

rebuild: clean all

-include $(DEPS)

run: $(BUILD_DIR)/$(EXEC)
	@$(BUILD_DIR)/$(EXEC)

.PHONY: all clean rebuild run