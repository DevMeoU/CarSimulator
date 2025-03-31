CXX = C:\cygwin64\bin\g++.exe
CXXFLAGS = -std=c++17 -Wall -Wextra -D_GLIBCXX_USE_CXX11_ABI=1
INCLUDES = -I../ -I../../ -I../../keyboard/ -I../../server/cpp-httplib/ -I../../third_party/
LDFLAGS = -static-libstdc++ -static-libgcc
LIBS = 

ifeq ($(OS),Windows_NT)
    TARGET = test_keyboard_windows.exe
    SOURCES = test_keyboard_windows.cpp
    LIBS += -luser32
else
    TARGET = test_keyboard_linux
    SOURCES = test_keyboard_linux.cpp
    LIBS += -lncurses
endif

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
