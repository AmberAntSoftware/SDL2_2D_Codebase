#ifndef RESOURCES_H_INCLUDED
#define RESOURCES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "textury.h"
#include "preutils.h"

///Temporary backwards compatibilities
#define RES_initFull RES_init




#define RES_setWindowTitle(title) SDL_SetWindowTitle(RES_window,title)
#define RES_setWindowSize(w, h) SDL_SetWindowSize(RES_window,w,h)
#define RES_centerWindow() SDL_SetWindowPosition(RES_window,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED)
#define RES_showWindow() SDL_ShowWindow(RES_window)
#define RES_hideWindow() SDL_HideWindow(RES_window)
#define RES_drawLine(x1,y1,x2,y2) SDL_RenderDrawLine(RES_renderer,x1,y1,x2,y2)

#define RES_drawRectR(rectPointer) SDL_RenderFillRect(RES_renderer, rectPointer)
#define RES_fillRectR(rectPointer) SDL_RenderFillRect(RES_renderer, rectPointer)

#define RES_inRectR(rectPointer, pointx, pointy) RES_inRect(rectPointer->x, rectPointer->y, rectPointer->w, rectPointer->h pointx,pointy)
#define RES_drawImageScaledAtR(imgPtr, rectPointer) RES_drawImageScaledAt(imgPtr, rectPointer->x, rectPointer->y, rectPointer->w, rectPointer->h)
#define RES_drawTextureScaledAtR(texPtr, rectPointer) RES_drawTextureScaledAt(texPtr, rectPointer->x, rectPointer->y, rectPointer->w, rectPointer->h)




MemoryDumpStrucure(RES_MemDump,RES_ResourceState);

typedef struct RES_ResourceState {

    SDL_Window *window;
    SDL_Surface *screen;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    Uint32 SCREEN_WIDTH;
    Uint32 SCREEN_HEIGHT;

    Uint8 running;
    Uint8 FPS;

} RES_ResourceState;
#define RES_isStateUsable(RES_ResourceStatePtr) (RES_ResourceStatePtr!=NULL && RES_ResourceStatePtr->window!=NULL)



#define RGBA(RGB) ((num<<8)|0xFF)
#define ARGB(RGB) (0xFFFFFFFF|num)
const Uint32 RES_WHITE;
const Uint32 RES_BLACK;
const Uint32 RES_GREEN;
const Uint32 RES_RED;
const Uint32 RES_BLUE;
const Uint32 RES_YELLOW;
const Uint32 RES_MAGENTA;
const Uint32 RES_CYAN;

const Uint32 RES_RMASK;
const Uint32 RES_GMASK;
const Uint32 RES_BMASK;
const Uint32 RES_AMASK;

SDL_Window *RES_window;
SDL_Surface *RES_screen;
SDL_Renderer *RES_renderer;
SDL_Texture *RES_texture;
volatile Uint32 *RES_pixels;

Uint32 RES_SCREEN_WIDTH;
Uint32 RES_SCREEN_HEIGHT;

Uint8 RES_running;
Uint8 RES_FPS;

Uint32 RES_currentWindow;

int RES_init();
int RES_initFull();

int RES_initWindow();
/*void RES_setWindowTitle();
void RES_setWindowSize();
void RES_centerWindow();
void RES_snapWindowTo();
void RES_showWindow();*/

int RES_initResources();

void RES_exitProcessDef(const char *msg);
void RES_exitProcessErr(const char *msg);

void RES_cleanup();
void RES_exit();
void RES_updateWindow();
void RES_resizeScreen();
void RES_resizeScreen2(const int width, const int height);
void RES_updateWindowTextureFromSurfaceToTexture(SDL_Texture *screen_texture, SDL_Surface *screen, SDL_Renderer *renderer);

void RES_setColor(const int r, const int g, const int b, const int a);
void RES_setColorRGBA(const int rgba);
void RES_setColorARGB(const int argb);

void RES_drawRect(const int x, const int y, const int w, const int h);
void RES_fillRect(const int x, const int y, const int w, const int h);
void RES_drawRect2(const SDL_Rect *rect);
void RES_fillRect2(const SDL_Rect *rect);

void RES_drawImageRect(const TEX_XtraTexture *img, const SDL_Rect *src_clip, const SDL_Rect *dest_place);

void RES_drawImageAt(const TEX_XtraTexture *img, const int x, const int y);
void RES_drawImageScaledAt(const TEX_XtraTexture *img, const int x, const int y, const int sw, const int sh);
void RES_drawImageSectionAt(const TEX_XtraTexture *img, const int x, const int y, const int cx, const int cy, const int cw, const int ch);
void RES_drawImageSectionScaledAt(const TEX_XtraTexture *img, const int x, const int y, const int sw, const int sh, const int cx, const int cy, const int cw, const int ch);

void RES_drawTextureAt(SDL_Texture *img, const int x, const int y);
void RES_drawTextureScaledAt(SDL_Texture *img, const int x, const int y, const int sw, const int sh);
void RES_drawTextureSectionAt(SDL_Texture *img, const int x, const int y, const int cx, const int cy, const int cw, const int ch);
void RES_drawTextureSectionScaledAt(SDL_Texture *img, const int x, const int y, const int sw, const int sh, const int cx, const int cy, const int cw, const int ch);

int RES_inRect(const int x, const int y, const int w, const int h, const int pointx, const int pointy);



void RES_removeStateFromDump(RES_ResourceState *state);
RES_ResourceState* RES_saveState();
RES_ResourceState* RES_newState(const char *title, const int width, const int height);
void RES_loadState(RES_ResourceState *windowState);
void RES_freeState(RES_ResourceState *windowState);



void RES_updateAllWindowStates();

void RES_setFPS(const int fps);
///consumes thread, nearly infinite loop
void RES_mainLoop(void (*processCallback)(void));
///meant to be put inside of an external loop
///used for frame delay, event handling, and updating the screen
void RES_pollingEventLoop();
void RES_pollingGraphicsLoop();



//int RES_initAudio();
/*int RES_thread_BGM_num;
char RES_thread_BGM_args[][64];*/

#endif // RESOURCES_H_INCLUDED
