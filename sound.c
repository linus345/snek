#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "sound.h"

Sound_effects* init_sounds()
{
    Sound_effects* sound = malloc(sizeof(Sound_effects));

    // Menu sound effects
    sound->press = Mix_LoadWAV("./resources/Sound_Effects/Button_press_effect.wav");
    sound->hover = Mix_LoadWAV("./resources/Sound_Effects/Hover_effect.wav");
    sound->back = Mix_LoadWAV("./resources/Sound_Effects/VALENTINE_whip_one_shot_high.wav");

    // Game sound effects
    sound->eat = Mix_LoadWAV("./resources/Sound_Effects/Eat_effect.wav");
    sound->wall_collison = Mix_LoadWAV("./resources/Sound_Effects/Slap effect.wav");
    sound->scoreboard = Mix_LoadWAV("./resources/Sound_Effects/Scoreboard_effect.wav");

    // Song
    sound->song = Mix_LoadMUS("./resources/Sound_Effects/Scoreboard_effect.wav");

    sound->muted = false;

    return sound;
}

void play_sound(Mix_Chunk* sound)
{
    Mix_PlayChannel(-1, sound, 0);
}

void play_song(Mix_Music* song)
{
    Mix_
}

void play_hover_sound(Sound_effects* sound, bool *playsound)
{
    if (*playsound) { // Makes sure the sound effect only plays once
        play_sound(sound->hover); // Plays hover effect
        *playsound = false;
    }
}

void free_sound_effects(Sound_effects* sound)
{
    Mix_FreeChunk(sound->press);
    Mix_FreeChunk(sound->hover);
    Mix_FreeChunk(sound->back);
    Mix_FreeChunk(sound->eat);
    Mix_FreeChunk(sound->wall_collison);
    Mix_FreeChunk(sound->scoreboard);
    Mix_FreeSound(sound->song);
}
