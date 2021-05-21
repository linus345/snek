#ifndef SOUND_H
#define SOUND_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef struct sound_effects {
    Mix_Chunk* press;
    Mix_Chunk* hover;
    Mix_Chunk* back;
    Mix_Chunk* eat;
    Mix_Chunk* wall_collison;
    Mix_Chunk* scoreboard;

} Sound_effects;

Sound_effects* int_sounds();
void play_sound(Mix_Chunk* sound);
void free_sound_effects(Sound_effects* sound);

#endif