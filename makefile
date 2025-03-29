CC = gcc
CFLAGS = -Wall

SRC = scdl.c scdl_time.c

CACHE_FOLDER = makeCache
MAIN_SRC_FILE = scdl
OUT_EXC_FILE = logger

DLL_FLAGS = -fPIC -DSCDL_EXPORT

OBJ = $(patsubst %.c, $(CACHE_FOLDER)/%.o, $(SRC))

.PHONY: all build clean dll
all: build

build:$(OBJ)
	$(CC) $(CFLAGS) $^ -o $(OUT_EXC_FILE).exe

$(CACHE_FOLDER)/%.o: %.c
	mkdir -p $(CACHE_FOLDER)
	$(CC) $(CFLAGS) -c $< -o $@

dll:$(OBJ)
	$(CC) $(CFLAGS) $^ -o $(OUT_EXC_FILE).dll $(DLL_FLAGS)

clean:
	rm -rf $(CACHE_FOLDER) $(OUT_EXC_FILE).exe $(OUT_EXC_FILE).dll

nuke:clean