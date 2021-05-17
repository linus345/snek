# set the compiler
CC := gcc

# general compiler flags
CFLAGS := --std=c99 -Wall -Wextra -pedantic -g

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

$(EXEC): main.o game.o app.o snake.o player.o fruit.o network.o circular_buffer.o
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

network.o: network.c
	$(CC) network.c -c $(CFLAGS)

circular_buffer.o: circular_buffer.c
	$(CC) circular_buffer.c -c $(CFLAGS)

clean:
	rm -f $(EXEC) *.o

.PHONY: clean
