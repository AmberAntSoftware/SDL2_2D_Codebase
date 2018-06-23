#define MAX_EVENT_CACHE 1024

#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Event EVT_event;
Uint8 EVT_key[MAX_EVENT_CACHE];
Uint8 EVT_btn[MAX_EVENT_CACHE];
Uint8 EVT_mov[MAX_EVENT_CACHE];
Sint32 EVT_mx;
Sint32 EVT_my;
Sint32 EVT_px;
Sint32 EVT_py;
Sint32 EVT_scroll;
Sint32 EVT_lastKey;
Sint32 EVT_lastButton;
//
int EVT_getKey(Uint32 keyID);
int EVT_getScroll();
int EVT_getButton(Uint32 button);
int EVT_getLastKey();
int EVT_getLastButton();
//--
int EVT_consumeKey(Uint32 keyID);
int EVT_consumeScroll();
int EVT_consumeButton(Uint32 button);
int EVT_consumeLastKey();
int EVT_consumeLastButton();
//
void EVT_EventHandler(SDL_Event *event, const SDL_Window *window);
void EVT_keyEvent(const SDL_Event *event, const SDL_Window *window);
void EVT_moveEvent(const SDL_Event *event, const SDL_Window *window);
void EVT_pressEvent(const SDL_Event *event, const SDL_Window *window);
void EVT_releaseKeyEvent(const SDL_Event *event, const SDL_Window *window);
void EVT_releaseEvent(const SDL_Event *event, const SDL_Window *window);
void EVT_scrollEvent(const SDL_Event *event, const SDL_Window *window);
void EVT_windowEvent(const SDL_Event *event, const SDL_Window *window);

#endif // EVENTS_H_INCLUDED
