CC = g++

CFLAGS = -g -Wall

cpp_files := $(wildcard *.cpp)
hpp_files := $(wildcard *.hpp)

all: bin/opengl

bin/opengl: $(cpp_files) $(hpp_files)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ src/main.cpp libs/glad.c -lglfw -lGL

run: bin/opengl
	@cd bin/ && ./opengl && cd ..

clean:
	rm -Rf bin/
