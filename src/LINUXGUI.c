//
// Created by haletas on 13/02/2026.
//

#include "../include/LINUXGUI.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../include/WINGUI.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../external/stb_image_resize2.h"

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

int width, height, channels;
unsigned char* pixels;
XImage* img;
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

    //Load image (only temporarily until I make a better method
    pixels = stbi_load("test.bmp", &width, &height, &channels, 4);

    //Create the image
    img = XCreateImage(MainDisplay,
        DefaultVisual(MainDisplay, DefaultScreen(MainDisplay)),
        DefaultDepth(MainDisplay, DefaultScreen(MainDisplay)),
        ZPixmap,
        0,
        malloc(width * height * 4),
        width, height,
        32, 0
    );

    //Load image into the Ximage
    int x, y;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            unsigned char *p = &pixels[(y*width + x)*4];
            unsigned long pixel = (p[0] << 16) | (p[1] << 8) | (p[2]);
            XPutPixel(img, x, y, pixel);
        }
    }
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

            //Create the buffer if it doesn't exist or is the wrong size
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

            //Clear the screen
            XSetForeground(MainDisplay, bufferGC, windowAttributes.background_pixel);
            XFillRectangle(MainDisplay, buffer, bufferGC, 0, 0, bufferWidth, bufferHeight);

            OSDrawImage(memDC, (GUI_IMAGE){25, 25, 50, 50, ""},
                bufferWidth, bufferHeight);

            OSDrawRect(memDC, (COLOUR_RECT){80, 0, 100, 100, 255, 122, 0},
                bufferWidth, bufferHeight);

            GUI_POINT points[3] = {(GUI_POINT){0,0}, (GUI_POINT){50,70}, (GUI_POINT){100, 0}};
            OSDrawLineChain(memDC, (COLOUR_LINE_CHAIN){points, 3, 3, 0,0,0},
                bufferWidth, bufferHeight);

            OSDrawPoint(memDC, (COLOUR_POINT){50, 50, 50, 255, 255, 0},
                bufferWidth, bufferHeight);

            OSDrawText(memDC, (GUI_TEXT){"Hello, World", 50, 50, 12},
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

void OSDrawLine(HDC hdc, COLOUR_LINE colourLine, int scrW, int scrH) {
    //Set colour and thickness
    XSetForeground(MainDisplay, hdc.gc, RGB(colourLine.colour));
    XSetLineAttributes(MainDisplay, hdc.gc, colourLine.line.thickness, LineSolid, CapButt, JoinMiter);

    //Create percentage based points
    GUI_POINT p1 = (GUI_POINT){colourLine.line.p1.x * scrW / 100, colourLine.line.p1.y * scrH / 100};
    GUI_POINT p2 = (GUI_POINT){colourLine.line.p2.x * scrW / 100, colourLine.line.p2.y * scrH / 100};

    XDrawLine(MainDisplay, hdc.window, hdc.gc, p1.x, p1.y, p2.x, p2.y);
}

void OSDrawPoint(HDC hdc, COLOUR_POINT colourPoint, int scrW, int scrH) {
    //Set colour
    XSetForeground(MainDisplay, hdc.gc, RGB(colourPoint.colour));

    //Create percentage based point and radius
    const GUI_POINT p = (GUI_POINT){colourPoint.point.x * scrW / 100, colourPoint.point.y * scrH / 100};
    const int radius = colourPoint.radius * min(scrW, scrH) / 100;

    //Build center x and y
    const int x = p.x - radius / 2;
    const int y = p.y - radius / 2;

    XFillArc(MainDisplay, hdc.window, hdc.gc, x, y, radius, radius, 0, 360*64);
}

void OSDrawText(HDC hdc, GUI_TEXT text, int scrW, int scrH) {
    //Create screen and draw
    const int screen = DefaultScreen(MainDisplay);

    XftDraw *draw = XftDrawCreate(MainDisplay, hdc.window,
           DefaultVisual(MainDisplay, screen),
           DefaultColormap(MainDisplay, screen)
    );

    //Percentage based x,y, size
    const GUI_POINT p = (GUI_POINT){text.pos.x * scrW / 100, text.pos.y * scrH / 100};
    const int size = text.size * min(scrW, scrH) / 100;

    //Create font
    //Get fontName + fontSize
    char fontNameBuffer[32] = "DejaVu Sans-";
    sprintf(fontNameBuffer, "%d", size);

    XftFont *font = XftFontOpenName(MainDisplay, screen, fontNameBuffer);
    XftColor colour;
    XftColorAllocName(MainDisplay,
        DefaultVisual(MainDisplay, screen),
        DefaultColormap(MainDisplay, screen),
        "black",
        &colour
    );

    //Get text width and height for centering
    XGlyphInfo extents;
    XftTextExtentsUtf8(MainDisplay, font, (XftChar8 *)text.text, strlen(text.text), &extents);

    XftDrawStringUtf8(draw, &colour, font, p.x - extents.width / 2, p.y + (font->ascent - font->descent) / 2, (XftChar8*)text.text, strlen(text.text));

    XftColorFree(MainDisplay, DefaultVisual(MainDisplay, screen), DefaultColormap(MainDisplay, screen), &colour);
}

void OSDrawImage(HDC hdc, GUI_IMAGE image, int scrW, int scrH) {
    //Get percentage based x,y,w,h
    const GUI_POINT start = (GUI_POINT){image.rect.x * scrW / 100, image.rect.y * scrH / 100};
    const GUI_POINT end = (GUI_POINT){image.rect.w * scrW / 100, image.rect.h * scrH / 100};

    //Scale image
    unsigned char* resizedPixels = malloc(end.x * end.y * 4);
    stbir_resize_uint8_srgb(pixels, width, height, 0, resizedPixels, end.x, end.y, 0, 4);

    //Create image
    XImage* resizedImg = XCreateImage(
        MainDisplay,
        DefaultVisual(MainDisplay, DefaultScreen(MainDisplay)),
        DefaultDepth(MainDisplay, DefaultScreen(MainDisplay)),
        ZPixmap,
        0,
        (char*)resizedPixels,
        end.x,
        end.y,
        32,
        0
    );

    //Draw image
    XPutImage(MainDisplay, hdc.window, hdc.gc, resizedImg, 0, 0, start.x, start.y, end.x, end.y);

    //Cleanup
    resizedImg->data = NULL;
    XDestroyImage(resizedImg);
    stbi_image_free(resizedPixels);
}

void OSDrawLineChain(HDC hdc, COLOUR_LINE_CHAIN colourLineChain, int scrW, int scrH) {
    //Set colour and thickness
    XSetForeground(MainDisplay, hdc.gc, RGB(colourLineChain.colour));
    XSetLineAttributes(MainDisplay, hdc.gc, colourLineChain.linePath.thickness, LineSolid, CapButt, JoinMiter);

    //Create percentage based x points
    XPoint points[colourLineChain.linePath.pointsSize];
    int p;
    for (p = 0; p < colourLineChain.linePath.pointsSize; p++) {
        points[p].x = colourLineChain.linePath.points[p].x * scrW / 100;
        points[p].y = colourLineChain.linePath.points[p].y * scrH / 100;
    }

    //Draw lines
    XDrawLines(MainDisplay, hdc.window, hdc.gc, points, colourLineChain.linePath.pointsSize, CoordModeOrigin);
}