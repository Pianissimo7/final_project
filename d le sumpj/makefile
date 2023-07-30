# Compiler and compiler flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall -mconsole

# Directories
SRC_DIR := sources
INC_DIR := headers
BUILD_DIR := objects

# Source and object files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp) main.cpp
HEADERS := $(wildcard $(INC_DIR)/*.hpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Target executable
TARGET := a.exe

# Default target
all: $(BUILD_DIR) $(TARGET)
	./$(TARGET)

# Rule to build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Rule to create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Clean the build directory
clean:
	del /Q $(BUILD_DIR) $(TARGET)