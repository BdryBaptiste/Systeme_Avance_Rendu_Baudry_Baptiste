# Compiler and flags
CFLAGS = -Wall -Wextra -g -fprofile-arcs -ftest-coverage
LDFLAGS = -lgcov

# Source and object files
SRC_DIR = src
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/shell.c $(SRC_DIR)/execute.c $(SRC_DIR)/builtins.c \
      $(SRC_DIR)/history.c $(SRC_DIR)/alias.c $(SRC_DIR)/env.c
OBJ = $(SRC:.c=.o)

# Executable name
EXEC = my_shell

# Header files (dependencies)
HEADERS = src/shell.h src/typedef.h src/alias.h src/builtins.h src/env.h src/history.h src/execute.h

# Default target: Compile everything
all: $(EXEC)

# Compile the executable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS)

# Compile each .c into .o (with dependency tracking)
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ) $(EXEC)

# Debug mode (compiles with debugging symbols)
debug: CFLAGS += -DDEBUG
debug: clean all

# Full rebuild
rebuild: clean all

# Phony targets (avoid name conflicts)
.PHONY: all clean debug rebuild