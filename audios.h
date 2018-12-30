#ifndef AUDIOS_H_INCLUDED
#define AUDIOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define AUD_setMusicVolume(zero128) Mix_VolumeMusic(zero128)
#define AUD_stopSFX() Mix_HaltChannel(-1)
#define AUD_stopMusic() Mix_HaltMusic()
#define AUD_freeSFX(sfx) Mix_FreeChunk(sfx)
#define AUD_freeMusic(music) Mix_FreeMusic(music)

#define AUD_SFX Mix_Chunk
#define AUD_Music Mix_Music
//https://en.wikipedia.org/wiki/Sampling_(signal_processing)
#define AUD_QUALITY_PROFESSIONAL 48000
#define AUD_QUALITY_CD 44100
#define AUD_QUALITY_GAME 22050

typedef struct AUD_SFX_MemDump{
    Mix_Chunk *sfx;
    struct AUD_SFX_MemDump *next;
}AUD_SFX_MemDump;

typedef struct AUD_Music_MemDump{
    Mix_Music *music;
    struct AUD_Music_MemDump *next;
}AUD_Music_MemDump;

int AUD_init(int audioQuality);
int AUD_initAudio(int audioQuality, int audioFlags);
void AUD_exit();

AUD_SFX* AUD_newSFX(const char* wavOggAiffRiffVOC_File);
AUD_Music* AUD_newMusic(const char* mp3OggFlac_File);

void AUD_playSFX(AUD_SFX *sfx);
void AUD_playMusic(AUD_Music *music);
void AUD_setSFXVolume(const int zero128);

#endif // AUDIOS_H_INCLUDED
