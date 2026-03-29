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
#include <shlwapi.h>
#include <uxtheme.h>
#include "../include/funcStructs.h"


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
void* OSCreateChildWindowOnChildWindow(unsigned int id, const char* name, Window* wnd, const Window* parentWindow);
void* OSCreateButton(unsigned int id, void (*func)(Window* wnd), Window* wnd);
void* OSCreateCheckBox(unsigned int id, void (*func)(Window* wnd), Window* wnd);
void* OSCreateInputBox(unsigned int id, Window* wnd);
void* OSCreateDropdown(unsigned int id, void (*func)(Window* wnd), Window* wnd, const char** items, unsigned int count);
void* OSCreateMultiSelectDropdown(unsigned int id, Window* wnd, const char** items, unsigned int count);
void* OSCreateProgressBar(unsigned int id, void(*progressFunc)(void), Window* wnd, unsigned int isIndeterminate);
void OSUpdateProgress();

//Drawing functions
void OSDrawRect(HDC hdc, COLOUR_RECT colourRect, int scrW, int scrH);
void OSDrawLine(HDC hdc, COLOUR_LINE colourLine, int scrW, int scrH);
void OSDrawLineChain(HDC hdc, COLOUR_LINE_CHAIN colourLineChain, int scrW, int scrH);
void OSDrawPoint(HDC hdc, COLOUR_POINT colourPoint, int scrW, int scrH);
void OSDrawPieSlice(HDC hdc, COLOUR_PIE_SLICE colourPieSlice, int scrW, int scrH);
void OSDrawText(HDC hdc, GUI_TEXT text, int scrW, int scrH);
void OSDrawImage(HDC hdc, GUI_IMAGE image, int scrWm, int scrH);
void OSDrawChildWindow(Window* wnd, int scrW, int scrH);
void OSDrawButtonLike(GUI_BUTTON_LIKE button, int scrW, int scrH);

//Misc
void OSDoAfterMillis(Window* wnd, unsigned int id, unsigned int millis, void (*func)(Window* wnd));
void OSKillTimer(const Window* wnd,  unsigned int id);
void OSShowButtonById(const Window* wnd, unsigned int id, unsigned int shouldShow);
void OSDestroyButtonById(const Window* wnd,  unsigned int id);
const char* OSGetInputBoxTextById(const Window* wnd, unsigned int id);
const char* OSGetFilePath();
const char* OSGetDirectoryPath();
void OSGetEXEDir(char* buffer, int size);
const char* OSGetDirectoryPathInsideWikiAnalyser(const char* localPath);
void OSOpenAs(const char* projectDir, const char* filename);
int OSShellExecute(const char* projectDir, const char* command);
void OSCreateDirectory(const char* dirName);
void OSFreeLibrary();
void* OSLoadLibrary(const char* libPath, const char* funcName);
void OSCreateThreadForDownloadTo(const char* url, const char* filePath, const char* fileName);
void OSCreateThreadForDownloadSpecialExportTo(const char** name, unsigned int count, const char* filePath, const char* fileName);
void OSCreateThreadForParse(PCSTRFILEPATH szFilePath, Article **articles, Article *baseArticle, unsigned int *articleCount);
void OSCreateThreadForSystemCall(const char* cmd, void(*func)(void));
void OSCreateThreadForCreateTopnFile(void(*func)(void));
int OSGetDropdownCurrentlySelected(unsigned int id, Window* wnd);
int OSGetDropdownMultiSelectCurrentlySelected(unsigned int id, Window* wnd, int* buffer, int bufferSize);
int GetRefreshRate();


#endif
