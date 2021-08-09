# Created by Dima Sushchenko
# Creation Date: 01.06.2021
# Last Update: 26.06.2021
# Makefile to build a project from .cpp files.
#

# Constants
TRUE = true
FALSE = false

# Chosen compiler
CXX = g++

# Chosen modes
DEBUG_MODE = Debug
RELEASE_MODE = Release

BUILD_MODE = $(DEBUG_MODE)
AUTO_RUN = $(TRUE)

# Flags for two build modes
Release_FLAGS = -O3 -std=c++17 -s -DNDEBUG
Debug_FLAGS = -O0 -std=c++17 -g -Wall

# EXECUTABLE - main filename
# SOURCES - returns list of all .cpp files
EXECUTABLE = main
SOURCES = $(shell find ./include -name '*.cpp')

# TEST - filename for testing
TEST = tests
TEST_FOLDER_NAME = Test

# Run build
all: build
ifeq ($(AUTO_RUN),$(TRUE))
all: run
endif

# Build project
build:
	@mkdir -p $(BUILD_MODE)
	$(CXX) $($(BUILD_MODE)_FLAGS) $(EXECUTABLE).cpp $(SOURCES) -o $(BUILD_MODE)/$(EXECUTABLE)
	@echo "makefile: \033[;32mBuild have done!\033[0m"

# Run program after build
run:
	@$(BUILD_MODE)/$(EXECUTABLE)

test:
	@mkdir -p $(TEST_FOLDER_NAME)
	$(CXX) $($(DEBUG_MODE)_FLAGS) $(TEST).cpp $(SOURCES) -o $(TEST_FOLDER_NAME)/$(TEST)
	@$(TEST_FOLDER_NAME)/$(TEST)
	@echo "makefile: \033[;32mTesting have done!\033[0m"

test_gdb:
	@gdb $(TEST_FOLDER_NAME)/$(TEST)

# Debugging program
gdb:
	@gdb $(DEBUG_MODE)/$(EXECUTABLE)

# Check memory leak
valgrind:
	@valgrind $(args) $(TEST_FOLDER_NAME)/$(TEST)

# Remove folders with executable files
clean: rm_debug rm_release rm_test

rm_debug:
	@rm -rf $(DEBUG_MODE)

rm_release:
	@rm -rf $(RELEASE_MODE)

rm_test:
	@rm -rf $(TEST_FOLDER_NAME)

.PHONY: all build run test test_gdb gdb valgrid clean rm_debug rm_release rm_test
