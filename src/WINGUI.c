//
// Created by halet on 2/10/2026.
//

#include "../include/WINGUI.h"

#include <stdio.h>

;

const wchar_t CLASS_NAME[] = L"WINDOW CLASS";
WNDCLASS wc = {};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_TIMER:
            if (wParam == 1) {
                InvalidateRect(hwnd, NULL, FALSE);
            } else {
                doAfters[wParam - 1].doAfterFunc();
            }
            return 0;
        case WM_PAINT:
        {
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

            int i;
            for (i = 0; i < paintStacks.buttonsSize; i++)
                OSDrawButtonLike(paintStacks.buttons[i], windowWidth, windowHeight);

            for (i = 0; i < paintStacks.imagesSize; i++)
                OSDrawImage(memDC, paintStacks.images[i], windowWidth, windowHeight);

            for (i = 0; i < paintStacks.colourRectsSize; i++)
                OSDrawRect(memDC, paintStacks.colourRects[i], windowWidth, windowHeight);

            for (i = 0; i < paintStacks.colourLinesSize; i++)
                OSDrawLine(memDC, paintStacks.colourLines[i], windowWidth, windowHeight);

            for (i = 0; i < paintStacks.colourLinesChainsSize; i++)
                OSDrawLineChain(memDC, paintStacks.colourLineChains[i], windowWidth,windowHeight);

            for (i = 0; i < paintStacks.colourPointsSize; i++)
                OSDrawPoint(memDC, paintStacks.colourPoints[i], windowWidth, windowHeight);

            for (i = 0; i < paintStacks.textsSize; i++)
                OSDrawText(memDC, paintStacks.texts[i], windowWidth, windowHeight);



            BitBlt(hdc, 0, 0, windowWidth, windowHeight, memDC, 0, 0, SRCCOPY);

            DeleteObject(memBmp);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
            return 0;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void OSCreateWindowClass() {
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;


    RegisterClass(&wc);
}

HWND hwnd;
void OSCreateWindow() {
    hwnd = CreateWindowEx(
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

    if (hwnd == NULL)
    {
        return;
    }

    SetTimer(hwnd, 1, min(1000 / 120, 1000 / GetRefreshRate()), NULL);

    ShowWindow(hwnd, 1);
}

void OSMessageLoop() {
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        GUILoop();
    }
}

void* OSCreateButton() {
    HWND buttonHwnd = CreateWindowEx(
        0,
        "BUTTON",
        "",
        WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0,0,0,0,
        hwnd,
        (HMENU)1,
        wc.hInstance,
        NULL
    );
    return buttonHwnd;
}

void* OSCreateCheckBox() {
    HWND buttonHwnd = CreateWindowEx(
        0,
        "BUTTON",
        "",
        WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        0,0,0,0,
        hwnd,
        (HMENU)1,
        wc.hInstance,
        NULL
    );
    return buttonHwnd;
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

void OSDrawText(HDC hdc, GUI_TEXT text, int scrW, int scrH) {
    text.size = text.size * min(scrW / 5, scrH) / 100;

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

void OSDrawButtonLike(GUI_BUTTON_LIKE button, int scrW, int scrH) {
    HWND buttonHwnd = button.buttonLoc;

    //Get percentage based x,y,w,h
    const GUI_POINT start = (GUI_POINT){button.rect.x * scrW / 100, button.rect.y * scrH / 100};
    const GUI_POINT end = (GUI_POINT){button.rect.w * scrW / 100, button.rect.h * scrH / 100};

    MoveWindow(buttonHwnd, start.x, start.y, end.x, end.y, TRUE);
    SetWindowText(buttonHwnd, button.text);
    ShowWindow(buttonHwnd, 1);
}

void OSDoAfterMillis(const unsigned int id, const unsigned int millis, void (*func)(void)) {
    doAfters[id - 1].doAfterFunc = func;
    SetTimer(hwnd, id, millis, NULL);
}

void OSKillTimer(const unsigned int id) {
    KillTimer(hwnd, id);
}

int GetRefreshRate() {
    DEVMODE dev = {};
    dev.dmSize = sizeof(dev);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dev);
    return dev.dmDisplayFrequency;
}