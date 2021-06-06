# set the compiler
CC := gcc

# general compiler flags
CFLAGS := --std=c99 -Wall -Wextra -pedantic -g #-fsanitize=address

ifeq ($(OS),Windows_NT)
	CFLAGS += -IC:\libsdl\include -LC:\libsdl\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_ttf -lSDL2_mixer
	EXEC := snek.exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CFLAGS += `sdl2-config --libs --cflags` -lSDL2_net -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
		EXEC := snek.out
	endif
	ifeq ($(UNAME_S),Darwin)
		CFLAGS += `sdl2-config --libs --cflags` -lSDL2_net -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
		EXEC := snek.out
	endif
endif

$(EXEC): main.o game.o app.o snake.o player.o fruit.o network.o circular_buffer.o menu.o rendering.o sound.o
	$(CC) *.o $(CFLAGS) -o $(EXEC)

main.o: src/main.c
	$(CC) src/main.c -c $(CFLAGS)

game.o: src/game.c
	$(CC) src/game.c -c $(CFLAGS)

app.o: src/app.c
	$(CC) src/app.c -c $(CFLAGS)
	
fruit.o: src/fruit.c
	$(CC) src/fruit.c -c $(CFLAGS)

snake.o: src/snake.c
	$(CC) src/snake.c -c $(CFLAGS)

player.o: src/player.c
	$(CC) src/player.c -c $(CFLAGS)

network.o: src/network.c
	$(CC) src/network.c -c $(CFLAGS)

circular_buffer.o: src/circular_buffer.c
	$(CC) src/circular_buffer.c -c $(CFLAGS)

menu.o: src/menu.c
	$(CC) src/menu.c -c $(CFLAGS)

rendering.o: src/rendering.c
	$(CC) src/rendering.c -c $(CFLAGS)

sound.o: src/sound.c
	$(CC) src/sound.c -c $(CFLAGS)

clean:
	rm -f $(EXEC) *.o

.PHONY: clean
