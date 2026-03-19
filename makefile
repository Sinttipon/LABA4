CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99

TARGET_MAIN = main.exe
TARGET_TESTS = tests.exe

SRC_COMMON = polynom.c integer.c double.c prosessing.c utilits.c
SRC_MAIN = main.c
SRC_TESTS = tests.c

OBJ_COMMON = $(SRC_COMMON:.c=.o)
OBJ_MAIN = $(SRC_MAIN:.c=.o)
OBJ_TESTS = $(SRC_TESTS:.c=.o)

all: $(TARGET_MAIN) $(TARGET_TESTS)

$(TARGET_MAIN): $(OBJ_COMMON) $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $^ 

$(TARGET_TESTS): $(OBJ_COMMON) $(OBJ_TESTS)
	$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /f *.o $(TARGET_MAIN) $(TARGET_TESTS)

run_tests: $(TARGET_TESTS)
	./$(TARGET_TESTS)

run: $(TARGET_MAIN)
	./$(TARGET_MAIN)

.PHONY: all clean run run_tests