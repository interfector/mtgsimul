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

oops:
	ln -sf OopsAllSpells/OopsAllSpells.hpp src/cards/TestDeck.hpp

oops_hivemind:
	ln -sf OopsAllSpells/OopsAllSpells_Hivemind.hpp src/cards/TestDeck.hpp

oops_belcher:
	ln -sf OopsAllSpells/OopsAllSpells_Belcher.hpp src/cards/TestDeck.hpp

belcher:
	ln -sf Charbelcher/Charbelcher.hpp src/cards/TestDeck.hpp
	
turbodepths:
	ln -sf TurboDepths/TurboDepths.hpp src/cards/TestDeck.hpp

clean:
	rm -rf src/cards/TestDeck.hpp
	rm -f $(BIN)
	rm -rf src/*.swp src/cards/*.swp include/*.swp


install: all
	cp -f $(BIN) /usr/local/bin/
