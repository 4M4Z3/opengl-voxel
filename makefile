# Set directories for source and include files
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Find all .cpp files in the SRC_DIR and create corresponding .o files in OBJ_DIR
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Target executable name
TARGET = game

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I$(INC_DIR) -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lGLEW -lglfw -framework OpenGL

# Link all object files to create the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

# Compile .cpp files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the OBJ_DIR if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up the build
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)
