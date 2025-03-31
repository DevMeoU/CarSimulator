CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./system/SynchronizedQueue -I./keyboard
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
CXXFLAGS += -DPLATFORM_LINUX
LDFLAGS := -lncurses
else
CXXFLAGS += -DPLATFORM_WINDOWS
LDFLAGS := -luser32 -lws2_32 -mconsole
endif

SRC_DIR := .
BUILD_DIR := build

SRCS := $(SRC_DIR)/system/test/SynchronizedQueueTestKeyboard.cpp \
	$(SRC_DIR)/keyboard/kb.cpp \
	$(SRC_DIR)/base/queue/SynchronizedQueue.cpp

TEST_TARGET := system/test/SynchronizedQueueTestKeyboard.exe
KEYBOARD_TEST_TARGET := keyboard/kb.exe
QUEUE_TEST_TARGET := system/SynchronizedQueue/SynchronizedQueueTest.exe

OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGET := $(TEST_TARGET)

all: $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/$(KEYBOARD_TEST_TARGET) $(BUILD_DIR)/$(QUEUE_TEST_TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(KEYBOARD_TEST_TARGET): $(BUILD_DIR)/keyboard/kb.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(QUEUE_TEST_TARGET): $(BUILD_DIR)/system/SynchronizedQueue/SynchronizedQueueTest.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/keyboard_test.exe: $(BUILD_DIR)/keyboard/kb.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/queue_test.exe: $(BUILD_DIR)/system/SynchronizedQueue/SynchronizedQueueTest.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	@$(BUILD_DIR)/$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)

help:
	@echo "Makefile for ECU Application"
	@echo "Targets:"
	@echo "  all     Build the application (default)"
	@echo "  run     Build and run the application"
	@echo "  clean   Remove build artifacts"

.PHONY: all clean run help