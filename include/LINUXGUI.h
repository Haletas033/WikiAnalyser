//
// Created by haletas on 13/02/2026.
//

#ifndef WIKIANALYSER_LINUXGUI_H
#define WIKIANALYSER_LINUXGUI_H

#include "GUI.h"
#include "X11/Xlib.h"
#include "X11/Xft/Xft.h"

//min macro
#define min(a, b) (((a) < (b)) ? (a) : (b))

//Work with GUI on the OS level

//Fake HDC so it works on windows and linux
typedef struct HDC {
    Window window;
    GC gc;
} HDC;



//Setup, Loop and Destroy
void OSCreateWindowClass();
void OSCreateWindow();
void OSMessageLoop();

//Drawing functions
void OSDrawRect(HDC hdc, COLOUR_RECT colourRect, int scrW, int scrH);
void OSDrawLine(HDC hdc, COLOUR_LINE colourLine, int scrW, int scrH);
void OSDrawLineChain(HDC hdc, COLOUR_LINE_CHAIN colourLineChain, int scrW, int scrH);
void OSDrawPoint(HDC hdc, COLOUR_POINT colourPoint, int scrW, int scrH);
void OSDrawText(HDC hdc, GUI_TEXT text, int scrW, int scrH);
void OSDrawImage(HDC hdc, GUI_IMAGE image, int scrWm, int scrH);

//Functions unique to the linux version and only used in LINUXGUI
unsigned long RGB(COLOUR col);


#endif //WIKIANALYSER_LINUXGUI_H