#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/*
SDL_Thread* RES_thread_BGM;
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
