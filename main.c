#include "2DBase_defines.h"

void graphicalDrawingLoop(){

    RES_setColor(0,200,0,255);
    RES_fillRect(0,0,RES_SCREEN_WIDTH,RES_SCREEN_HEIGHT);

}

void callbackLoop(){

    RES_mainLoop(graphicalDrawingLoop);

}

void externalLoop(){
    RES_running = 1;
    while(RES_running){

        RES_pollingEventLoop();

        graphicalDrawingLoop();

        RES_pollingGraphicsLoop();

    }
}

void oldMain(){
    RES_setWindowSize(800,600);
    RES_setWindowTitle("Example");
    RES_initWindow();
    RES_showWindow();
}

void newMain(){
    RES_newState("Example", 800,600);
    RES_showWindow();
}

int main(int argc, char *argv[])
{

    RES_initFull();
    RES_setFPS(25);

    //oldMain();
    newMain();

    //callbackLoop();
    externalLoop();
    ///process will end and return to here as final execution, most commonly only RES_exit() for cleanup

    RES_exit();
    return 0;
}
