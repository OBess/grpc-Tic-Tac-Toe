# Created by Dima Sushchenko
# Creation Date: 01.06.2021
# Last Update: 26.06.2021
# Makefile to build a project from .cpp files.
#

# Constants
TRUE = true
FALSE = false

# Chosen modes
DEBUG_MODE = Debug
RELEASE_MODE = Release

BUILD_MODE = $(DEBUG_MODE)
# Auto run
AUTO_RUN = $(FALSE)

# Pathes
CLIENT_PATH = client
SERVER_PATH = server

# Run build
all: build
ifeq ($(AUTO_RUN),$(TRUE))
all: run
endif

# Build project
build:
	@cd $(CLIENT_PATH) && make && cd ..
	@cd $(SERVER_PATH) && make && cd ..

# Run program after build
run:
# Server
	@$(BUILD_MODE)/$(SERVER_PATH)/$(EXECUTABLE)
# Client №1
	@$(BUILD_MODE)/$(CLIENT_PATH)/$(EXECUTABLE)
# Client №2
	@$(BUILD_MODE)/$(CLIENT_PATH)/$(EXECUTABLE)

# Remove folders with executable files
# clean: 

.PHONY: all build run test test_gdb gdb valgrid clean rm_debug rm_release rm_test
