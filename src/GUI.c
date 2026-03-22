//
// Created by halet on 2/11/2026.
//


#include "../include/GUI.h"


#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../include/curl.h"
#include "../include/mainGUI.h"
#include "../include/topN.h"
#include "../include/welcomeGUI.h"

#ifdef __WIN32__
#include "../include/WINGUI.h"
#endif

PaintStacks paintStacks = {0};
DO_AFTER_ENTRY doAfters[5] = {0};
BUTTON_COMMAND_ENTRY buttonCommands[32] = {0};

volatile int progress = 0;
int maxProgress = 1;
void(*progressDoneFunc)(void) = NULL;
int indeterminate = 0;

//Draw functions that add themselves to paintStacks until manually removed
COLOUR_RECT* DrawPermanentRectToPaintStacks(COLOUR_RECT colourRect, PaintStacks* ps) {
    ps->colourRectsSize++;
    COLOUR_RECT* tmp = realloc(ps->colourRects, ps->colourRectsSize*sizeof(COLOUR_RECT));
    if (!tmp) { return NULL; }
    ps->colourRects = tmp;
    ps->colourRects[ps->colourRectsSize-1] = colourRect;
    return &ps->colourRects[ps->colourRectsSize-1];
}

COLOUR_LINE* DrawPermanentLineToPaintStacks(COLOUR_LINE colourLine, PaintStacks* ps) {
    ps->colourLinesSize++;
    COLOUR_LINE* tmp = realloc(ps->colourLines, ps->colourLinesSize*sizeof(COLOUR_LINE));
    if (!tmp) { return NULL; }
    ps->colourLines = tmp;
    ps->colourLines[ps->colourLinesSize-1] = colourLine;
    return &ps->colourLines[ps->colourLinesSize-1];
}

COLOUR_LINE_CHAIN* DrawPermanentLineChainToPaintStacks(COLOUR_LINE_CHAIN colourLineChain, PaintStacks* ps) {
    ps->colourLinesChainsSize++;
    COLOUR_LINE_CHAIN* tmp = realloc(ps->colourLineChains, ps->colourLinesChainsSize*sizeof(COLOUR_LINE_CHAIN));
    if (!tmp) { return NULL; }
    ps->colourLineChains = tmp;

    //Deep copy points
    GUI_POINT* points = malloc(colourLineChain.linePath.pointsSize * sizeof(GUI_POINT));
    if (!points) return NULL;
    memcpy(points, colourLineChain.linePath.points, colourLineChain.linePath.pointsSize * sizeof(GUI_POINT));
    colourLineChain.linePath.points = points;

    ps->colourLineChains[ps->colourLinesChainsSize-1] = colourLineChain;
    return &ps->colourLineChains[ps->colourLinesChainsSize-1];
}

COLOUR_POINT* DrawPermanentPointToPaintStacks(COLOUR_POINT colourPoint, PaintStacks* ps) {
    ps->colourPointsSize++;
    COLOUR_POINT* tmp = realloc(ps->colourPoints, ps->colourPointsSize*sizeof(COLOUR_POINT));
    if (!tmp) { return NULL; }
    ps->colourPoints = tmp;
    ps->colourPoints[ps->colourPointsSize-1] = colourPoint;
    return &ps->colourPoints[ps->colourPointsSize-1];
}

GUI_TEXT* DrawPermanentTextToPaintStacks(GUI_TEXT text, PaintStacks* ps) {
    ps->textsSize++;
    GUI_TEXT* tmp = realloc(ps->texts, ps->textsSize*sizeof(GUI_TEXT));
    if (!tmp) { return NULL; }
    ps->texts = tmp;
    ps->texts[ps->textsSize-1] = text;
    return &ps->texts[ps->textsSize-1];
}

GUI_IMAGE* DrawPermanentImageToPaintStacks(GUI_IMAGE image, PaintStacks* ps) {
    ps->imagesSize++;
    GUI_IMAGE* tmp = realloc(ps->images, ps->imagesSize*sizeof(GUI_IMAGE));
    if (!tmp) { return NULL; }
    ps->images = tmp;
    ps->images[ps->imagesSize-1] = image;
    return &ps->images[ps->imagesSize-1];
}

Window** DrawPermanentWindowToPaintStacks(const Window *wnd, PaintStacks* ps) {
    ps->windowsSize++;
    Window* tmp = realloc(ps->windows, ps->windowsSize*sizeof(Window*));
    if (!tmp) { return NULL; }
    ps->windows = tmp;
    ps->windows[ps->windowsSize-1] = wnd;
    return &ps->windows[ps->windowsSize-1];
}

GUI_BUTTON_LIKE* DrawPermanentButtonToPaintStacks(GUI_BUTTON_LIKE button, PaintStacks* ps){
    button.shouldShow = 1; //Show by default
    ps->buttonsSize++;
    GUI_BUTTON_LIKE* tmp = realloc(ps->buttons, ps->buttonsSize*sizeof(GUI_BUTTON_LIKE));
    if (!tmp) { return NULL; }
    ps->buttons = tmp;
    ps->buttons[ps->buttonsSize-1] = button;
    return &ps->buttons[ps->buttonsSize-1];
}

COLOUR_RECT* DrawPermanentRect(COLOUR_RECT colourRect, Window* wnd) {return DrawPermanentRectToPaintStacks(colourRect, &wnd->paintStacks);}
COLOUR_LINE* DrawPermanentLine(COLOUR_LINE colourLine, Window* wnd) {return DrawPermanentLineToPaintStacks(colourLine, &wnd->paintStacks);}
COLOUR_LINE_CHAIN* DrawPermanentLineChain(COLOUR_LINE_CHAIN colourLineChain, Window* wnd) {return DrawPermanentLineChainToPaintStacks(colourLineChain, &wnd->paintStacks);}
GUI_TEXT* DrawPermanentText(GUI_TEXT text, Window* wnd) {return DrawPermanentTextToPaintStacks(text, &wnd->paintStacks);}
COLOUR_POINT* DrawPermanentPoint(COLOUR_POINT colourPoint, Window* wnd) {return DrawPermanentPointToPaintStacks(colourPoint, &wnd->paintStacks);}
GUI_IMAGE* DrawPermanentImage(GUI_IMAGE image, Window* wnd) {return DrawPermanentImageToPaintStacks(image, &wnd->paintStacks);}
Window** DrawPermanentWindow(const Window *wnd, Window* parentWnd) {return DrawPermanentWindowToPaintStacks(wnd, &parentWnd->paintStacks);}
GUI_BUTTON_LIKE* DrawPermanentButton(GUI_BUTTON_LIKE button, Window* wnd) {return DrawPermanentButtonToPaintStacks(button, &wnd->paintStacks);}

void ShowButtonLike(GUI_BUTTON_LIKE* button, const unsigned int shouldShow) {
    button->shouldShow = shouldShow;
}

GUI_RECT GetButtonPos(const int totalButtons, const GUI_POINT center, const int buttonSize, const int buttonNumber) {
    const int t = totalButtons*buttonSize;
    const GUI_POINT start = (GUI_POINT){center.x-t/2, center.y}; 
    return (GUI_RECT){start.x+(buttonNumber*buttonSize)-buttonSize, start.y, buttonSize, buttonSize};
}

GUI_RECT GetButtonPosWH(const int totalButtons, const GUI_POINT center, const int buttonSizeW, const int buttonSizeH, const int buttonNumber) {
    const int t = totalButtons*buttonSizeW;
    const GUI_POINT start = (GUI_POINT){center.x-t/2, center.y};
    return (GUI_RECT){start.x+(buttonNumber*buttonSizeW)-buttonSizeW, start.y, buttonSizeW, buttonSizeH};
}

void ClearGUIFull(PaintStacks* paintStacks, DO_AFTER_ENTRY* doAfters[5], BUTTON_COMMAND_ENTRY* buttonCommands[32]) {
    *paintStacks = (PaintStacks){0};
    *doAfters = NULL;
    *buttonCommands = NULL;
}

void ClearGUI(PaintStacks* paintStacks) {
    *paintStacks = (PaintStacks){0};
}

void GUIStart(Window* wnd) {
    //CreateTopNFile();
    //WelcomeGUI(wnd, MainGUIStart);
    MainGUIStart(wnd);
}

void GUILoop() {

}
