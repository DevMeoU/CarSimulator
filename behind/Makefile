CXX := g++
CXXFLAGS := -std=c++17 -Wall -I./include -I./include/external/cpp-httplib -I./include/external/nlohmann
LDFLAGS := -lstdc++ -lpthread

SRC_DIR := src
BUILD_DIR := build

SRCS := $(filter-out $(SRC_DIR)/create_ui.cpp $(SRC_DIR)/main_integrated.cpp, $(wildcard $(SRC_DIR)/*.cpp)) \
	$(SRC_DIR)/EnvironmentThread.cpp \
	$(SRC_DIR)/MainThread.cpp \
	$(SRC_DIR)/KeyboardThread.cpp \
	$(SRC_DIR)/ServerThread.cpp \
	$(SRC_DIR)/VehicleSimulation.cpp \
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

.PHONY: all clean rebuild