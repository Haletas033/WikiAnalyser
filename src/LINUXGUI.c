//
// Created by haletas on 13/02/2026.
//

#include "../include/LINUXGUI.h"
#include <unistd.h>
Display* MainDisplay = NULL;
Window RootWindow = {};

int windowX;
int windowY;
int windowWidth;
int windowHeight;
int borderWidth;
int windowDepth;
int windowClass;
Visual* windowVisual;
int attributeValueMask;
XSetWindowAttributes windowAttributes = {};


void OSCreateWindowClass() {
    MainDisplay = XOpenDisplay(0);
    RootWindow = XDefaultRootWindow(MainDisplay);

    //Default window properties
    windowX = 0;
    windowY = 0;
    windowWidth = 640;
    windowHeight = 480;
    borderWidth = 0;
    windowDepth = CopyFromParent;
    windowClass = CopyFromParent;
    windowVisual = CopyFromParent;
    attributeValueMask = CWBackPixel;
    windowAttributes.background_pixel = 0xffff7a00; //Orange colour
}

void OSCreateWindow() {
    Window MainWindow = XCreateWindow(MainDisplay, RootWindow, windowX, windowY, windowWidth, windowHeight, borderWidth,
        windowDepth, windowClass, windowVisual, attributeValueMask, &windowAttributes);
    XMapWindow(MainDisplay, MainWindow);
    XFlush(MainDisplay);

    for(;;) {
        XEvent generalEvent = {};
        XNextEvent(MainDisplay, &generalEvent);
    }
}