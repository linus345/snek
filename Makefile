# set the compiler
CC := gcc

# general compiler flags
CFLAGS := --std=c99 -Wall -Wextra -pedantic

ifeq ($(OS),Windows_NT)
	CFLAGS += -IC:\libsdl\include -LC:\libsdl\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_ttf
	EXEC := snek.exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CFLAGS += `sdl2-config --libs --cflags` -lSDL2_net -lSDL2_image -lSDL2_ttf -lm
		EXEC := snek.out
	endif
	ifeq ($(UNAME_S),Darwin)
		CFLAGS += `sdl2-config --libs --cflags` -lSDL2_net -lSDL2_image -lSDL2_ttf -lm
		EXEC := snek.out
	endif
endif

$(EXEC): main.o game.o app.o snake.o player.o fruit.o menu.o
	$(CC) *.o $(CFLAGS) -o $(EXEC)

main.o: main.c
	$(CC) main.c -c $(CFLAGS)

game.o: game.c
	$(CC) game.c -c $(CFLAGS)

app.o: app.c
	$(CC) app.c -c $(CFLAGS)
	
fruit.o: fruit.c
	$(CC) fruit.c -c $(CFLAGS)

snake.o: snake.c
	$(CC) snake.c -c $(CFLAGS)

player.o: player.c
	$(CC) player.c -c $(CFLAGS)

menu.o: menu.c
	$(CC) menu.c -c $(CFLAGS)

clean:
	rm -f $(EXEC) *.o

.PHONY: clean
