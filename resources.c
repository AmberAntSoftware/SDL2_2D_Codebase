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

const Uint32 RES_WHITE = 0xFFFFFFFF;
const Uint32 RES_BLACK = 0x000000FF;
const Uint32 RES_GREEN = 0x00FF00FF;
const Uint32 RES_RED = 0xFF0000FF;
const Uint32 RES_BLUE = 0x0000FFFF;
const Uint32 RES_YELLOW = 0xFFFF00FF;
const Uint32 RES_MAGENTA = 0xFFFF00FF;
const Uint32 RES_CYAN = 0x00FFFFFF;

SDL_Event RES_event;
SDL_Window *RES_window;
SDL_Surface *RES_screen;
SDL_Renderer *RES_renderer;
SDL_Texture *RES_texture;
volatile Uint32 *RES_pixels;

Uint32 RES_SCREEN_WIDTH = 800;
Uint32 RES_SCREEN_HEIGHT = 600;

Uint8 RES_running = 1;
Uint8 RES_FPS = 0;

int RES_initSDL(){
    if(!SDL_WasInit(SDL_INIT_EVERYTHING)){
#if WIN32
        SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
#endif // WIN32
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
            RES_exitErr("Could not initialize SDL");
        }
        if(AUD_initAudio(AUD_QUALITY_GAME)){
            printf("Could not initialize SDL_Mixer Audio\n");
        }
        if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF/*|IMG_INIT_WEBP*/) == -1){
            printf("Could not initialize IMG loader\n");
        }
    }
    return 0;
}

#define RES_initFull RES_init
int RES_init(){
    RES_initSDL();
    TEX_init();
    FNT_init();
    if(RES_initWindow()){
        RES_exitErr("Could not Initialize SDL [Full]");
        return -1;
    }
    if(RES_initResources()){
        RES_exitErr("Could not Initialize SDL [Full]");
        return -1;
    }
    return 0;
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
    /*RES_fontRAW = IMG_Load("5bit_font_clear.png");
    RES_font = SDL_CreateTextureFromSurface(RES_renderer, RES_fontRAW);
    if(RES_fontRAW==NULL||RES_font==NULL){
        printf("Could not initialize Default Font\n");
    }

    RES_fontRAW = IMG_Load("5bit_font_clear_white.png");
    RES_fontWhite = SDL_CreateTextureFromSurface(RES_renderer, RES_fontRAW);
    if(RES_fontRAW==NULL||RES_fontWhite==NULL){
        printf("Could not initialize Default Font\n");
    }*/
    return 0;
}

int RES_initWindow(){
    RES_window = SDL_CreateWindow("Window 0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  RES_SCREEN_WIDTH, RES_SCREEN_HEIGHT,
                                  SDL_WINDOW_RESIZABLE
                                  //|SDL_WINDOW_SHOWN
                                  | SDL_WINDOW_HIDDEN
                                 );
    RES_renderer = SDL_CreateRenderer(RES_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    SDL_SetTextureBlendMode(RES_texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(RES_renderer, SDL_BLENDMODE_BLEND);
    ///SDL_ShowWindow(RES_window);
    return 0;
}




void RES_exitDef(const char *msg){
    printf("%s\n", msg);
    exit(0);
}

void RES_exitErr(const char *msg){
    printf("%s\n", msg);
    exit(-1);
}

void RES_exit(){
    RES_cleanup();
    AUD_exit();
    RES_running = 0;
    MEM_exit();
    SDL_Quit();
}

void RES_cleanup(){
    //free randoms
    TEX_exit();
    // free image
    if(RES_screen != NULL){
        SDL_FreeSurface(RES_screen);
    }
    if(RES_fontRAW != NULL){
        SDL_FreeSurface(RES_fontRAW);
    }
    // free image texture
    if(RES_texture != NULL){
        SDL_DestroyTexture(RES_texture);
    }
    if(RES_font != NULL){
        SDL_DestroyTexture(RES_font);
    }
    // free renderer
    if(RES_renderer != NULL){
        SDL_DestroyRenderer(RES_renderer);
    }
    // free window
    if(RES_window != NULL){
        SDL_DestroyWindow(RES_window);
    }
    //quit SDL
    //SDL_Quit();
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
void RES_setColorInt(const int rgba){
    SDL_SetRenderDrawColor(RES_renderer, (rgba >> 24) & 0xFF, (rgba >> 16) & 0xFF, (rgba >> 8) & 0xFF, rgba & 0xFF);
}
void RES_drawRect(const int x, const int y, const int w, const int h){
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_RenderDrawRect(RES_renderer, &r);
}
void RES_fillRect(const int x, const int y, const int w, const int h){
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_RenderFillRect(RES_renderer, &r);
}


void inline RES_drawImageRect(const TEX_XtraTexture *img, const SDL_Rect *src_clip, const SDL_Rect *dest_place){
    SDL_RenderCopy(RES_renderer, img->img, src_clip, dest_place);
}

void RES_drawImageAt(const TEX_XtraTexture *img, const int x, const int y){
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = img->w;
    r.h = img->h;
    SDL_RenderCopy(RES_renderer, img->img, &img->clip_rect, &r);
}

void RES_drawImageScaledAt(const TEX_XtraTexture *img, const int x, const int y, const int sw, const int sh){
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = sw;
    r.h = sh;
    SDL_RenderCopy(RES_renderer, img->img, &img->clip_rect, &r);
}

void RES_drawImageSectionAt(const TEX_XtraTexture *img, const int x, const int y, const int cx, const int cy, const int cw, const int ch){
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
    SDL_Rect r;
    r.x = x;
    r.y = y;
    SDL_QueryTexture(img,NULL,NULL,&(r.w),&(r.h));
    SDL_RenderCopy(RES_renderer, img, NULL, &r);
}

void RES_drawTextureScaledAt(SDL_Texture *img, const int x, const int y, const int sw, const int sh){
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = sw;
    r.h = sh;
    SDL_RenderCopy(RES_renderer, img, NULL, &r);
}

void RES_drawTextureSectionAt(SDL_Texture *img, const int x, const int y, const int cx, const int cy, const int cw, const int ch){
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



void RES_drawString(const char *txt, const int pt, int x, int y){
    SDL_Rect ltr;
    ltr.x = 0;
    ltr.y = 0;
    ltr.w = 5;
    ltr.h = 5;
    SDL_Rect whr;
    whr.x = x;
    whr.y = y;
    whr.w = pt;
    whr.h = pt;
    Uint32 length = SDL_strlen(txt), i;
    for(i = 0; i < length; i++){
        ltr.y = 5 * (txt[i] & 0xFF);
        SDL_RenderCopy(RES_renderer, RES_font, &ltr, &whr);
        //SDL_RenderCopy()
        whr.x += pt + pt / 10;
    }
}

void RES_drawStringWhite(const char *txt, const int pt, int x, int y){
    SDL_Rect ltr;
    ltr.x = 0;
    ltr.y = 0;
    ltr.w = 5;
    ltr.h = 5;
    SDL_Rect whr;
    whr.x = x;
    whr.y = y;
    whr.w = pt;
    whr.h = pt;
    Uint32 length = SDL_strlen(txt), i;
    for(i = 0; i < length; i++){
        ltr.y = 5 * (txt[i] & 0xFF);
        SDL_RenderCopy(RES_renderer, RES_fontWhite, &ltr, &whr);
        //SDL_RenderCopy()
        whr.x += pt + pt / 10;
    }
}

int RES_inRect(const int x, const int y, const int w, const int h, const int pointx, const int pointy){
    return pointx > x && pointx < x + w && pointy > y && pointy < y + h;
}



void RES_setFPS(int fps){
    if(fps > 999){
        fps = 1000;
    }
    if(fps < 0){
        return;
    }
    RES_FPS = fps;
}

void RES_mainLoop(void (*gameProcessCallback)(void)){
    RES_running = 1;
    if(RES_FPS == 0){
        RES_FPS = 25;
    }
    Uint32 last = SDL_GetTicks();
    Uint32 store;
    while(RES_running){
        last = SDL_GetTicks();
        EVT_EventHandler(&EVT_event, RES_window);
        if(gameProcessCallback != NULL){
            gameProcessCallback();
        }
        RES_updateWindow();
        store = SDL_GetTicks();
        if((store - last) < (1000 / RES_FPS) + 1){
            SDL_Delay((1000 / RES_FPS) - (store - last));
        }
    }
}
