#ifndef RESOURCES_H_INCLUDED
#define RESOURCES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "memorys.h"
#include "textury.h"

#define RES_setWindowTitle(title) {SDL_SetWindowTitle(RES_window,title);}
#define RES_setWindowSize(w, h) {SDL_SetWindowSize(RES_window,w,h);}
#define RES_centerWindow() {SDL_SetWindowPosition(RES_window,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);}
#define RES_showWindow() {SDL_ShowWindow(RES_window);}
#define RES_hideWindow() {SDL_HideWindow(RES_window);}

SDL_Surface *RES_fontRAW;
SDL_Texture *RES_font;
SDL_Texture *RES_fontWhite;

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

int RES_init();
int RES_initFull();

int RES_initWindow();
/*void RES_setWindowTitle();
void RES_setWindowSize();
void RES_centerWindow();
void RES_snapWindowTo();
void RES_showWindow();*/

int RES_initResources();

void RES_exitDef(const char *msg);
void RES_exitErr(const char *msg);

void RES_cleanup();
void RES_exit();
void RES_updateWindow();
void RES_resizeScreen();
void RES_resizeScreen2(const int width, const int height);
void RES_updateWindowTextureFromSurfaceToTexture(SDL_Texture *screen_texture, SDL_Surface *screen, SDL_Renderer *renderer);

void RES_setColor(const int r, const int g, const int b, const int a);
void RES_setColorInt(const int rgba);

void RES_drawRect(const int x, const int y, const int w, const int h);
void RES_fillRect(const int x, const int y, const int w, const int h);

void RES_drawImageRect(const XtraTexture *img, const SDL_Rect *src_clip, const SDL_Rect *dest_place);

void RES_drawImageAt(const XtraTexture *img, const int x, const int y);
void RES_drawImageScaledAt(const XtraTexture *img, const int x, const int y, const int sw, const int sh);
void RES_drawImageSectionAt(const XtraTexture *img, int x, int y, int cx, int cy, int cw, int ch);
void RES_drawImageSectionScaledAt(const XtraTexture *img, const int x, const int y, const int sw, const int sh, const int cx, const int cy, const int cw, const int ch);

void RES_setStringColor(const int r, const int g, const int b, const int a);
void RES_setStringColorInt(const int rgba);
void RES_drawString(const char *txt, const int pt, int x, const int y);
void RES_drawStringWhite(const char *txt, const int pt, int x, const int y);
int RES_inRect(const int x, const int y, const int w, const int h, const int pointx, const int pointy);



void RES_setFPS(int fps);
///consumes thread, nearly infinite loop
void RES_mainLoop(void (*gameProcessCallback)(void));




int RES_initAudio();
/*int RES_thread_BGM_num;
char RES_thread_BGM_args[][64];*/

#endif // RESOURCES_H_INCLUDED
