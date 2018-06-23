#ifndef AUDIOS_H_INCLUDED
#define AUDIOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define AUD_SFX Mix_Chunk
#define AUD_Music Mix_Music

//https://en.wikipedia.org/wiki/Sampling_(signal_processing)
#define AUD_QUALITY_PROFESSIONAL 48000
#define AUD_QUALITY_CD 44100
#define AUD_QUALITY_GAME 22050

typedef struct AUD_SFXCache{
    Mix_Chunk *sfx;
    struct AUD_SFXCache *next;
}AUD_SFXCache;

typedef struct AUD_MusicCache{
    Mix_Music *music;
    struct AUD_MusicCache *next;
}AUD_MusicCache;

int AUD_initAudio(int audioQuality);
AUD_SFX* AUD_newSFX(char* wavOggAiffRiffVOC);
AUD_Music* AUD_newMusic(char* mp3OggFlac);
void AUD_exit();

#endif // AUDIOS_H_INCLUDED
