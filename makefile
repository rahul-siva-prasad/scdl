# Compiler and flags
CC 		= gcc
CFLAGS 	= -Wall

# Source files and output settings
SRC				= scdl.c scdl_time.c
SAMPLE_SRC 		= sample
CACHE_FOLDER 	= makeCache
SHARED_LIB 		= logger
SAMPLE_EXC 		= a

# Flags for creating shared library
SHARED_LIB_FLAGS = -shared -fPIC -DSCDL_EXPORT

# Object files (compiled source files)
OBJ = $(patsubst %.c, $(CACHE_FOLDER)/%.o, $(SRC))

# Detect OS
ifeq ($(OS),Windows_NT)
    SHARED_LIB_EXT = dll
else
    SHARED_LIB_EXT = so
endif

# Phony targets (not actual files)
.PHONY : all build shared_lib sample clean

# Default target
all : build

# Build target: compiles object files and creates the DLL
build : shared_lib

# Rule to compile source files into object files
$(CACHE_FOLDER)/%.o: %.c
	@echo "Compiling $< into $@"
	@mkdir -p $(CACHE_FOLDER)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the shared library
shared_lib: $(OBJ)
	@echo "Creating shared library: $(SHARED_LIB).$(SHARED_LIB_EXT)"
	$(CC) $(CFLAGS) $^ -o $(SHARED_LIB).$(SHARED_LIB_EXT) $(SHARED_LIB_FLAGS)

# Clean target: removes generated files
clean:
	@echo "Cleaning up generated files..."
	@rm -rf $(CACHE_FOLDER) $(SHARED_LIB).exe $(SHARED_LIB).$(SHARED_LIB_EXT) *.log $(SAMPLE_EXC).exe

# Build sample.c file
sample: shared_lib
	@echo "Building $(SAMPLE_SRC).c"
	$(CC) $(CFLAGS) $@.c -o $(SAMPLE_EXC).exe -L. -l$(SHARED_LIB)