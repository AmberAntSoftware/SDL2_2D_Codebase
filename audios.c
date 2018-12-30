#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "audios.h"

static int AUD_X_SFXvolume = 100;

static AUD_SFX_MemDump *sfxCache = NULL;
static AUD_SFX_MemDump *lastCache = NULL;

static AUD_Music_MemDump *musicCache = NULL;
static AUD_Music_MemDump *mlastCache = NULL;

int AUD_init(int audioQuality){
    return AUD_initAudio(audioQuality,MIX_INIT_OGG|MIX_INIT_FLAC|MIX_INIT_MP3);
}

int AUD_initAudio(int audioQuality, int audioFlags){
    int flags=audioFlags;
    int initted=Mix_Init(flags);
    if((initted&flags) != flags){
        printf("Mix_Init: Failed to init required ogg, flac, mp3 support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
    }

    if(audioQuality<AUD_QUALITY_GAME){
        audioQuality = AUD_QUALITY_GAME;
    }

    if(audioQuality>AUD_QUALITY_PROFESSIONAL){
        audioQuality = AUD_QUALITY_PROFESSIONAL;
    }

    if( Mix_OpenAudio( audioQuality, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ){
		return -1;
    }

    return 0;

}

void AUD_exit(){

    while(sfxCache!=NULL){

        lastCache = sfxCache;
        sfxCache = sfxCache->next;

        if(lastCache->sfx!=NULL){
            Mix_FreeChunk(lastCache->sfx);
        }

        if(lastCache!=NULL){
            SDL_free(lastCache);
        }

    }

    sfxCache = NULL;
    lastCache = NULL;

    while(musicCache!=NULL){

        mlastCache = musicCache;
        musicCache = musicCache->next;

        if(mlastCache->music!=NULL){
            Mix_FreeMusic(mlastCache->music);
        }

        if(mlastCache!=NULL){
            SDL_free(mlastCache);
        }

    }

    musicCache = NULL;
    mlastCache = NULL;

    Mix_CloseAudio();
    Mix_Quit();
}


AUD_Music* AUD_newMusic(const char* mp3OggFlac_File){

    Mix_Music *music = Mix_LoadMUS(mp3OggFlac_File);
    if(music==NULL){
        return NULL;
    }

    AUD_Music_MemDump *cache = SDL_malloc(sizeof(AUD_Music_MemDump));
    if(cache==NULL){
        Mix_FreeMusic(music);
        return NULL;
    }

    cache->next = NULL;
    cache->music = music;

    if(musicCache==NULL){
        musicCache = cache;
        mlastCache = cache;
        return (AUD_Music*)music;
    }

    mlastCache->next = cache;
    mlastCache = cache;

    return (AUD_Music*)music;
}

AUD_SFX* AUD_newSFX(const char* wavOggAiffRiffVOC_File){

    Mix_Chunk *sfx = Mix_LoadWAV(wavOggAiffRiffVOC_File);
    if(sfx==NULL){
        return NULL;
    }

    AUD_SFX_MemDump *cache = SDL_malloc(sizeof(AUD_SFX_MemDump));
    if(cache==NULL){
        Mix_FreeChunk(sfx);
        return NULL;
    }

    cache->next = NULL;
    cache->sfx = sfx;

    if(sfxCache==NULL){
        sfxCache = cache;
        lastCache = cache;
        return (AUD_SFX*)sfx;
    }

    lastCache->next = cache;

    return (AUD_SFX*)sfx;

}

void AUD_setSFXVolume(const int zero128){
    if(zero128>-1&&zero128<129){
        AUD_X_SFXvolume = zero128;
    }
}

void AUD_playSFX(AUD_SFX *sfx){
    if(sfx==NULL){
        return;
    }
    Mix_VolumeChunk(sfx,AUD_X_SFXvolume);
    Mix_PlayChannel( -1, sfx, 0 );
}

void AUD_playMusic(AUD_Music *music){
    if(music==NULL){
        return;
    }
    if(Mix_PlayingMusic()){
        Mix_FadeOutMusic(500);
    }
    Mix_FadeInMusic(music,0,500);
}
