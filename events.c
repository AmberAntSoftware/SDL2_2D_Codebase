#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "2DBase_defines.h"

#include "resources.h"
#include "events.h"
#include "stringys.h"


static Uint32 lastWindow = 1;


static int EVT_X_deleteWindowStates(const SDL_Event *event){
    char ibuff[20];
    char *nbuff = NULL;
    int i = -1, j;
    void *data = NULL;
    void *freed[128];
    for(j=0;j<128;j++){
        freed[j]=NULL;
    }
    SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
    if(window==NULL){
        return -1;
    }
    do{
        i++;
        SDL_itoa(i,&(ibuff[0]),10);
        if(nbuff!=NULL){
            free(nbuff);
        }
        nbuff = STR_concat((char*)"state",ibuff);
        data = SDL_GetWindowData(window,nbuff);
        if(data!=NULL){
            for(j=0;j<128;j++){
                if(freed[j]==NULL){
                    freed[j]=data;
                    break;
                }
                if(freed[j]==data){
                    break;
                }
            }
        }
    }while(data!=NULL);
    if(nbuff!=NULL){
        free(nbuff);
    }
    for(j=0;j<128;j++){
        if(freed[j]!=NULL){
            RES_ResourceState *state = (RES_ResourceState*)freed[j];
            TEX_freeTexturesByRenderer(state->renderer);
            RES_removeStateFromDump(state);
        }else{
            break;
        }
    }
    return i;
}

static void EVT_X_handleExit(const SDL_Event* event, const SDL_Window *window){
    //Handle quit requests (like Ctrl-c). [LINUX Terminal Exit]
    EVT_X_deleteWindowStates(event);
    /*
    if(lastWindow==event->window.windowID){
        //RES_running = 0;
    }
    //RES_exit();
    //RES_exitProcessDef("Exiting SDL\n");
    */
}

static void EVT_ifWindowChanged(Uint32 eventWindow){
    if(eventWindow==lastWindow){
        return;
    }

    lastWindow = eventWindow;

#if ALLOW_EVENTS_WIPING_ON_SWITCH
    EVT_mx = -1;
    EVT_my = -1;
    EVT_px = -1;
    EVT_py = -1;
    EVT_scroll = 0;
    EVT_lastKey = -1;
    EVT_lastButton = -1;

    Uint32 i;
    for(i = 0; i < MAX_EVENT_CACHE; i++){
        EVT_key[i] = 0;
        EVT_btn[i] = 0;
        EVT_mov[i] = 0;
    }
#endif // ALLOW_EVENTS_WIPING_ON_SWITCH
}





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
        EVT_ifWindowChanged(event->window.windowID);

        switch(EVT_event.type){
        case SDL_KEYDOWN:
            //printf("Key Press\n");
            EVT_keyPressEvent(event, window);
            break;
        case SDL_KEYUP:
            //printf("Key Release\n");
            EVT_keyReleaseEvent(event, window);
            break;
        case SDL_QUIT:
            EVT_X_handleExit(event,window);
            return;
        case SDL_MOUSEMOTION:
            //printf("Mouse Move\n");
            EVT_mouseMoveEvent(event, window);
            break;
        case SDL_WINDOWEVENT:
            EVT_windowEvent(event, window);
            break;
        case SDL_MOUSEBUTTONDOWN:
            EVT_mousePressEvent(event, window);
            break;
        case SDL_MOUSEBUTTONUP:
            EVT_mouseReleaseEvent(event, window);
            break;
        case SDL_MOUSEWHEEL:
            EVT_mouseScrollEvent(event, window);
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


void EVT_keyPressEvent(const SDL_Event *event, const SDL_Window *window){
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

void EVT_mouseMoveEvent(const SDL_Event *event, const SDL_Window *window){
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

void EVT_mousePressEvent(const SDL_Event *event, const SDL_Window *window){
    EVT_lastButton = event->button.button;
    EVT_px = event->button.x;
    EVT_py = event->button.y;
    if(event->button.button < 1024){
        EVT_btn[event->button.button] = 1;
    }
}

void EVT_mouseReleaseEvent(const SDL_Event *event, const SDL_Window *window){
    if(event->button.button < 1024){
        EVT_btn[event->button.button] = 0;
    }
}

void EVT_keyReleaseEvent(const SDL_Event *event, const SDL_Window *window){
    if(event->key.keysym.sym < 1024){
        EVT_key[event->key.keysym.sym] = 0;
    } else if(event->key.keysym.sym >= SDLK_SCANCODE_MASK){
        EVT_key[event->key.keysym.sym ^ SDLK_SCANCODE_MASK] = 0;
        return;
    } else {
        printf("Key Event Out Of Bounds [Too Many Keys!]\n");
    }
}

void EVT_mouseScrollEvent(const SDL_Event *event, const SDL_Window *window){
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
        if(lastWindow==event->window.windowID){
            RES_SCREEN_WIDTH = event->window.data1;
            RES_SCREEN_HEIGHT = event->window.data2;
        }
        break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        //SDL_Log("Window %d size changed to %dx%d", event->window.windowID, event->window.data1, event->window.data2);
        if(lastWindow==event->window.windowID){
            RES_SCREEN_WIDTH = event->window.data1;
            RES_SCREEN_HEIGHT = event->window.data2;
        }
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
        EVT_X_handleExit(event,window);
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
