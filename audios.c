#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "audios.h"

int AUD_SFXvolume = 100;

static AUD_SFXCache *sfxCache = NULL;
static AUD_SFXCache *lastCache = NULL;

static AUD_MusicCache *musicCache = NULL;
static AUD_MusicCache *mlastCache = NULL;

int AUD_initAudio(int audioQuality){
    int flags=MIX_INIT_OGG|MIX_INIT_FLAC|MIX_INIT_MP3;
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


AUD_Music* AUD_newMusic(char* mp3OggFlac){

    Mix_Music *music = Mix_LoadMUS(mp3OggFlac);
    if(music==NULL){
        return NULL;
    }

    AUD_MusicCache *cache = SDL_malloc(sizeof(AUD_MusicCache));
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

AUD_SFX* AUD_newSFX(char* wavOggAiffRiffVOC){

    Mix_Chunk *sfx = Mix_LoadWAV(wavOggAiffRiffVOC);
    if(sfx==NULL){
        return NULL;
    }

    AUD_SFXCache *cache = SDL_malloc(sizeof(AUD_SFXCache));
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

void AUD_setSFXVolume(int zero128){
    if(zero128>-1&&zero128<129){
        AUD_SFXvolume = zero128;
    }
}

void AUD_playSFX(AUD_SFX *sfx){
    if(sfx==NULL){
        return;
    }
    Mix_VolumeChunk(sfx,AUD_SFXvolume);
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
