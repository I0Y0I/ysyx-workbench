CC := gcc
CFLAGS = -Wall -g

SRC := $(wildcard *.c)
TARGETS := $(patsubst %.c, build/%, $(SRC))

.PHONY: all
all: $(TARGETS)

build:
	@echo "Creating build directory..."
	mkdir -p build

build/%: %.c | build
	@echo "Compiling executable: $@"
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning up..."
	$(RM) -rf build
