#ifndef GAME_H
#define GAME_H

#define CELL_SIZE 32

typedef struct {
    int x;
    int y;
} Pos;

void load_texture(App *app, SDL_Texture **texture, char *path);

#endif