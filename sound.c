#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

#include "sound.h"

Sound_effects* int_sounds()
{
    Sound_effects* sound = malloc(sizeof(Sound_effects));

    // Menu sound effects
    sound->press = Mix_LoadWAV("./resources/Sound Effects/Button_press_effect.wav");
    sound->hover = Mix_LoadWAV("./resources/Sound Effects/Hover_effect.wav");
    sound->back = Mix_LoadWAV("./resources/Sound Effects/VALENTINE_whip_one_shot_high.wav");

    // Game sound effects
    sound->eat = Mix_LoadWAV("");
    sound->wall_collison = Mix_LoadWAV("");


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
}