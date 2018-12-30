#include "2DBase_defines.h"

#include "stringys.h"

RES_ResourceState *window1 = NULL;
RES_ResourceState *window2 = NULL;
RES_ResourceState *window3 = NULL;
RES_ResourceState *window4 = NULL;

void graphicalDrawingLoop(){

    if(RES_isStateUsable(window1)){
        RES_loadState(window1);
        RES_setColor(0,200,0,255);
        RES_fillRect(0,0,RES_SCREEN_WIDTH,RES_SCREEN_HEIGHT);
    }

    if(RES_isStateUsable(window2)){
        RES_loadState(window2);
        RES_setColor(0,255,0,255);
        RES_fillRect(0,0,RES_SCREEN_WIDTH,RES_SCREEN_HEIGHT);
    }

    if(RES_isStateUsable(window3)){
        RES_loadState(window3);
        RES_setColor(0,150,100,255);
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

//This will compile, but due to how states work and how the init function changed, this will do nothing and potentially crash
void oldMain(){

    RES_initFull();
    RES_setFPS(25);
    RES_setWindowSize(800,600);
    RES_setWindowTitle("Example");
    RES_initWindow();
    RES_showWindow();

    callbackLoop();

}

void newMain(){

    RES_init();
    RES_setFPS(25);
    window1 = RES_newState("Example", 800,600);
    RES_showWindow();
    window2 = RES_newState("Example2", 300,600);
    RES_showWindow();
    window3 = RES_newState("Example3", 400,400);
    RES_showWindow();
    window4 = RES_newState("Example4", 500,200);
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
