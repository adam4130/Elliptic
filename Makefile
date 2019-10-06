CXX := g++

SRC_DIR := src
BUILD_DIR := build

EXT := cpp

SRC := $(wildcard $(SRC_DIR)/*.$(EXT))
OBJ := $(SRC:$(SRC_DIR)/%.$(EXT)=$(BUILD_DIR)/%.o)

CXX_FLAGS := -Wall -g
LIB_FLAGS := -lgmpxx -lgmp
INC := -I include

TARGET := elliptic

$(shell mkdir -p $(BUILD_DIR))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ $(LIB_FLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(EXT)
	$(CXX) $(CXX_FLAGS) $(INC) -c $< -o $@

clean:
	$(RM) $(BUILD_DIR)/*.o $(TARGET)
