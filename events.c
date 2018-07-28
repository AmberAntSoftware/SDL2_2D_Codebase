#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "resources.h"
#include "events.h"

SDL_Event EVT_event;
Uint8 EVT_key[MAX_EVENT_CACHE];
Uint8 EVT_btn[MAX_EVENT_CACHE];
Uint8 EVT_mov[MAX_EVENT_CACHE];
Sint32 EVT_mx = -1;
Sint32 EVT_my = -1;
Sint32 EVT_px = -1;
Sint32 EVT_py = -1;
Sint32 EVT_scroll = 0;
Sint32 EVT_lastKey = -1;
Sint32 EVT_lastButton = -1;

void EVT_EventHandler(SDL_Event *event, const SDL_Window *window){
    while(SDL_PollEvent(event)){
        switch(EVT_event.type){
        case SDL_KEYDOWN:
            //printf("Key Press\n");
            EVT_keyEvent(event, window);
            break;
        case SDL_KEYUP:
            //printf("Key Release\n");
            EVT_releaseKeyEvent(event, window);
            break;
        case SDL_QUIT:
            //Handle quit requests (like Ctrl-c). [LINUX Terminal Exit]
            RES_exit();
            RES_exitDef("Exiting SDL\n");
            break;
        case SDL_MOUSEMOTION:
            //printf("Mouse Move\n");
            EVT_moveEvent(event, window);
            break;
        case SDL_WINDOWEVENT:
            EVT_windowEvent(event, window);
            break;
        case SDL_MOUSEBUTTONDOWN:
            EVT_pressEvent(event, window);
            break;
        case SDL_MOUSEBUTTONUP:
            EVT_releaseEvent(event, window);
            break;
        case SDL_MOUSEWHEEL:
            EVT_scrollEvent(event, window);
            break;
        case SDL_TEXTEDITING:
            //printf("Text Edit || Keyboard Focus Change\n");
            break;
        case SDL_TEXTINPUT:
            //printf("Text Input || Visible Character entered\n");
            break;
        default:
            printf("Unknown Event\n");
        }
    }
}

int EVT_getLastButton(){
    return EVT_lastButton;
}

int EVT_consumeLastButton(){
    auto int local = 0;
    local = EVT_lastButton;
    EVT_lastButton = 0;
    return local;
}

int EVT_getLastKey(){
    return EVT_lastKey;
}

int EVT_consumeLastKey(){
    auto int local = 0;
    local = EVT_lastKey;
    EVT_lastKey = 0;
    return local;
}

int EVT_getKey(Uint32 keyID){
    keyID=EVT_getSDLK(keyID);
    if(keyID >= MAX_EVENT_CACHE){
        return 0;
    }
    return EVT_key[keyID];
}

int EVT_consumeKey(Uint32 keyID){
    keyID=EVT_getSDLK(keyID);
    if(keyID >= MAX_EVENT_CACHE){
        return 0;
    }
    auto int local = 0;
    local = EVT_key[keyID];
    EVT_key[keyID] = 0;
    return local;
}

int EVT_getScroll(){
    return EVT_scroll;
}

int EVT_consumeScroll(){
    auto int local = 0;
    local = EVT_scroll;
    EVT_scroll = 0;
    return local;
}

int EVT_getButton(Uint32 button){
    if(button >= MAX_EVENT_CACHE){
        return 0;
    }
    return EVT_btn[button];
}

int EVT_consumeButton(Uint32 button){
    if(button >= MAX_EVENT_CACHE){
        return 0;
    }
    auto int local = 0;
    local = EVT_btn[button];
    EVT_btn[button] = 0;
    return local;
}


void EVT_keyEvent(const SDL_Event *event, const SDL_Window *window){
    if(event->key.keysym.sym < 1024){
        EVT_key[event->key.keysym.sym] = 1;
        EVT_lastKey = event->key.keysym.sym;
    } else if(event->key.keysym.sym >= SDLK_SCANCODE_MASK){
        //printf("Scancode: %i\n",event->key.keysym.sym^SDLK_SCANCODE_MASK);
        //printf("ScanCodeDefined Test UP: %i\n",SDL_SCANCODE_UP);
        EVT_key[event->key.keysym.sym ^ SDLK_SCANCODE_MASK] = 1;
        EVT_lastKey = event->key.keysym.sym ^ SDLK_SCANCODE_MASK;
        return;
    } else {
        printf("Key Event Out Of Bounds [Too Many Keys!]\n");
    }
    /*switch(event->key.keysym.sym){
        case SDLK_ESCAPE:
            printf("Exiting Program\n");
            EVT_exit();
        //break;
    }
      case SDLK_w:
        break;
        case SDLK_s:
        break;
        case SDLK_a:
        break;
        case SDLK_d:
        break;
        case SDLK_SPACE:
        break;
        case SDLK_TAB:
        break;
        default:
            break;
    }*/
}

void EVT_moveEvent(const SDL_Event *event, const SDL_Window *window){
    EVT_mx = event->motion.x;
    EVT_my = event->motion.y;
    if(event->motion.state < 1024){
        EVT_mov[event->motion.state] = 1;
    } else {
        return;
    }
    /*switch(event->motion.state){
    case(0)://no button
    return;
    case(1):// button 1
    return;
    case(2):// button 2
    return;
    case(3):// button 1+2
    return;
    case(4):// button 3
    return;
    case(5):// button 1+3
    return;
    case(6):// button 2+3
    return;
    case(7):// button 1+2+3
    return;
    //RARE/GAMING
    //...
    case(8):// button 4
    return;
    //...
    case(16):// button 5
    return;
    }*/
}

void EVT_pressEvent(const SDL_Event *event, const SDL_Window *window){
    EVT_lastButton = event->button.button;
    EVT_px = event->button.x;
    EVT_py = event->button.y;
    if(event->button.button < 1024){
        EVT_btn[event->button.button] = 1;
    }
}

void EVT_releaseEvent(const SDL_Event *event, const SDL_Window *window){
    if(event->button.button < 1024){
        EVT_btn[event->button.button] = 0;
    }
}

void EVT_releaseKeyEvent(const SDL_Event *event, const SDL_Window *window){
    if(event->key.keysym.sym < 1024){
        EVT_key[event->key.keysym.sym] = 0;
    } else if(event->key.keysym.sym >= SDLK_SCANCODE_MASK){
        EVT_key[event->key.keysym.sym ^ SDLK_SCANCODE_MASK] = 0;
        return;
    } else {
        printf("Key Event Out Of Bounds [Too Many Keys!]\n");
    }
}

void EVT_scrollEvent(const SDL_Event *event, const SDL_Window *window){
    /*if(event->wheel.y<0){

    }else{

    }*/
    EVT_scroll = event->wheel.y;
}

void EVT_windowEvent(const SDL_Event *event, const SDL_Window *window){
    switch(event->window.event){
    case SDL_WINDOWEVENT_SHOWN:
        //SDL_Log("Window %d shown", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_HIDDEN:
        //SDL_Log("Window %d hidden", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_EXPOSED:
        //SDL_Log("Window %d exposed", event->window.windowID);
        /**Usually Last Event in window events, 'repaint' here for 1 call*/
        break;
    case SDL_WINDOWEVENT_MOVED:
        //SDL_Log("Window %d moved to %d,%d",event->window.windowID, event->window.data1,event->window.data2);
        break;
    case SDL_WINDOWEVENT_RESIZED:
        //SDL_Log("Window %d resized to %dx%d",event->window.windowID, event->window.data1,event->window.data2);
        RES_SCREEN_WIDTH = event->window.data1;
        RES_SCREEN_HEIGHT = event->window.data2;
        break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        //SDL_Log("Window %d size changed to %dx%d", event->window.windowID, event->window.data1, event->window.data2);
        RES_SCREEN_WIDTH = event->window.data1;
        RES_SCREEN_HEIGHT = event->window.data2;
        break;
    case SDL_WINDOWEVENT_MINIMIZED:
        //SDL_Log("Window %d minimized", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_MAXIMIZED:
        //SDL_Log("Window %d maximized", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_RESTORED:
        //SDL_Log("Window %d restored", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_ENTER:
        //SDL_Log("Mouse entered window %d", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_LEAVE:
        //SDL_Log("Mouse left window %d", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
        //SDL_Log("Window %d gained keyboard focus",event->window.windowID);
        break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
        //SDL_Log("Window %d lost keyboard focus",event->window.windowID);
        break;
    case SDL_WINDOWEVENT_CLOSE:
        //SDL_Log("Window %d closed", event->window.windowID);
        /*if(SDL_GetWindowID(EVT_window)==event->window.windowID){
            RES_running = 0;
        }*/
        break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
    case SDL_WINDOWEVENT_TAKE_FOCUS:
        //SDL_Log("Window %d is offered a focus", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_HIT_TEST:
        //SDL_Log("Window %d has a special hit test", event->window.windowID);
        break;
#endif
    default:
        //SDL_Log("Window %d got unknown event %d", event->window.windowID, event->window.event);
        break;
    }
}
