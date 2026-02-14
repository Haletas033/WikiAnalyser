//
// Created by haletas on 13/02/2026.
//

#include "../include/LINUXGUI.h"
#include <unistd.h>

#include "../include/WINGUI.h"
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
HDC _hdc;
Pixmap buffer;

unsigned long RGB(const COLOUR col) { return (col.r << 16) | (col.g << 8) | col.b; }

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
    windowAttributes.background_pixel = 0xFFFFFFFF; //Orange colour
}

void OSCreateWindow() {
    Window MainWindow = XCreateWindow(MainDisplay, RootWindow, windowX, windowY, windowWidth, windowHeight, borderWidth,
        windowDepth, windowClass, windowVisual, attributeValueMask, &windowAttributes);
    XStoreName(MainDisplay, MainWindow, "WikiAnalyser"); //Set window name

    //Create a Graphics context for drawing
    GC gc = XCreateGC(MainDisplay, MainWindow, 0, NULL);

    _hdc = (HDC){MainWindow, gc};

    XSelectInput(MainDisplay, MainWindow, ExposureMask);
    XSetWindowBackgroundPixmap(MainDisplay, _hdc.window, None);


    XMapWindow(MainDisplay, MainWindow);
    XFlush(MainDisplay);


}

int bufferWidth, bufferHeight;
HDC memDC;
GC bufferGC;

void OSMessageLoop() {
    for(;;) {
        XEvent generalEvent = {};
        XNextEvent(MainDisplay, &generalEvent);
        if (generalEvent.type == Expose) {
            //Get window width and height
            XWindowAttributes attrs;
            XGetWindowAttributes(MainDisplay, _hdc.window, &attrs);

            if (buffer == None || bufferWidth != attrs.width || bufferHeight != attrs.height) {
                if (buffer != None) {
                    XFreePixmap(MainDisplay, buffer);
                    XFreeGC(MainDisplay, bufferGC);
                }

                int depth = XDefaultDepth(MainDisplay, DefaultScreen(MainDisplay));
                buffer = XCreatePixmap(MainDisplay, _hdc.window, attrs.width, attrs.height, depth);
                bufferGC = XCreateGC(MainDisplay, buffer, 0, NULL);

                bufferWidth = attrs.width;
                bufferHeight = attrs.height;

                memDC = (HDC){buffer, bufferGC};
            }

            XSetForeground(MainDisplay, bufferGC, windowAttributes.background_pixel);
            XFillRectangle(MainDisplay, buffer, bufferGC, 0, 0, bufferWidth, bufferHeight);

            OSDrawRect(memDC, (COLOUR_RECT){80, 0, 100, 100, 255, 122, 0},
                bufferWidth, bufferHeight);

            XCopyArea(MainDisplay, buffer, _hdc.window, _hdc.gc, 0, 0, bufferWidth, bufferHeight, 0, 0);
        }
    }
}

void OSDrawRect(HDC hdc, COLOUR_RECT colourRect, int scrW, int scrH) {
    //Set colour
    XSetForeground(MainDisplay, hdc.gc, RGB(colourRect.colour));

    //Create percentage based x,y,w,h
    const int x = colourRect.rect.x * scrW / 100;
    const int y = colourRect.rect.y * scrH / 100;
    const int w = (colourRect.rect.w * scrW / 100) - x;
    const int h = (colourRect.rect.h * scrH / 100) - y;

    XFillRectangle(MainDisplay, hdc.window, hdc.gc, x, y, w, h);
}