
SRC = main.c
TARGET = main.exe

FLAGS = -lraylib -mwindows

.PHONY: all build run

all: build

build:
	gcc $(SRC) -o $(TARGET) $(FLAGS)

run: build
	./$(TARGET)