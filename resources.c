#include "2DBase_defines.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "resources.h"
#include "events.h"
#include "audios.h"
#include "textury.h"
#include "xfonts.h"
#include "stringys.h"

static RES_ResourceState *RES_X_storedState = NULL;
static Uint32 RES_X_lastFrameStore=0;
static Uint32 RES_X_currentFrameStore=0;

RES_ResourceState *RES_NO_WINDOW = NULL;

MemoryDumpStaticLocals(RES_MemDump,RES_freeState,RES_ResourceState,RES_X_initDump,RES_X_freeDump,RES_X_addToDump,RES_X_removeFromDump);

static int RES_X_appendWindowState(RES_ResourceState *newState){
    if(newState==NULL){
        return -1;
    }
    int exists = 0;
    char ibuff[20];
    char *nbuff = NULL;
    int i = -1;
    void *data = NULL;
    do{
        i++;
        SDL_itoa(i,&(ibuff[0]),10);
        if(nbuff!=NULL){
            free(nbuff);
        }
        nbuff = STR_concat((char*)"state",&(ibuff[0]));
        data = SDL_GetWindowData(RES_window,nbuff);
        if(data==(void*)newState){
            exists = 1;
            break;
        }
        //SDL_Log("Window: %p   DATA: %p | %i     AT: %s\n",RES_window, data,i, nbuff);
    }while(data!=NULL);
    if(nbuff!=NULL){
        if(newState!=NULL&&!exists){
            SDL_SetWindowData(RES_window,nbuff,newState);
        }
        free(nbuff);
    }
    return i;
}

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
const Uint32 RES_RMASK = 0xff000000;
const Uint32 RES_GMASK = 0x00ff0000;
const Uint32 RES_BMASK = 0x0000ff00;
const Uint32 RES_AMASK = 0x000000ff;
#else
const Uint32 RES_RMASK = 0x000000ff;
const Uint32 RES_GMASK = 0x0000ff00;
const Uint32 RES_BMASK = 0x00ff0000;
const Uint32 RES_AMASK = 0xff000000;
#endif

const Uint32 RES_WHITE = 0xFFFFFF;
const Uint32 RES_BLACK = 0x000000;
const Uint32 RES_GREEN = 0x00FF00;
const Uint32 RES_RED = 0xFF0000;
const Uint32 RES_BLUE = 0x0000FF;
const Uint32 RES_YELLOW = 0xFFFF00;
const Uint32 RES_MAGENTA = 0xFFFF00;
const Uint32 RES_CYAN = 0x00FFFF;

SDL_Event RES_event;
SDL_Window *RES_window = NULL;
SDL_Surface *RES_screen = NULL;
SDL_Renderer *RES_renderer = NULL;
SDL_Texture *RES_texture = NULL;
volatile Uint32 *RES_pixels = NULL;

Uint32 RES_SCREEN_WIDTH = 800;
Uint32 RES_SCREEN_HEIGHT = 600;

Uint8 RES_running = 1;
Uint8 RES_FPS = 0;
Uint8 RES_dirtyRenderer = 0;

int RES_initSDL(){
    if(!SDL_WasInit(SDL_INIT_EVERYTHING)){
#if WIN32
        SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
#endif // WIN32
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
            RES_exitProcessErr("Could not initialize SDL");
        }
        if(AUD_init(BASE_AUDIO_QUALITY)){
            printf("Could not initialize SDL_Mixer Audio\n");
        }
        if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF /*|IMG_INIT_WEBP*/) == -1){
            printf("Could not initialize IMG loader\n");
        }
    }
    return 0;
}

int RES_init(){
    RES_initSDL();
    TEX_init();
    FNT_init();
    //#if ALLOW_RESOURCES_MEMORY_DUMP
    RES_X_initDump();
    //#endif
    return 0;
}

void RES_exitProcessDef(const char *msg){
    printf("%s\n", msg);
    exit(0);
}

void RES_exitProcessErr(const char *msg){
    printf("%s\n", msg);
    exit(-1);
}

void RES_exit(){
    RES_cleanup();
    TEX_exit();
    AUD_exit();
    FNT_exit();

    SDL_Quit();
}

void RES_cleanup(){
    RES_running = 0;
    //#if ALLOW_RESOURCES_MEMORY_DUMP
    RES_X_freeDump();
    //#endif
    //quit SDL
    //SDL_Quit();
}









int RES_initResources(){
    // create screen surface
    RES_screen = SDL_CreateRGBSurface(0, RES_SCREEN_WIDTH, RES_SCREEN_HEIGHT,
                                      32, RES_RMASK, RES_GMASK, RES_BMASK, RES_AMASK);
    RES_texture = SDL_CreateTexture(RES_renderer, SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING, RES_SCREEN_WIDTH, RES_SCREEN_HEIGHT);
    SDL_SetRenderTarget(RES_renderer, RES_texture);
    //SDL_SetTextureBlendMode(screen_texture,SDL_BLENDMODE_BLEND);
    //SDL_SetTextureAlphaMod(screen_texture,255);
    SDL_SetSurfaceBlendMode(RES_screen, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(RES_screen, 255);
    return 0;
}

int RES_initWindow(const int SDL_WINDOW_FLAGS, const int SDL_RENDERER_FLAGS){
    RES_window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  RES_SCREEN_WIDTH, RES_SCREEN_HEIGHT,
                                  //SDL_WINDOW_RESIZABLE
                                  //|SDL_WINDOW_SHOWN
                                  //| SDL_WINDOW_HIDDEN
                                  SDL_WINDOW_FLAGS
                                 );
    RES_renderer = SDL_CreateRenderer(RES_window, -1, SDL_RENDERER_FLAGS/*SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE*/);
    SDL_SetTextureBlendMode(RES_texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(RES_renderer, SDL_BLENDMODE_BLEND);
    ///SDL_ShowWindow(RES_window);
    return 0;
}






void RES_removeStateFromDump(RES_ResourceState *state){
    if(state==NULL){
        return;
    }

    /*RES_MemDump *loop = DMP_X_X_first;
    while(loop!=NULL){
        SDL_Log("PREEXISTS: %p\n",loop->leaf);
        loop=loop->node;
    }
    SDL_Log("PRE_FirstValue: %p\n",DMP_X_X_first);
    SDL_Log("ATTEMPT_REMOVE: %p\n",state);*/

    RES_X_removeFromDump(state);
    if(DMP_X_X_first==NULL){
        RES_running = 0;
    }
    /*SDL_Log("FirstValue: %p\n",DMP_X_X_first);
    loop = DMP_X_X_first;
    while(loop!=NULL){
            SDL_Log("EXISTS: %p\n",loop->leaf);
        loop=loop->node;

    }*/
}

RES_ResourceState* RES_saveState(){
    int exists = 0;
    RES_ResourceState *curState = NULL;
    if(RES_X_storedState!=NULL&&RES_X_storedState->window==RES_window){
        exists = 1;
    }
    if(exists){
        curState = RES_X_storedState;
    }else{
        if(RES_window==NULL||RES_renderer==NULL){
            return NULL;
        }
        curState = calloc(sizeof(RES_ResourceState),1);
    }
    curState->FPS = RES_FPS;
    curState->renderer = RES_renderer;
    curState->running = RES_running;
    curState->screen = RES_screen;
    curState->SCREEN_HEIGHT = RES_SCREEN_WIDTH;
    curState->SCREEN_HEIGHT = RES_SCREEN_HEIGHT;
    curState->texture = RES_texture;
    curState->window = RES_window;
#if ALLOW_AUTO_DETECT_RENDER
    curState->dirtyRenderer = RES_dirtyRenderer;
#endif // ALLOW_AUTO_DETECT_RENDER
    if(!exists){
        RES_X_appendWindowState(curState);
        RES_X_addToDump(curState);
    }
    RES_X_storedState = curState;
    return curState;
}

RES_ResourceState* RES_newStateX(const char *title, const int width, const int height, const int SDL_WINDOW_FLAGS, const int SDL_RENDERER_FLAGS){

    int windowFlags = SDL_WINDOW_FLAGS;
    int rendererFlags = SDL_RENDERER_FLAGS;

    if(windowFlags==0){
        windowFlags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
    }

    if(rendererFlags==0){
        rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
    }

    RES_ResourceState *revert = NULL;
    int needRevert = 0;
    needRevert = 0;
    revert = RES_saveState();

    RES_ResourceState *newState = NULL;

    if(RES_initWindow(windowFlags, rendererFlags)){
        SDL_Log("Could not Initialize SDL Window [Full]\n");
        needRevert = 1;
    }else{
        SDL_SetWindowSize(RES_window,width,height);
        SDL_SetWindowTitle(RES_window,title);
        newState = RES_saveState();
        RES_X_appendWindowState(newState);
    }
    if(RES_initResources()){
        SDL_Log("Could not Initialize SDL [Full]\n");
        needRevert = 1;
    }

    if(needRevert==1){
        if(revert!=NULL){
            RES_loadState(revert);
        }else if(RES_X_storedState!=NULL){
            RES_loadState(RES_X_storedState);
        }
        RES_freeState(newState);
        free(newState);
        newState = NULL;
    }else{
        RES_X_addToDump(newState);
        RES_X_storedState = newState;
    }

    RES_saveState();

    //
    return newState;
}

void RES_loadState(RES_ResourceState *windowState){
#if ALLOW_AUTO_DETECT_RENDER
    if(RES_X_storedState!=NULL){
        RES_X_storedState->dirtyRenderer = RES_dirtyRenderer;
    }
#endif // ALLOW_AUTO_DETECT_RENDER
    int w = 0;
    int h = 0;
    SDL_GetWindowSize(windowState->window,&w,&h);
    RES_SCREEN_WIDTH = (Uint32)w;
    RES_SCREEN_HEIGHT = (Uint32)h;
    RES_FPS = windowState->FPS;
    RES_renderer = windowState->renderer;
    RES_running = windowState->running;
    RES_screen = windowState->screen;
    RES_texture = windowState->texture;
    RES_window = windowState->window;
    RES_X_storedState = windowState;
}

void RES_attachState(RES_ResourceState **namedStatePointerLocation){
    (*namedStatePointerLocation)->storedLocation = namedStatePointerLocation;
}

void RES_freeState(RES_ResourceState *windowState){
    if(windowState==NULL){
        return;
    }
    ///free randoms
    ///TEX_exit(); TODO go through and remove anything that matches the RES_renderer pointer
    TEX_freeTexturesByRenderer(windowState->renderer);
    // free image
    if(windowState->screen != NULL){
        SDL_FreeSurface(windowState->screen);
    }
    if(windowState->texture != NULL){
        SDL_DestroyTexture(windowState->texture);
    }
    if(windowState->renderer != NULL){
        SDL_DestroyRenderer(windowState->renderer);
    }
    if(windowState->window != NULL){
        SDL_HideWindow(windowState->window);
        SDL_DestroyWindow(windowState->window);
        windowState->window=NULL;
    }

    if(windowState->storedLocation!=NULL){
        windowState->storedLocation = NULL;
        //printf("KILLED: %p   VALUE: %p\n",&windowState->storedLocation,windowState->storedLocation);
    }
}












void RES_updateWindowTextureFromSurfaceToTexture(SDL_Texture *screen_texture, SDL_Surface *screen, SDL_Renderer *renderer){
    //update screen texture
    SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->pitch);
    // clear renderer
    SDL_RenderClear(renderer);
    // copy image into renderer to be rendered
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
    // render image
    SDL_RenderPresent(renderer);
}

void RES_updateWindow(){
    //update screen texture
    ///SDL_UpdateTexture(RES_screen, NULL, RES_screen->pixels, RES_screen->pitch);
    // clear renderer
    ///SDL_RenderClear(RES_renderer);
    // copy image into renderer to be rendered
    ///SDL_RenderCopy(RES_renderer, RES_screen, NULL, NULL);
    // render image
    SDL_SetRenderTarget(RES_renderer, NULL);
    SDL_RenderPresent(RES_renderer);
    SDL_SetRenderTarget(RES_renderer, RES_texture);
}

void RES_resizeScreen(){
    // free image
    SDL_FreeSurface(RES_screen);
    // free image texture
    SDL_DestroyTexture(RES_texture);
    // create screen surface
    RES_screen = SDL_CreateRGBSurface(0, RES_SCREEN_WIDTH, RES_SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    // create screen texture
    RES_texture = SDL_CreateTexture(RES_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RES_SCREEN_WIDTH, RES_SCREEN_HEIGHT);
    //SDL_SetTextureBlendMode(screen_texture,SDL_BLENDMODE_BLEND);
    //SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceBlendMode(RES_screen, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(RES_screen, 255);
    SDL_SetTextureAlphaMod(RES_texture, 100);
    printf(">>Resized Display to: %d, %d\n", RES_SCREEN_WIDTH, RES_SCREEN_HEIGHT);
}

void RES_resizeScreen2(const int width, const int height){
    // free image
    SDL_FreeSurface(RES_screen);
    // free image texture
    SDL_DestroyTexture(RES_texture);
    RES_SCREEN_WIDTH = width;
    RES_SCREEN_HEIGHT = height;
    // create screen surface
    RES_screen = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // create screen texture
    RES_texture = SDL_CreateTexture(RES_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    SDL_SetSurfaceBlendMode(RES_screen, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(RES_screen, 255);
    SDL_SetTextureAlphaMod(RES_texture, 100);
    printf(">>Resized Display to: %d, %d\n", width, height);
}

void RES_setColor(const int r, const int g, const int b, const int a){
    SDL_SetRenderDrawColor(RES_renderer, r, g, b, a);
}
void RES_setColorRGBA(const int rgba){
    RES_setColor((rgba >> 24) & 0xFF, (rgba >> 16) & 0xFF, (rgba >> 8) & 0xFF, rgba & 0xFF);
}
void RES_setColorARGB(const int argb){
    RES_setColor((argb >> 16) & 0xFF, (argb >> 8) & 0xFF, argb & 0xFF, (argb >> 24) & 0xFF);
}
void RES_drawRect(const int x, const int y, const int w, const int h){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_RenderDrawRect(RES_renderer, &r);
}
void RES_drawRect2(const SDL_Rect *rect){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_RenderDrawRect(RES_renderer, rect);
}
void RES_fillRect(const int x, const int y, const int w, const int h){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_RenderFillRect(RES_renderer, &r);
}
void RES_fillRect2(const SDL_Rect *rect){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_RenderFillRect(RES_renderer, rect);
}


void RES_drawImageRect(const TEX_XtraTexture *img, const SDL_Rect *src_clip, const SDL_Rect *dest_place){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_RenderCopy(RES_renderer, img->img, src_clip, dest_place);
}

void RES_drawImageAt(const TEX_XtraTexture *img, const int x, const int y){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = img->w;
    r.h = img->h;
    SDL_RenderCopy(RES_renderer, img->img, &img->clip_rect, &r);
}

void RES_drawImageScaledAt(const TEX_XtraTexture *img, const int x, const int y, const int sw, const int sh){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = sw;
    r.h = sh;
    SDL_RenderCopy(RES_renderer, img->img, &img->clip_rect, &r);
}

void RES_drawImageSectionAt(const TEX_XtraTexture *img, const int x, const int y, const int cx, const int cy, const int cw, const int ch){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_Rect s, r;
    r.x = x;
    r.y = y;
    r.w = cw;
    r.h = ch;
    s.x = cx;
    s.y = cy;
    s.w = cw;
    s.h = ch;
    SDL_RenderCopy(RES_renderer, img->img, &s, &r);
}

void RES_drawImageSectionScaledAt(const TEX_XtraTexture *img, const int x, const int y, const int sw, const int sh, const int cx, const int cy, const int cw, const int ch){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_Rect s, r;
    r.x = x;
    r.y = y;
    r.w = sw;
    r.h = sh;
    s.x = cx;
    s.y = cy;
    s.w = cw;
    s.h = ch;
    SDL_RenderCopy(RES_renderer, img->img, &s, &r);
}



void RES_drawTextureAt(SDL_Texture *img, const int x, const int y){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_Rect r;
    r.x = x;
    r.y = y;
    SDL_QueryTexture(img,NULL,NULL,&(r.w),&(r.h));
    SDL_RenderCopy(RES_renderer, img, NULL, &r);
}

void RES_drawTextureScaledAt(SDL_Texture *img, const int x, const int y, const int sw, const int sh){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = sw;
    r.h = sh;
    SDL_RenderCopy(RES_renderer, img, NULL, &r);
}

void RES_drawTextureSectionAt(SDL_Texture *img, const int x, const int y, const int cx, const int cy, const int cw, const int ch){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_Rect s, r;
    r.x = x;
    r.y = y;
    r.w = cw;
    r.h = ch;
    s.x = cx;
    s.y = cy;
    s.w = cw;
    s.h = ch;
    SDL_RenderCopy(RES_renderer, img, &s, &r);
}

void RES_drawTextureSectionScaledAt(SDL_Texture *img, const int x, const int y, const int sw, const int sh, const int cx, const int cy, const int cw, const int ch){
#if ALLOW_AUTO_DETECT_RENDER
    RES_dirtyRenderer = 1;
#endif // ALLOW_AUTO_DETECT_RENDER
    SDL_Rect s, r;
    r.x = x;
    r.y = y;
    r.w = sw;
    r.h = sh;
    s.x = cx;
    s.y = cy;
    s.w = cw;
    s.h = ch;
    SDL_RenderCopy(RES_renderer, img, &s, &r);
}




int RES_inRect(const int x, const int y, const int w, const int h, const int pointx, const int pointy){
    return pointx > x && pointx < x + w && pointy > y && pointy < y + h;
}








void RES_updateAllWindowStates(){
    RES_ResourceState *current = RES_saveState();

    RES_MemDump *loop = DMP_X_X_first;
    while(loop!=NULL){
        if(loop!=NULL&&loop->leaf!=NULL){
            if(loop->leaf->window!=NULL&&loop->leaf->renderer!=NULL&&loop->leaf->texture!=NULL){
#if ALLOW_AUTO_DETECT_RENDER
                if(loop->leaf->dirtyRenderer!=0){
                    RES_loadState(loop->leaf);
                    RES_dirtyRenderer = 0;
                    RES_updateWindow();loop->leaf->dirtyRenderer = 0;
                    printf("UPDAED\n");
                }
#else
                RES_loadState(loop->leaf);
                RES_updateWindow();
#endif // ALLOW_AUTO_DETECT_RENDER
            }
        }
        loop = loop->node;
    }

    RES_loadState(current);
}

void RES_setFPS(const int fps){
    if(fps < 1){
        return;
    }
    RES_FPS = fps;
}

void RES_mainLoop(void (*processCallback)(void)){
    RES_running = 1;
    if(RES_FPS == 0){
        RES_FPS = 25;
    }
    Uint32 last = SDL_GetTicks();
    Uint32 store;
    while(RES_running){
        last = SDL_GetTicks();
        EVT_EventHandler(&EVT_event, RES_window);

        RES_updateAllWindowStates();

        if(processCallback != NULL){
            processCallback();
        }
        RES_updateWindow();
        store = SDL_GetTicks();
        if((store - last) < (1000 / RES_FPS) + 1){
            SDL_Delay((1000 / RES_FPS) - (store - last));
        }
    }
}

void RES_pollingEventLoop(){
    EVT_EventHandler(&EVT_event, RES_window);
}

void RES_pollingGraphicsLoop(){

    RES_updateAllWindowStates();

    RES_X_currentFrameStore = SDL_GetTicks();
    if((RES_X_currentFrameStore - RES_X_lastFrameStore) < (1000 / RES_FPS) + 1){
        SDL_Delay((1000 / RES_FPS) - (RES_X_currentFrameStore - RES_X_lastFrameStore));
    }

    RES_X_lastFrameStore = SDL_GetTicks();

}
