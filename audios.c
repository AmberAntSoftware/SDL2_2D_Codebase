#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/*
int RES_thread_BGM_num = 0;
char RES_thread_BGM_args[][64] = {
    "res/audio/ssgmlg_bgm1.ogg","res/audio/air canada 143_1.ogg",
    "res/audio/mgs_ep_sfx.ogg","res/audio/ambient_wind.ogg",
    "res/audio/song026.ogg"};
SDL_Thread* RES_thread_BGM;
//char* RES_thread_SFX_args;
SDL_Thread* RES_thread_SFX;
//*/
int AUD_initAudio(){
    // load support for the OGG, FLAC, MP3 sample/music formats
    //FIXME
    int flags=MIX_INIT_OGG|MIX_INIT_FLAC|MIX_INIT_MP3;
    int initted=Mix_Init(flags);
    if((initted&flags) != flags){
        printf("Mix_Init: Failed to init required ogg, flac, mp3 support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
        // handle error
        //RES_exitErr("Could not initialize SDL_Mixer");
    }

    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
		return -1;

    //RES_initBGM();

    return 0;

}
/*
void RES_switchMusic(int mus){
    RES_thread_BGM_num=mus;
    Mix_HaltMusic();
}

int RES_ThreadBGM(){
    static pre = 0;
    pre = RES_thread_BGM_num;

    while(RES_running){
        if(RES_thread_BGM_num+1){//if negative 1, escape, no sound
            Mix_Music* music = Mix_LoadMUS(RES_thread_BGM_args[RES_thread_BGM_num]);
            if (music == NULL)
                return -1;
            if ( Mix_PlayMusic( music, 1) == -1)
                return -1;
            while ( Mix_PlayingMusic()){
                SDL_Delay(5);
            }
            if(music){
                Mix_FreeMusic(music);
            }
        }else SDL_Delay(5);
    }
    Mix_HaltMusic();
    return 0;
}

int RES_ThreadSFX(){
    while(RES_running){
        SDL_Delay(5);
    }
    return 0;
}

void RES_initBGM(){
    RES_thread_BGM = SDL_CreateThread(RES_ThreadBGM,"SCBD_bgm",NULL);
}

void RES_initSFX(){
    RES_thread_BGM = SDL_CreateThread(RES_ThreadSFX,"SCBD_sfx",NULL);
}
*/
