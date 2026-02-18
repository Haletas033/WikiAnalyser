//
// Created by halet on 2/10/2026.
//

#ifndef WINGUI_H
#define WINGUI_H

#ifndef __linux
#include <windows.h>


#include "GUI.h"

extern const wchar_t CLASS_NAME[];
extern WNDCLASS wc;
extern HWND hwnd;
#endif
//Work with GUI on the OS level

//Setup, Loop and Destroy
void OSCreateWindowClass();
void OSCreateWindow();
void OSMessageLoop();

//Creation functions
void* OSCreateImage(const char* imgPath);
void* OSCreateButton(unsigned int id, void (*func)(void));
void* OSCreateCheckBox(unsigned int id, void (*func)(void));

//Drawing functions
void OSDrawRect(HDC hdc, COLOUR_RECT colourRect, int scrW, int scrH);
void OSDrawLine(HDC hdc, COLOUR_LINE colourLine, int scrW, int scrH);
void OSDrawLineChain(HDC hdc, COLOUR_LINE_CHAIN colourLineChain, int scrW, int scrH);
void OSDrawPoint(HDC hdc, COLOUR_POINT colourPoint, int scrW, int scrH);
void OSDrawText(HDC hdc, GUI_TEXT text, int scrW, int scrH);
void OSDrawImage(HDC hdc, GUI_IMAGE image, int scrWm, int scrH);
void OSDrawButtonLike(GUI_BUTTON_LIKE button, int scrW, int scrH);

//Misc
void OSDoAfterMillis(unsigned int id, unsigned int millis, void (*func)(void));
void OSKillTimer(unsigned int id);
void OSDestroyButtonById(unsigned int id);
int GetRefreshRate();


#endif
