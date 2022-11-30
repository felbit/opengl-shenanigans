CC = g++

CFLAGS = -g -Wall

all: basics

basics: basics/main.cpp misc/glad.c
	$(CC) $(CFLAGS) -o basics/basics basics/main.cpp misc/glad.c -lglfw -lGL

clean:
	rm basics
