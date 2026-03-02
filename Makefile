# =========================
# Compiler configuration
# =========================

CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -fPIC
INCLUDES := -Iinclude

# =========================
# Project structure
# =========================

SRC_DIR := src
OBJ_DIR := build
LIB_DIR := librairies
BIN_DIR := bin

# =========================
# Source files
# =========================

# NeuralNetwork library
NN_SRCS := $(shell find $(SRC_DIR)/neural_network -name "*.cpp")
NN_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(NN_SRCS))
NN_LIB := $(LIB_DIR)/libNeuralNetwork.so

# ExperimentRunner library
ER_SRCS := $(shell find $(SRC_DIR)/experiment_runner -name "*.cpp") \
           $(shell find $(SRC_DIR)/dataSetConversion/convert.cpp)
ER_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(ER_SRCS))

ER_LIB := $(LIB_DIR)/libExperimentRunner.so

# Shell
SHELL_SRCS := $(shell find $(SRC_DIR)/shell -name "*.cpp")
SHELL_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SHELL_SRCS))
SHELL_BIN := $(BIN_DIR)/shell

# Analyzer
ANALYZER_SRCS := $(shell find $(SRC_DIR)/my_torch_analyzer -name "*.cpp")
ANALYZER_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(ANALYZER_SRCS))
ANALYZER_BIN := my_torch_analyzer

# Benchmark
BENCHMARK_SRCS := $(shell find $(SRC_DIR)/benchmark -name "*.cpp")
BENCHMARK_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(BENCHMARK_SRCS))
BENCHMARK_BIN := $(BIN_DIR)/benchmark

# =========================
# Targets
# =========================

.PHONY: all neural_network experiment_runner shell analyzer benchmark clean re debug

all: neural_network experiment_runner shell analyzer benchmark

# -------------------------
# Neural Network Library
# -------------------------
neural_network: $(NN_LIB)

$(NN_LIB): $(NN_OBJS)
	@mkdir -p $(LIB_DIR)
	$(CXX) -shared -o $@ $^

# -------------------------
# Experiment Runner Library
# -------------------------
experiment_runner: $(ER_LIB)

$(ER_LIB): $(ER_OBJS) $(NN_LIB)
	@mkdir -p $(LIB_DIR)
	$(CXX) -shared -o $@ $^

# -------------------------
# Shell
# -------------------------
shell: $(SHELL_BIN)

$(SHELL_BIN): $(SHELL_OBJS) $(NN_LIB) $(ER_LIB)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ -ldl -Wl,-rpath,'\$ORIGIN/../librairies'

# -------------------------
# Analyzer
# -------------------------
analyzer: $(ANALYZER_BIN)

$(ANALYZER_BIN): $(ANALYZER_OBJS) $(NN_LIB) $(ER_LIB)
	$(CXX) -o $@ $^ -ldl -Wl,-rpath,'\$ORIGIN/../librairies'

# -------------------------
# Benchmark
# -------------------------
benchmark: $(BENCHMARK_BIN)

$(BENCHMARK_BIN): $(BENCHMARK_OBJS) $(NN_LIB) $(ER_LIB)
	$(CXX) -o $@ $^

# =========================
# Compile object files
# =========================

# Rule for compiling any cpp under src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# =========================
# Utilities
# =========================

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR) $(ANALYZER_BIN)

re: clean all

# =========================
# Debug build
# =========================

debug: CXXFLAGS += -g
debug: re
