//
// Created by halet on 2/10/2026.
//

#include "../include/WINGUI.h"

#include <stdio.h>

#include "../include/curl.h"

const wchar_t CLASS_NAME[] = L"WINDOW CLASS";
WNDCLASS wc = {};
HWND rootHwnd;
HMODULE zigLib = NULL;

HWND progressBarHwnd;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY: {
            const PaintStacks* hwndPaintStacks = (PaintStacks*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            int i;
            for (i = 0; i < hwndPaintStacks->windowsSize; i++) {
                free(hwndPaintStacks->windows[i]);
            }

            if (hwnd == rootHwnd)
                PostQuitMessage(0);
            return 0;
        }
        case WM_TIMER:
            if (wParam == 1) {
                InvalidateRect(hwnd, NULL, FALSE);
                OSUpdateProgress();
            } else {
                doAfters[wParam - 1].doAfterFunc(doAfters[wParam - 1].wnd);
            }
            return 0;
        case WM_COMMAND: {
            const int id = LOWORD(wParam);
            const int notification = HIWORD(wParam);
            HWND control = (HWND)lParam;

            char className[32];
            GetClassName(control, className, 32);

            if (strcmp(className, "ComboBox") == 0 && notification == CBN_SELCHANGE) {
                if (dropdownCommands[id-1].buttonCommand != NULL)
                    dropdownCommands[id-1].buttonCommand(dropdownCommands[id-1].wnd);
            } else if (strcmp(className, "Button") == 0 && notification == BN_CLICKED) {
                currentButtonId = id;
                if (buttonCommands[id-1].buttonCommand != NULL)
                    buttonCommands[id-1].buttonCommand(buttonCommands[id-1].wnd);
            }
            return 0;
        }
        case WM_PAINT: {
            RECT windowRect;
            GetClientRect(hwnd, &windowRect);
            const int windowWidth = windowRect.right - windowRect.left;
            const int windowHeight = windowRect.bottom - windowRect.top;

            PAINTSTRUCT ps;
            const HDC hdc = BeginPaint(hwnd, &ps);

            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBmp = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
            SelectObject(memDC, memBmp);

            //Clear Background
            FillRect(memDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));

            const PaintStacks* hwndPaintStacks = (PaintStacks*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            int i;
            for (i = 0; i < hwndPaintStacks->windowsSize; i++)
                OSDrawChildWindow(hwndPaintStacks->windows[i], windowWidth, windowHeight);

            for (i = 0; i < hwndPaintStacks->buttonsSize; i++)
                OSDrawButtonLike(hwndPaintStacks->buttons[i], windowWidth, windowHeight);

            for (i = 0; i < hwndPaintStacks->imagesSize; i++)
                OSDrawImage(memDC, hwndPaintStacks->images[i], windowWidth, windowHeight);

            for (i = 0; i < hwndPaintStacks->colourRectsSize; i++)
                OSDrawRect(memDC, hwndPaintStacks->colourRects[i], windowWidth, windowHeight);

            for (i = 0; i < hwndPaintStacks->colourLinesSize; i++)
                OSDrawLine(memDC, hwndPaintStacks->colourLines[i], windowWidth, windowHeight);

            for (i = 0; i < hwndPaintStacks->colourLinesChainsSize; i++)
                OSDrawLineChain(memDC, hwndPaintStacks->colourLineChains[i], windowWidth,windowHeight);

            for (i = 0; i < hwndPaintStacks->colourPointsSize; i++)
                OSDrawPoint(memDC, hwndPaintStacks->colourPoints[i], windowWidth, windowHeight);

            for (i = 0; i < hwndPaintStacks->colourPieSlicesSize; i++)
                OSDrawPieSlice(memDC, hwndPaintStacks->colourPieSlices[i], windowWidth, windowHeight);

            for (i = 0; i < hwndPaintStacks->textsSize; i++)
                OSDrawText(memDC, hwndPaintStacks->texts[i], windowWidth, windowHeight);



            BitBlt(hdc, 0, 0, windowWidth, windowHeight, memDC, 0, 0, SRCCOPY);

            DeleteObject(memBmp);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
        }
        return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void OSCreateWindowClass() {
    INITCOMMONCONTROLSEX icc = {0};
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_PROGRESS_CLASS;

    InitCommonControlsEx(&icc);

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;

    RegisterClass(&wc);
}

void OSCreateWindow(Window* wnd) {
    rootHwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "WikiAnalyser",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );

    if (rootHwnd == NULL) {
        return;
    }

    SetWindowLongPtr(rootHwnd, GWLP_USERDATA, (LONG_PTR)&wnd->paintStacks);

    SetTimer(rootHwnd, 1, min(1000 / 120, 1000 / GetRefreshRate()), NULL);

    ShowWindow(rootHwnd, 1);

    wnd->wndHwnd = rootHwnd;
}

void OSMessageLoop() {
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        GUILoop();
    }
}

void* OSCreateChildWindow(const unsigned int id, const char* name, Window* wnd) {
    HWND windowHwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        name,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN,
        0, 0, 0, 0,
        rootHwnd,
        (HMENU)id,
        wc.hInstance,
        NULL
    );

    SetWindowLongPtr(windowHwnd, GWLP_USERDATA, (LONG_PTR)&wnd->paintStacks);

    SetTimer(windowHwnd, 1, min(1000 / 120, 1000 / GetRefreshRate()), NULL);

    wnd->wndHwnd = windowHwnd;
    return windowHwnd;
}

void* OSCreateChildWindowOnChildWindow(const unsigned int id, const char* name, Window* wnd, const Window* parentWindow) {
    HWND windowHwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        name,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN,
        0, 0, 0, 0,
        parentWindow->wndHwnd,
        (HMENU)id,
        wc.hInstance,
        NULL
    );

    SetWindowLongPtr(windowHwnd, GWLP_USERDATA, (LONG_PTR)&wnd->paintStacks);

    SetTimer(windowHwnd, 1, min(1000 / 120, 1000 / GetRefreshRate()), NULL);

    wnd->wndHwnd = windowHwnd;
    return windowHwnd;
}

void* OSCreateButton(const unsigned int id, void (*func)(Window* wnd), Window* wnd) {
    HWND buttonHwnd = CreateWindowEx(
        0,
        "BUTTON",
        "",
        WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0,0,0,0,
        wnd->wndHwnd,
        (HMENU)id,
        wc.hInstance,
        NULL
    );

    buttonCommands[id-1].buttonCommand = func;
    buttonCommands[id-1].wnd = wnd;
    return buttonHwnd;
}

void* OSCreateCheckBox(const unsigned int id, void (*func)(Window* wnd), Window* wnd) {
    HWND buttonHwnd = CreateWindowEx(
        0,
        "BUTTON",
        "",
        WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        0,0,0,0,
        wnd->wndHwnd,
        (HMENU)id,
        wc.hInstance,
        NULL
    );

    buttonCommands[id-1].buttonCommand = func;
    buttonCommands[id-1].wnd = wnd;
    return buttonHwnd;
}

void* OSCreateInputBox(const unsigned int id, Window* wnd) {
    HWND inputBox = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL,
        0,0,0,0,
        wnd->wndHwnd,
        (HMENU)id,
        wc.hInstance,
        NULL
    );

    return inputBox;
}

void* OSCreateDropdown(const unsigned int id, void (*func)(Window* wnd), Window* wnd, const char** items, const unsigned int count) {
    HWND comboBox = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "COMBOBOX",
        "",
        WS_TABSTOP | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
        0,0,0,0,
        wnd->wndHwnd,
        (HMENU)id,
        wc.hInstance,
        NULL
    );

    int i;
    for (i = 0; i < count; i++)
        SendMessage(comboBox, CB_ADDSTRING, 0, (LPARAM)items[i]);

    dropdownCommands[id-1].buttonCommand = func;
    dropdownCommands[id-1].wnd = wnd;

    return comboBox;
}

void* OSCreateMultiSelectDropdown(const unsigned int id, Window* wnd, const char** items, const unsigned int count) {
    HWND listBox = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_MULTIPLESEL | LBS_NOTIFY,
        0,0,0,0,
        wnd->wndHwnd,
        (HMENU)id,
        wc.hInstance,
        NULL
    );

    int i;
    for (i = 0; i < count; i++)
        SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)items[i]);

    return listBox;
}

void* OSCreateProgressBar(const unsigned int id, void(*progressFunc)(void), Window* wnd, const unsigned int isIndeterminate) {
    HWND progressBar = CreateWindowEx(
        0,
        "msctls_progress32",
        "",
        WS_TABSTOP | WS_CHILD | WS_VISIBLE | (isIndeterminate ? PBS_MARQUEE : 0),
        0,0,0,0,
        wnd->wndHwnd,
        (HMENU)id,
        wc.hInstance,
        NULL
    );
    indeterminate = isIndeterminate;
    progressDoneFunc = progressFunc;
    progressBarHwnd = progressBar;

    if (indeterminate) {
        SendMessage(progressBarHwnd, PBM_SETMARQUEE, 1, 10);
    }

    return progressBar;
}

void OSUpdateProgress() {
    if (!indeterminate) {
        SendMessage(progressBarHwnd, PBM_SETRANGE32, 0, maxProgress);
        SendMessage(progressBarHwnd, PBM_SETPOS, progress, 0);

        if (progress >= maxProgress) {
            if (progressDoneFunc != NULL) progressDoneFunc();
            //Cleanup
            progress = 0;
            maxProgress = 0;
            indeterminate = 0;
            progressDoneFunc = NULL;
        }
    } else {
        if (progress == -1) {
            if (progressDoneFunc != NULL) progressDoneFunc();
            //Cleanup
            progress = 0;
            maxProgress = 0;
            indeterminate = 0;
            progressDoneFunc = NULL;
        }
    }


}

void* OSCreateImage(const char* imgPath) {
    void* img = (HBITMAP)LoadImage(NULL, imgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!img) return NULL;
    return img;
}

void OSDrawRect(HDC hdc, COLOUR_RECT colourRect, int scrW, int scrH) {
    const GUI_RECT rect = colourRect.rect;
    const GUI_RECT realRect = (GUI_RECT){(rect.x * scrW / 100), (rect.y * scrH / 100), (rect.w * scrW / 100), (rect.h * scrH / 100)};
    const COLOUR colour = colourRect.colour;

    HBRUSH brush = CreateSolidBrush(RGB(colour.r, colour.g, colour.b));
    //The +1 ensures no 1 pixel white gaps caused by integer division
    FillRect(hdc, &(RECT){realRect.x, realRect.y, realRect.w + 1, realRect.h + 1}, brush);

    DeleteObject(brush);
}

void OSDrawLine(HDC hdc, COLOUR_LINE colourLine, int scrW, int scrH) {
    const COLOUR col = colourLine.colour;

    const GUI_POINT p1 = (GUI_POINT){colourLine.line.p1.x * scrW / 100, colourLine.line.p1.y * scrH / 100};
    const GUI_POINT p2 = (GUI_POINT){colourLine.line.p2.x * scrW / 100, colourLine.line.p2.y * scrH / 100};

    HPEN pen = CreatePen(PS_SOLID, colourLine.line.thickness,RGB(col.r, col.g, col.b));
    HPEN oldPen = SelectObject(hdc, pen);
    MoveToEx(hdc, p1.x,p1.y, NULL);
    LineTo(hdc, p2.x, p2.y);

    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void OSDrawLineChain(HDC hdc, COLOUR_LINE_CHAIN colourLineChain, int scrW, int scrH) {
    const COLOUR col = colourLineChain.colour;

    const GUI_POINT p1 = (GUI_POINT){colourLineChain.linePath.points[0].x * scrW / 100, colourLineChain.linePath.points[0].y * scrH / 100};

    #define nextPoint(i) (GUI_POINT){colourLineChain.linePath.points[i].x * scrW / 100, colourLineChain.linePath.points[i].y * scrH / 100}

    HPEN pen = CreatePen(PS_SOLID, colourLineChain.linePath.thickness,RGB(col.r, col.g, col.b));
    HPEN oldPen = SelectObject(hdc, pen);
    MoveToEx(hdc, p1.x,p1.y, NULL);
    int i;
    for (i = 1; i < colourLineChain.linePath.pointsSize; i++)
        LineTo(hdc, nextPoint(i).x, nextPoint(i).y);

    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void OSDrawPoint(HDC hdc, COLOUR_POINT colourPoint, int scrW, int scrH) {
    const COLOUR col = colourPoint.colour;
    const GUI_POINT p = (GUI_POINT){colourPoint.point.x * scrW / 100, colourPoint.point.y * scrH / 100};
    const int radius =colourPoint.radius/2 * min(scrW, scrH) / 100;

    HPEN pen = CreatePen(PS_SOLID, 1,RGB(col.r, col.g, col.b));
    HBRUSH brush = CreateSolidBrush(RGB(col.r, col.g, col.b));
    HPEN oldPen = SelectObject(hdc, pen);
    HBRUSH oldBrush = SelectObject(hdc, brush);

    Ellipse(hdc, p.x - radius, p.y - radius, p.x + radius, p.y + radius);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void OSDrawPieSlice(HDC hdc, COLOUR_PIE_SLICE colourPieSlice, int scrW, int scrH) {
    const COLOUR col = colourPieSlice.colour;
    const GUI_POINT center = (GUI_POINT){colourPieSlice.pieSlice.center.x * scrW / 100, colourPieSlice.pieSlice.center.y * scrH / 100};
    const int radius = colourPieSlice.pieSlice.radius/2 * min(scrW, scrH) / 100;
    const GUI_RECT boundingBox = (GUI_RECT){center.x-radius, center.y-radius, center.x+radius, center.y+radius};

    const GUI_POINT startDeg = (GUI_POINT){
        center.x+cos(DEG_TO_RAD(-colourPieSlice.pieSlice.startDegrees))*radius,
        center.y-sin(DEG_TO_RAD(-colourPieSlice.pieSlice.startDegrees))*radius
    };

    const GUI_POINT endDeg = (GUI_POINT){
        center.x+cos(DEG_TO_RAD(-colourPieSlice.pieSlice.endDegrees))*radius,
        center.y-sin(DEG_TO_RAD(-colourPieSlice.pieSlice.endDegrees))*radius
    };

    HPEN pen = CreatePen(PS_SOLID, 1,RGB(col.r, col.g, col.b));
    HBRUSH brush = CreateSolidBrush(RGB(col.r, col.g, col.b));
    HPEN oldPen = SelectObject(hdc, pen);
    HBRUSH oldBrush = SelectObject(hdc, brush);

    Pie(hdc, boundingBox.x, boundingBox.y, boundingBox.w, boundingBox.h,endDeg.x, endDeg.y,  startDeg.x, startDeg.y);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void OSDrawText(HDC hdc, GUI_TEXT text, int scrW, int scrH) {
    text.size = text.size * min(scrW / 6, scrH) / 100;

    HFONT hFont = CreateFont(
        text.size, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, TEXT("Courier New")
    ); SetBkMode(hdc, TRANSPARENT);
    HFONT oldFont = SelectObject(hdc, hFont);

    SIZE textSize;
    GetTextExtentPoint32A(hdc, text.text, strlen(text.text), &textSize);

    int x = text.pos.x * scrW  / 100;
    int y = text.pos.y * scrH / 100;

    x -= textSize.cx / 2;
    y -= textSize.cy / 2;

    TextOut(hdc, x, y, text.text, strlen(text.text));

    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
}

void OSDrawImage(HDC hdc, GUI_IMAGE image, int scrW, int scrH) {
    HDC imgDC = CreateCompatibleDC(hdc);
    HBITMAP oldImage = SelectObject(imgDC, image.imgLoc);

    BITMAP bm;
    GetObject(image.imgLoc, sizeof(BITMAP), &bm);

    const int x = image.rect.x * scrW / 100;
    const int y = image.rect.y * scrH / 100;
    const int w = image.rect.w * scrW / 100;
    const int h = image.rect.h * scrH / 100;

    StretchBlt(hdc, x, y, w, h, imgDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    SelectObject(imgDC, oldImage);
    DeleteDC(imgDC);
}

void OSDrawChildWindow(Window* wnd, int scrW, int scrH) {
        HWND buttonHwnd = wnd->wndHwnd;

        //Get percentage based x,y,w,h
        const GUI_POINT start = (GUI_POINT){wnd->windowRect.x * scrW / 100, wnd->windowRect.y * scrH / 100};
        const GUI_POINT end = (GUI_POINT){wnd->windowRect.w * scrW / 100, wnd->windowRect.h * scrH / 100};

        MoveWindow(buttonHwnd, start.x, start.y, end.x, end.y, TRUE);
        ShowWindow(buttonHwnd, 1);
}

void OSDrawButtonLike(GUI_BUTTON_LIKE button, int scrW, int scrH) {

    HWND buttonHwnd = button.buttonLoc;

    //Get percentage based x,y,w,h
    const GUI_POINT start = (GUI_POINT){button.rect.x * scrW / 100, button.rect.y * scrH / 100};
    const GUI_POINT end = (GUI_POINT){button.rect.w * scrW / 100, button.rect.h * scrH / 100};

    char buff[32];
    GetClassName(buttonHwnd, buff, 32);

    RECT rect;
    GetWindowRect(buttonHwnd, &rect);
    MapWindowPoints(HWND_DESKTOP, GetParent(buttonHwnd), (POINT*)&rect, 2);

    //Only move if needed
    if ((strcmp(buff, "ComboBox") != 0 && strcmp(buff, "ListBox") != 0) || (start.x != rect.left || start.y != rect.top))
        MoveWindow(buttonHwnd, start.x, start.y, end.x, end.y, TRUE);

    if (strcmp(buff, "Edit") != 0)
        SetWindowText(buttonHwnd, button.text);
    ShowWindow(buttonHwnd, button.shouldShow);
}

void OSDoAfterMillis(Window* wnd, const unsigned int id, const unsigned int millis, void (*func)(Window* wnd)) {
    doAfters[id - 1].doAfterFunc = func;
    doAfters[id - 1].wnd = wnd;
    SetTimer(wnd->wndHwnd, id, millis, NULL);
}

void OSKillTimer(const Window* wnd, const unsigned int id) {
    KillTimer(wnd->wndHwnd, id);
}

void OSShowButtonById(const Window* wnd, const unsigned int id, const unsigned int shouldShow) {
    ShowWindow(GetDlgItem(wnd->wndHwnd, id), shouldShow);
}

void OSDestroyButtonById(const Window* wnd, const unsigned int id) {
    DestroyWindow(GetDlgItem(wnd->wndHwnd, id));
}

const char* OSGetInputBoxTextById(const Window* wnd, const unsigned int id) {
    HWND hwnd = GetDlgItem(wnd->wndHwnd, id);
    const unsigned int textLength = GetWindowTextLength(hwnd)+1;
    char* text = malloc(textLength);
    GetWindowText(hwnd, text, textLength);
    return text;
}

const char* OSGetFilePath() {
    OPENFILENAME ofn;

    char* tmp = calloc(1, MAX_FILE_SIZE);
    if (!tmp) { return NULL; }
    char* szFile = tmp;

    HWND hwnd = rootHwnd;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_FILE_SIZE;

    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;

    GetOpenFileName(&ofn);

    return szFile;
}

const char* OSGetDirectoryPath() {
    BROWSEINFO info = {0};
    info.hwndOwner = rootHwnd;
    info.lpszTitle = "Where do you want to download to?";
    info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolder(&info);
    if (pidl == NULL) {
        CoTaskMemFree(NULL);
        return NULL;
    }

    char* path = calloc(1, 512);
    SHGetPathFromIDList(pidl, path);
    CoTaskMemFree(pidl);
    return path;
}

void OSGetEXEDir(char* buffer, const int size) {
    GetModuleFileName(NULL, buffer, size);
    PathRemoveFileSpec(buffer);
}

const char* OSGetDirectoryPathInsideWikiAnalyser(const char* localPath) {
    //Get base dir
    char basePath[512]; OSGetEXEDir(basePath, 512);
    char projectsPath[768];

    sprintf(projectsPath, "%s\\%s", basePath, localPath);

    BROWSEINFO info = {0};
    info.hwndOwner = rootHwnd;
    info.lpszTitle = "Where do you want to download to?";
    info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    LPITEMIDLIST pidlRoot = ILCreateFromPath(projectsPath);
    info.pidlRoot = (LPCITEMIDLIST)pidlRoot;

    LPITEMIDLIST pidl = SHBrowseForFolder(&info);
    if (pidl == NULL) {
        CoTaskMemFree(NULL);
        return NULL;
    }

    char* path = calloc(1, 1024);
    SHGetPathFromIDList(pidl, path);
    CoTaskMemFree(pidl);

    ILFree(pidlRoot);
    return path;
}

void OSOpenAs(const char* projectDir, const char* filename) {
    char fullPath[512]; sprintf(fullPath, "%s\\%s", projectDir, filename);
    ShellExecute(NULL, "openas", fullPath, NULL, NULL, SW_SHOWNORMAL);
}

int OSShellExecute(const char* projectDir, const char* command) {
    STARTUPINFO si = {0};
    si.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION pi = {0};

    char cmd[512]; strcpy(cmd, command);

    CreateProcess(
        NULL,
        cmd,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        projectDir,
        &si,
        &pi
    );

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return (int)exitCode;
}

void OSCreateDirectory(const char* dirName) {
    _mkdir(dirName);
}

void OSFreeLibrary() {
    if (zigLib != NULL) {
        OnArticle = NULL;
        FreeLibrary(zigLib);
        zigLib = NULL;
    }
}

void* OSLoadLibrary(const char* libPath, const char* funcName) {
    if (zigLib != NULL) {
        OnArticle = NULL;
        FreeLibrary(zigLib);
        zigLib = NULL;
    }
    Sleep(1000);
    zigLib = LoadLibrary(libPath);
    return GetProcAddress(zigLib, funcName);
}

DWORD WINAPI DownloadToThread(LPVOID param) {
    DownloadToStruct* downloadToStruct = param;
    CurlDownloadTo(downloadToStruct->url, downloadToStruct->filePath, downloadToStruct->fileName);
    free(downloadToStruct);
    return 0;
}

DWORD WINAPI DownloadSpecialExportToThread(LPVOID param) {
    DownloadSpecialExportToStruct* downloadSpecialExportToStruct = param;
    CurlDownloadWithSpecialExportTo(downloadSpecialExportToStruct->name, downloadSpecialExportToStruct->count,
        downloadSpecialExportToStruct->filePath, downloadSpecialExportToStruct->fileName);
    free(downloadSpecialExportToStruct);
    return 0;
}

DWORD WINAPI ParseThread(LPVOID param) {
    ParseStruct* parseStruct = param;
    ParseArticles(parseStruct->szFilePath, parseStruct->articles, parseStruct->baseArticle, parseStruct->articleCount);
    free(parseStruct);
    return 0;
}

void OSCreateThreadForFunction(const LPTHREAD_START_ROUTINE function, const LPVOID funcStruct) {
    CreateThread(
        NULL,
        0,
        function,
        funcStruct,
        0,
        NULL
    );
}

void OSCreateThreadForDownloadTo(const char* url, const char* filePath, const char* fileName) {
    DownloadToStruct* downloadToStruct = malloc(sizeof(DownloadToStruct));
    downloadToStruct->url = url; downloadToStruct->filePath = filePath; downloadToStruct->fileName = fileName;
    OSCreateThreadForFunction(DownloadToThread, downloadToStruct);
}

void OSCreateThreadForDownloadSpecialExportTo(const char** name, unsigned int count, const char* filePath, const char* fileName) {
    DownloadSpecialExportToStruct* downloadSpecialExportToStruct = malloc(sizeof(DownloadSpecialExportToStruct));
    downloadSpecialExportToStruct->name = name; downloadSpecialExportToStruct->count = count;
    downloadSpecialExportToStruct->filePath = filePath; downloadSpecialExportToStruct->fileName = fileName;
    OSCreateThreadForFunction(DownloadSpecialExportToThread, downloadSpecialExportToStruct);
}

void OSCreateThreadForParse(PCSTRFILEPATH szFilePath, Article **articles, Article *baseArticle, unsigned int *articleCount) {
    ParseStruct* parseStruct = malloc(sizeof(ParseStruct));
    parseStruct->szFilePath = szFilePath; parseStruct->articles = articles; parseStruct->baseArticle = baseArticle; parseStruct->articleCount = articleCount;
    OSCreateThreadForFunction(ParseThread, parseStruct);
}

int OSGetDropdownCurrentlySelected(const unsigned int id, Window* wnd) {
    return SendMessage(GetDlgItem(wnd->wndHwnd, id), CB_GETCURSEL, 0, 0);
}

int OSGetDropdownMultiSelectCurrentlySelected(const unsigned int id, Window* wnd, int* buffer, const int bufferSize) {
    HWND list = GetDlgItem(wnd->wndHwnd, id);

    int count = SendMessage(list, LB_GETSELCOUNT, 0, 0);
    if (count <= 0) return 0;
    if (count > bufferSize) count = bufferSize;

    SendMessage(list, LB_GETSELITEMS, (WPARAM)count, (LPARAM)buffer);

    return count;
}

int GetRefreshRate() {
    DEVMODE dev = {};
    dev.dmSize = sizeof(dev);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dev);
    return dev.dmDisplayFrequency;
}

