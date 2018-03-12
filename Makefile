# mtgsimul

VERSION = 0.0.1

# includes and libs
LIBS =

# flags
CFLAGS = -Wall -O3 ${LIBS} -DVERSION=\"${VERSION}\" -I./include -I./src/cards/ -std=c++11 -g

SRC = src/main.cpp
BIN = mtgsimul

all:
	g++ -o $(BIN) $(SRC) $(CFLAGS)

clean:
	rm -f $(BIN)
	rm -rf src/*.swp src/cards/*.swp include/*.swp


install: all
	cp -f $(BIN) /usr/local/bin/
