//
// Created by halet on 2/10/2026.
//

#ifndef WINGUI_H
#define WINGUI_H

#ifndef __linux
#include <windows.h>
#include <commdlg.h>
#include <direct.h>
#include <shlobj.h>


#include "GUI.h"

extern const wchar_t CLASS_NAME[];
extern WNDCLASS wc;
extern HWND hwnd;
#endif
//Work with GUI on the OS level

//Setup, Loop and Destroy
void OSCreateWindowClass();
void OSCreateWindow(Window* wnd);
void OSMessageLoop();

//Creation functions
void* OSCreateImage(const char* imgPath);
void* OSCreateChildWindow(unsigned int id, const char* name, Window* wnd);
void* OSCreateButton(unsigned int id, void (*func)(Window* wnd), Window* wnd);
void* OSCreateCheckBox(unsigned int id, void (*func)(Window* wnd), Window* wnd);
void* OSCreateInputBox(unsigned int id, Window* wnd);

//Drawing functions
void OSDrawRect(HDC hdc, COLOUR_RECT colourRect, int scrW, int scrH);
void OSDrawLine(HDC hdc, COLOUR_LINE colourLine, int scrW, int scrH);
void OSDrawLineChain(HDC hdc, COLOUR_LINE_CHAIN colourLineChain, int scrW, int scrH);
void OSDrawPoint(HDC hdc, COLOUR_POINT colourPoint, int scrW, int scrH);
void OSDrawText(HDC hdc, GUI_TEXT text, int scrW, int scrH);
void OSDrawImage(HDC hdc, GUI_IMAGE image, int scrWm, int scrH);
void OSDrawChildWindow(Window* wnd, int scrW, int scrH);
void OSDrawButtonLike(GUI_BUTTON_LIKE button, int scrW, int scrH);

//Misc
void OSDoAfterMillis(Window* wnd, unsigned int id, unsigned int millis, void (*func)(Window* wnd));
void OSKillTimer(const Window* wnd,  unsigned int id);
void OSDestroyButtonById(const Window* wnd,  unsigned int id);
const char* OSGetInputBoxTextById(const Window* wnd, unsigned int id);
const char* OSGetFilePath();
const char* OSGetDirectoryPath();
void OSCreateDirectory(const char* dirName);
int GetRefreshRate();


#endif
