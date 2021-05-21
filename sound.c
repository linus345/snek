#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

#include "sound.h"

Sound_effects* int_sounds()
{
    Sound_effects* sound = malloc(sizeof(Sound_effects));

    // Menu sound effects
    sound->press = Mix_LoadWAV("./resources/Sound Effects/Button_press_effect.wav");
    sound->hover = Mix_LoadWAV("./resources/Sound Effects/Hover_effect.wav");
    sound->back = Mix_LoadWAV("./resources/Sound Effects/VALENTINE_whip_one_shot_high.wav");

    // Game sound effects
    sound->eat = Mix_LoadWAV("./resources/Sound Effects/Eat_effect.wav");
    sound->wall_collison = Mix_LoadWAV("./resources/Sound Effects/Slap effect.wav");
    sound->scoreboard = Mix_LoadWAV("./resources/Sound Effects/Scoreboard_effect.wav");

    return sound;
}

void play_sound(Mix_Chunk* sound)
{
    Mix_PlayChannel(-1, sound, 0);
}

void free_sound_effects(Sound_effects* sound)
{
    Mix_FreeChunk(sound->press);
    Mix_FreeChunk(sound->hover);
    Mix_FreeChunk(sound->back);
    Mix_FreeChunk(sound->eat);
    Mix_FreeChunk(sound->wall_collison);
    Mix_FreeChunk(sound->scoreboard);
}