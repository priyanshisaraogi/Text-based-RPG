CC = gcc
CFLAGS = -Wall -Werror -pedantic -ansi
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = Legacy_of_Yharnam

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) src/*.o

.PHONY: all clean
