//
// Created by halet on 2/10/2026.
//

#include "../include/WINGUI.h"

#include <stdio.h>

const wchar_t CLASS_NAME[] = L"WINDOW CLASS";
WNDCLASS wc = {};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_TIMER:
            InvalidateRect(hwnd, NULL, FALSE);
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            const HDC hdc = BeginPaint(hwnd, &ps);

            RECT windowRect;
            GetClientRect(hwnd, &windowRect);
            const int windowWidth = windowRect.right - windowRect.left;
            const int windowHeight = windowRect.bottom - windowRect.top;

            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBmp = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
            SelectObject(memDC, memBmp);

            //Clear Background
            FillRect(memDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));

            int i;
            for (i = 0; i < paintStacks.colourRectsSize; i++) {
                OSDrawRect(memDC, paintStacks.colourRects[i], windowWidth, windowHeight);
            }

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
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);
}

void OSCreateWindow() {
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "WikiAnalyser",
        WS_OVERLAPPEDWINDOW,
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
    }
}

void OSDrawRect(HDC hdc, COLOUR_RECT colourRect, int scrW, int scrH) {
    const GUI_RECT rect = colourRect.rect;
    const GUI_RECT realRect = (GUI_RECT){(rect.x * scrW / 100), (rect.y * scrH / 100), (rect.w * scrW / 100), (rect.h * scrH / 100)};
    const COLOUR colour = colourRect.colour;
    //The +1 ensures no 1 pixel white gaps caused by integer division
    FillRect(hdc, &(RECT){realRect.x, realRect.y, realRect.w + 1, realRect.h + 1}, CreateSolidBrush(RGB(colour.r, colour.g, colour.b)));
}

int GetRefreshRate() {
    DEVMODE dev = {};
    dev.dmSize = sizeof(dev);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dev);
    return dev.dmDisplayFrequency;
}