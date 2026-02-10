//
// Created by halet on 2/10/2026.
//

#ifndef WINGUI_H
#define WINGUI_H

#include <windows.h>

extern const wchar_t CLASS_NAME[];
extern WNDCLASS wc;

//Works with GUI on the OS level
void OSCreateWindowClass();
void OSCreateWindow();
void OSMessageLoop();

#endif //WINGUI_H
