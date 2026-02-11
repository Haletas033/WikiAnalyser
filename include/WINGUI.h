//
// Created by halet on 2/10/2026.
//

#ifndef WINGUI_H
#define WINGUI_H

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

//Misc
int GetRefreshRate();

#endif //WINGUI_H
