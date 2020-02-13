CXX := g++

SRC_DIR := src
BUILD_DIR := build
TEST_DIR := test

EXT := cpp

SRC := $(wildcard $(SRC_DIR)/*.$(EXT))
OBJ := $(SRC:$(SRC_DIR)/%.$(EXT)=$(BUILD_DIR)/%.o)

TEST_SRC := $(wildcard $(TEST_DIR)/*.$(EXT))
TEST_OBJ := $(TEST_SRC:$(TEST_DIR)/%.$(EXT)=$(BUILD_DIR)/%.o)
TEST_OBJ += $(filter-out $(BUILD_DIR)/main.o, $(OBJ))

CXX_FLAGS := -Wall -Werror
LIB_FLAGS := -lgmpxx -lgmp -lcrypto
INC := -I include

TARGET := elliptic
TEST_TARGET := tests

$(shell mkdir -p $(BUILD_DIR))

.PHONY: all test clean

all: $(TARGET)
test: $(TEST_TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ $(LIB_FLAGS) -o $@

$(TEST_TARGET): $(TEST_OBJ)
	$(CXX) $^ $(LIB_FLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(EXT)
	$(CXX) $(CXX_FLAGS) $(INC) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.$(EXT)
	$(CXX) $(CXX_FLAGS) $(INC) -c $< -o $@

clean:
	$(RM) $(BUILD_DIR)/*.o $(TARGET) $(TEST_TARGET)

