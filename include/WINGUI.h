//
// Created by halet on 2/10/2026.
//

#ifndef WINGUI_H
#define WINGUI_H

#ifdef __win32__

#include <windows.h>

#include "GUI.h"

extern const wchar_t CLASS_NAME[];
extern WNDCLASS wc;
extern HWND hwnd;

//Work with GUI on the OS level

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

//Misc
int GetRefreshRate();

#endif

#endif //WINGUI_H
