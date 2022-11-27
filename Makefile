CC = g++

CFLAGS = -g -Wall

TARGET = hellogl

all: $(TARGET)

$(TARGET): main.cpp glad.c
	$(CC) $(CFLAGS) -o $(TARGET) main.cpp glad.c -lglfw -lGL

run: $(TARGET)
	./$(TARGET)

clean:
	rm $(TARGET)
