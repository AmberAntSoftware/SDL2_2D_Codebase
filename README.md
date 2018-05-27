# SDL2_2D_Codebase
Externally high coupling, internally low coupling codebase for 2D graphical programming

>>A quick and dirty approach for specificaly one window applications

>>Direct access to the SDL pointers for the SDL_Renderer, SDL_Window, ect allows for more optimized code without the setup

>>Lacking documentation and function descriptions

>>Note: Uses software timings to limit FPS by default -- 60FPS usually results in 63 frames on average due to SDL_Delay(1000/60) resulting in an integer millisecond delay of 16, resulting in 63.33333 FPS

>>Note: There is no automatically loop based on event polling yet


How To Start
============

1) Make a main.c

2) include these headers for this project along with any others
```
#include "resources.h"
#include "events.h"
#include "audios.h"
#include "memorys.h"
#include "textury.h"
#include "xfonts.h"
```

3) The main function -- currently no other init besides RES_initFull()
```
int main(int argc, char *argv[])
{
    //call this to init everything and SDL
    RES_initFull();
    
    //... whatever you want to do here, probably setup your window
    
    //frees resources and exits program cleanly
    RES_exit();
    return 0;
}
```

Window Examples
========

1) Window Methods

Set the window title to the passed in char* or compiled string
```
RES_setWindowTitle("I Am A Title");
```

Set the window size -- X,Y
```
RES_setWindowSize(800,600);
```

Center it after setting the size, otherwise will be off-center
```
RES_centerWindow();
```

Call this last to show the window and to reduce window editing flicker, such as resizing
```
RES_showWindow();
```



Main Loop Example(s)
==================

Pass an external function to be run until the window is closed or killed internally -- must not have parameters and will not handle returns so do not return memory needing to be freed
```
RES_mainLoop(graphicsLoop);

/* elsewhere.... potentially a separate file included through headers; this will 
consume the thread at this point, so only call this when you want to run a while() 
on the thread calling this -- do note that you should only call this on the same 
thread that you called RES_initFull() on*/

void graphicsLoop(){
//... stuff
}
```


Interfacing Examples
====================
***You may make your own window outside of the convience function, but you have to use the function call `EVT_EventHandler(&EVT_event, {your_window}); and include the "events.h" header` to grab the events***

Taking input on every frame for the keyboard
```
/**NO SCANCODE MASKS -- the backspace Key will be directly stored in index 127**/

//returns nonzero if the key is currently pressed 
EVT_getKey(SDLK_{KEY})
//returns nonzero if pressed and deletes pressed state
EVT_consumeKey(SDLK_{KEY})
//Directly Access key state nonzero if pressed -- no bounding protections
EVT_key[SDLK_{KEY}]

//Directly get the last key pressed
EVT_lastKey
//returns last key as a function call
EVT_getLastKey()
//returns lastKey and deletes it -- convience function
EVT_consumeLastKey()
```

Taking input on every frame for the mouse
```
//returns nonzero if the button is currently pressed -- sta
EVT_getButton(1 .. EVT_MAX_EVENT_CACHE)
//returns nonzero if pressed and deletes pressed state
EVT_consumeButton(1 .. EVT_MAX_EVENT_CACHE)
//Directly Access key state nonzero if pressed -- no bounding protections
EVT_btn[..]

//Directly get the last button pressed
EVT_lastButton
//returns last button as a function call
EVT_getLastButton()
//returns last button and deletes it -- convience function
EVT_consumeLastButton()
```
Taking input on every frame for the mouse position -- no functions yet, only unsafe direct integer access
```
//x point at which mouse pointer was moved last
RES_mx
//y point at which mouse pointer was moved last
RES_my
//x point at which mouse pointer was pressed last
RES_px
//y point at which mouse pointer was pressed last
RES_py
```

Taking input on every frame for the mouse wheel
```
/*Currently, SDL only takes directional integers, so smooth scrolling of 
less than 1, is not supported -- e.g a precision of four where .25 .5 .75 1 
exist -- float values are rounded to zero*/

/**A value of zero means no scroll**/

//Direct access of directional value
EVT_scroll
//returns directional value
EVT_getScroll()
//returns directional value and sets to zero
EVT_consumeScroll()
```


Graphical Examples
==================

Set the drawing color
```
RES_setColor(r,g,b,a)
```

Rectangles
```
//rectangle no fill
RES_drawRect(x,y,width,height)
//rectangle with fill
RES_fillRect(x,y,width,height)
```

Lines
```
RES_drawLine(x1,y1,x2,y2)
```

Images -- Must be an XtraTexture container to use these [texture functions not yet implemented]
```
//draws the image at the location with native resolution
void RES_drawImageAt(img, x, y);
//dras the image at the location scaled to a rectangle of width, height
void RES_drawImageScaledAt(img, x, y, width, height);
//draws a section of image at the specified location at native resolution
void RES_drawImageSectionAt(img, x, y, clipBoxX, clipBoxY, clipBoxWidth, clipBoxHeight);
//draws a section of image at location scaled to a rectangle of width and height
void RES_drawImageSectionScaledAt(img, x, y, sourceWidth, sourceHeight, destX, destY, destWidth, destHeight);
```



Full Working Example
====================

```
#include <SDL2/SDL.h>

#include "resources.h"
#include "events.h"
#include "audios.h"
#include "memorys.h"
#include "textury.h"
#include "xfonts.h"

int cx = 0;
int cy = 0;
int size = 20;

//moves a square around on the screen using wasd
void notAGame(){
    RES_setColor(255,255,255,255);
    RES_fillRect(0,0,RES_SCREEN_WIDTH,RES_SCREEN_HEIGHT);

    RES_setColor(0,200,0,255);
    RES_fillRect(cx,cy,size,size);

    if(EVT_getKey(SDLK_a)){
        cx-=size;
    }
    if(EVT_getKey(SDLK_d)){
        cx+=size;
    }
    if(EVT_getKey(SDLK_s)){
        cy+=size;
    }
    if(EVT_getKey(SDLK_w)){
        cy-=size;
    }
}


int main(int argc, char *argv[])
{
    RES_initFull();
    
    RES_initFull();
    RES_setWindowTitle("THING");
    RES_setWindowSize(800,600);
    RES_centerWindow();
    RES_showWindow();
    
    RES_setFPS(20);
    RES_mainLoop(notAGame);
    
    RES_exit();
    return 0;
}
```
