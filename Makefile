# Compile Flags
CC          = g++
LD          = g++
CCFLAGS     = -Wall -Wextra -std=c++17
OPT			= -O0
DEPFLAGS    = -MP -MD
THREADS		= -pthread

# Main Program
PROG_FLIK	= flik.out

# Folders
SRC_DIR		= ./src
BUILD_DIR	= ./build
BIN_DIR     = ./bin
INCLUDE_DIR	=. ./include

# CPP, Sources, Dependencies and Object files
CPP_LIST_FLIK = main.cpp flik.cpp vmachine.cpp task.cpp
SRC_LIST_FLIK = $(patsubst %.cpp,$(SRC_DIR)/%.cpp,$(CPP_LIST_FLIK))
OBJ_LIST_FLIK = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_LIST_FLIK))
DEP_LIST_FLIK = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SRC_LIST_FLIK))

INCLUDES = $(foreach dir, $(INCLUDE_DIR), $(addprefix -I, $(dir)))
-include $(DEP_LIST_FLIK)

# Prettier Makefile
NO_COLOR=$(shell printf "%b" "\033[0m")
OK_COLOR=$(shell printf "%b" "\033[32;01m")
OK_STRING=$(OK_COLOR)[OK]$(NO_COLOR)

.PHONY: all clean $(PROG_FLIK)

all: $(BIN_DIR)/$(PROG_FLIK)

$(BIN_DIR)/$(PROG_FLIK): $(OBJ_LIST_FLIK)
	@echo "Linking the target $(PROG_FLIK) in $(BIN_DIR)"
	$(LD) -o $@ $^ $(THREADS)
	@echo ""

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $< in $(BUILD_DIR) $(OK_STRING)"
	@$(CC) $(CCFLAGS) -g $(INCLUDES) $(OPT) $(DEPFLAGS) -c -o $@ $<

clean:
	rm -f $(BIN_DIR)/$(PROG_FLIK) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d
