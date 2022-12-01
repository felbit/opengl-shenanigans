CC = g++

CFLAGS = -g -Wall

all: basics

basics: basics/main.cpp misc/glad.c
	$(CC) $(CFLAGS) -o basics/out basics/main.cpp misc/glad.c includes/* -lglfw -lGL

lighting: lighting/main.cpp misc/glad.c
	$(CC) $(CFLAGS) -o lighting/out lighting/main.cpp misc/glad.c includes/* -lglfw -lGL

clean:
	rm basics/out lighting/out
