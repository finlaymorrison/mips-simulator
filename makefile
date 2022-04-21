RM := rm -f
CXX := g++
CXXFLAGS := -I include

SRC_DIR := source
OBJ_DIR := obj
BIN_DIR := bin
INC_DIR := include

EXE := $(BIN_DIR)/mips-simulator
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
INC := $(wildcard $(INC_DIR)/*.h)

.PHONY: all clean

all: $(EXE)

clean:
	$(RM) -r $(BIN_DIR) $(OBJ_DIR)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@
