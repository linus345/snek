# set the compiler
CC := gcc

# general compiler flags
CFLAGS := --std=c99 -Wall -Wextra -pedantic

ifeq ($(OS),Windows_NT)
	CFLAGS += -IC:\libsdl\include -LC:\libsdl\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_image
	EXEC := snek.exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CFLAGS += `sdl2-config --libs --cflags` -lSDL2_net -lSDL2_image
		EXEC := snek.out
	endif
	ifeq ($(UNAME_S),Darwin)
		CFLAGS += `sdl2-config --libs --cflags` -lSDL2_net -lSDL2_image
		EXEC := snek.out
	endif
endif

$(EXEC): main.o game.o app.o
	$(CC) *.o $(CFLAGS) -o $(EXEC)

main.o: main.c
	$(CC) main.c -c $(CFLAGS)

game.o: game.c
	$(CC) game.c -c $(CFLAGS)

app.o: app.c
	$(CC) app.c -c $(CFLAGS)

clean:
	rm -f $(EXEC) *.o

.PHONY: clean
