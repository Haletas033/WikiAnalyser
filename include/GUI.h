//
// Created by halet on 2/11/2026.
//

#ifndef GUI_H
#define GUI_H

#include <stdlib.h>
#include "config.h"
#include <math.h>


#define IDT_DO_AFTER 2
#define IDT_DO_AFTER_2 3
#define IDT_DO_AFTER_3 4
#define IDT_DO_AFTER_4 5
#define IDT_DO_AFTER_5 6

#define MAX_FILE_SIZE 260

#define DEG_TO_RAD(deg) deg*(M_PI/180)

typedef struct GUI_RECT { int x, y, w, h; } GUI_RECT;

typedef struct GUI_POINT { int x, y; } GUI_POINT;

typedef struct GUI_LINE { GUI_POINT p1, p2; int thickness; } GUI_LINE;

typedef struct GUI_LINE_CHAIN { GUI_POINT* points; unsigned int pointsSize; int thickness; } GUI_LINE_CHAIN;

typedef struct GUI_TEXT {char* text; GUI_POINT pos; int size;} GUI_TEXT;

typedef struct GUI_IMAGE {GUI_RECT rect; void* imgLoc;} GUI_IMAGE;

typedef struct GUI_PIE_SLICE {GUI_POINT center; int radius; int startDegrees; int endDegrees;} GUI_PIE_SLICE;

typedef struct GUI_BUTTON_LIKE {const char* text; GUI_RECT rect; void* buttonLoc; unsigned int shouldShow;} GUI_BUTTON_LIKE;

typedef struct COLOUR{ int r, g, b; } COLOUR;

typedef struct COLOUR_RECT { GUI_RECT rect; COLOUR colour; } COLOUR_RECT;

typedef struct COLOUR_LINE { GUI_LINE line; COLOUR colour; } COLOUR_LINE;

typedef struct COLOUR_LINE_CHAIN { GUI_LINE_CHAIN linePath; COLOUR colour; } COLOUR_LINE_CHAIN;

typedef struct COLOUR_POINT { GUI_POINT point; int radius; COLOUR colour; } COLOUR_POINT;

typedef struct COLOUR_PIE_SLICE { GUI_PIE_SLICE pieSlice; COLOUR colour; } COLOUR_PIE_SLICE;

typedef struct Window Window;

typedef struct PaintStacks {
    COLOUR_RECT* colourRects;
    COLOUR_LINE* colourLines;
    COLOUR_LINE_CHAIN* colourLineChains;
    COLOUR_POINT* colourPoints;
    COLOUR_PIE_SLICE* colourPieSlices;
    GUI_TEXT* texts;
    GUI_IMAGE* images;
    Window** windows;
    GUI_BUTTON_LIKE* buttons;

    unsigned int colourRectsSize;
    unsigned int colourLinesSize;
    unsigned int colourLinesChainsSize;
    unsigned int colourPointsSize;
    unsigned int colourPieSlicesSize;
    unsigned int textsSize;
    unsigned int imagesSize;
    unsigned int windowsSize;
    unsigned int buttonsSize;
} PaintStacks;

typedef struct Window {
    GUI_RECT windowRect;
    PaintStacks paintStacks;
    void* wndHwnd;
} Window;

typedef struct DO_AFTER_ENTRY {
    void (*doAfterFunc)(Window* wnd);
    Window* wnd;
} DO_AFTER_ENTRY;

typedef struct BUTTON_COMMAND_ENTRY {
    void (*buttonCommand)(Window* wnd);
    Window* wnd;
} BUTTON_COMMAND_ENTRY;

extern PaintStacks paintStacks;
extern DO_AFTER_ENTRY doAfters[5];
extern BUTTON_COMMAND_ENTRY buttonCommands[512];

extern volatile int progress;
extern int maxProgress;
extern void(*progressDoneFunc)(void);
extern int indeterminate;
extern int currentButtonId;

typedef struct Layout {
    Window* windows;
} Layout;

COLOUR_RECT* DrawPermanentRect(COLOUR_RECT colourRect, Window* wnd);
COLOUR_LINE* DrawPermanentLine(COLOUR_LINE colourLine, Window* wnd);
COLOUR_LINE_CHAIN* DrawPermanentLineChain(COLOUR_LINE_CHAIN colourLineChain, Window* wnd);
GUI_TEXT* DrawPermanentText(GUI_TEXT text, Window* wnd);
COLOUR_POINT* DrawPermanentPoint(COLOUR_POINT colourPoint, Window* wnd);
GUI_IMAGE* DrawPermanentImage(GUI_IMAGE image, Window* wnd);
Window** DrawPermanentWindow(const Window *wnd, Window* parentWnd);
GUI_BUTTON_LIKE* DrawPermanentButton(GUI_BUTTON_LIKE button, Window* wnd);

COLOUR_RECT* DrawPermanentRectToPaintStacks(COLOUR_RECT colourRect, PaintStacks* ps);
COLOUR_LINE* DrawPermanentLineToPaintStacks(COLOUR_LINE colourLine, PaintStacks* ps);
COLOUR_LINE_CHAIN* DrawPermanentLineChainToPaintStacks(COLOUR_LINE_CHAIN colourLineChain, PaintStacks* ps);
COLOUR_POINT* DrawPermanentPointToPaintStacks(COLOUR_POINT colourPoint, PaintStacks* ps);
GUI_TEXT* DrawPermanentTextToPaintStacks(GUI_TEXT text, PaintStacks* ps);
GUI_IMAGE* DrawPermanentImageToPaintStacks(GUI_IMAGE image, PaintStacks* ps);
Window** DrawPermanentWindowToPaintStacks(const Window *wnd, PaintStacks* ps);
GUI_BUTTON_LIKE* DrawPermanentButtonToPaintStacks(GUI_BUTTON_LIKE button, PaintStacks* ps);

void ShowButtonLike(GUI_BUTTON_LIKE* button, unsigned int shouldShow);
GUI_RECT GetButtonPos(int totalButtons, GUI_POINT center, int buttonSize, int buttonNumber);
GUI_RECT GetButtonPosWH(int totalButtons, GUI_POINT center, int buttonSizeW, int buttonSizeH, int buttonNumber);

void ClearGUIFull(PaintStacks* paintStacks, DO_AFTER_ENTRY* doAfters[5], BUTTON_COMMAND_ENTRY* buttonCommands[32]);
void ClearGUI(PaintStacks* paintStacks);

void GUIStart(Window* wnd);

void GUILoop();


#endif //GUI_H
