#include "2DBase_defines.h"

RES_ResourceState *window1 = NULL;
RES_ResourceState *window2 = NULL;
RES_ResourceState *window3 = NULL;
RES_ResourceState *window4 = NULL;

void graphicalDrawingLoop(){

    if(RES_isStateUsable(window1)){
        RES_loadState(window1);
        RES_setColor(0,200,0,255);
        RES_fillRect(0,0,RES_SCREEN_WIDTH,RES_SCREEN_HEIGHT);
        printf("WIN1\n");
    }

    if(RES_isStateUsable(window2)){
        RES_loadState(window2);
        //RES_setColor(0,255,0,255);
        RES_setColorARGB(ARGB(RGB(0,255,0)));
        RES_fillRect(0,0,RES_SCREEN_WIDTH,RES_SCREEN_HEIGHT);
    }

    if(RES_isStateUsable(window3)){
        RES_loadState(window3);
        //RES_setColor(0,150,100,255);
        RES_setColorRGBA(RGBA(RGB(0,150,100)));
        RES_fillRect(0,0,RES_SCREEN_WIDTH,RES_SCREEN_HEIGHT);
    }

    if(RES_isStateUsable(window4)){
        RES_loadState(window4);
        RES_setColor(0,255,200,255);
        RES_fillRect(0,0,RES_SCREEN_WIDTH,RES_SCREEN_HEIGHT);
    }

}

void callbackLoop(){

    RES_mainLoop(graphicalDrawingLoop);

}

void externalLoop(){
    RES_running = 1;
    while(RES_running){

        RES_pollingEventLoop();
        if(!RES_running){
            break;
        }

        graphicalDrawingLoop();

        RES_pollingGraphicsLoop();
    }
}

/*
void oldMain(){

    RES_initFull();
    RES_setFPS(25);
    RES_setWindowSize(800,600);
    RES_setWindowTitle("Example");
    RES_initWindow();
    RES_showWindow();

    callbackLoop();

    RES_exit();
}
*/
#include "gui/components.h"
void newMain(){

    RES_init();
    RES_setFPS(10);
    window1 = RES_newState("Example", 800,600);
    RES_attachState(&window1);
    //printf("Location: %p     Pointed: %p    STATED: %p\n",&window1, window1, &window1);
    //printf("GUI_COMPONENT_SIZE: %i\n",sizeof(GUI_Element));
    RES_showWindow();
    window2 = RES_newState_softwareRender("Example2", 300,600);
    RES_attachState(&window2);
    RES_showWindow();
    window3 = RES_newState_alwaysOnTop("Example3", 400,400);
    RES_attachState(&window3);
    RES_showWindow();
    window4 = RES_newState_alwaysOnTop_softwareRender("Example4", 500,200);
    RES_attachState(&window4);
    RES_showWindow();

    externalLoop();

}

int main(int argc, char *argv[])
{

    //oldMain();
    newMain();

    ///process will end and return to here as final execution, most commonly only RES_exit() for cleanup
    RES_exit();

    return 0;
}
