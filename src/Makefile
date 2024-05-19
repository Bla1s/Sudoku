CXX = g++
CXXFLAGS = -std=c++17 -I./include/headers -I./include/SDL2
LDFLAGS = -L./lib -lSDL2 -lSDL2_ttf

SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE = Main.exe

$(EXECUTABLE): $(OBJECTS)
    $(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    rm -f $(OBJ_DIR)/*.o $(EXECUTABLE)
