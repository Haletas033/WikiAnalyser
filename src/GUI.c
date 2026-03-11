//
// Created by halet on 2/11/2026.
//


#include "../include/GUI.h"


#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../include/curl.h"
#include "../include/mainGUI.h"
#include "../include/welcomeGUI.h"

#ifdef __WIN32__
#include "../include/WINGUI.h"
#endif

PaintStacks paintStacks = {0};
DO_AFTER_ENTRY doAfters[5] = {0};
BUTTON_COMMAND_ENTRY buttonCommands[32] = {0};

//Draw functions that add themselves to paintStacks until manually removed
COLOUR_RECT* DrawPermanentRect(COLOUR_RECT colourRect, Window* wnd) {
    wnd->paintStacks.colourRectsSize++;
    COLOUR_RECT* tmp = realloc(wnd->paintStacks.colourRects, wnd->paintStacks.colourRectsSize*sizeof(COLOUR_RECT));
    if (!tmp) { return NULL; }
    wnd->paintStacks.colourRects = tmp;
    wnd->paintStacks.colourRects[wnd->paintStacks.colourRectsSize-1] = colourRect;
    return &wnd->paintStacks.colourRects[wnd->paintStacks.colourRectsSize-1];
}

COLOUR_LINE* DrawPermanentLine(COLOUR_LINE colourLine, Window* wnd) {
    wnd->paintStacks.colourLinesSize++;
    COLOUR_LINE* tmp = realloc(wnd->paintStacks.colourLines, wnd->paintStacks.colourLinesSize*sizeof(COLOUR_LINE));
    if (!tmp) { return NULL; }
    wnd->paintStacks.colourLines = tmp;
    wnd->paintStacks.colourLines[wnd->paintStacks.colourLinesSize-1] = colourLine;
    return &wnd->paintStacks.colourLines[wnd->paintStacks.colourLinesSize-1];
}

COLOUR_LINE_CHAIN* DrawPermanentLineChain(COLOUR_LINE_CHAIN colourLineChain, Window* wnd) {
    wnd->paintStacks.colourLinesChainsSize++;
    COLOUR_LINE_CHAIN* tmp = realloc(wnd->paintStacks.colourLineChains, wnd->paintStacks.colourLinesChainsSize*sizeof(COLOUR_LINE_CHAIN));
    if (!tmp) { return NULL; }
    wnd->paintStacks.colourLineChains = tmp;

    //Deep copy points
    GUI_POINT* points = malloc(colourLineChain.linePath.pointsSize * sizeof(GUI_POINT));
    if (!points) return NULL;
    memcpy(points, colourLineChain.linePath.points, colourLineChain.linePath.pointsSize * sizeof(GUI_POINT));
    colourLineChain.linePath.points = points;

    wnd->paintStacks.colourLineChains[wnd->paintStacks.colourLinesChainsSize-1] = colourLineChain;
    return &wnd->paintStacks.colourLineChains[wnd->paintStacks.colourLinesChainsSize-1];
}

COLOUR_POINT* DrawPermanentPoint(COLOUR_POINT colourPoint, Window* wnd) {
    wnd->paintStacks.colourPointsSize++;
    COLOUR_POINT* tmp = realloc(wnd->paintStacks.colourPoints, wnd->paintStacks.colourPointsSize*sizeof(COLOUR_POINT));
    if (!tmp) { return NULL; }
    wnd->paintStacks.colourPoints = tmp;
    wnd->paintStacks.colourPoints[wnd->paintStacks.colourPointsSize-1] = colourPoint;
    return &wnd->paintStacks.colourPoints[wnd->paintStacks.colourPointsSize-1];
}

GUI_TEXT* DrawPermanentText(GUI_TEXT text, Window* wnd) {
    wnd->paintStacks.textsSize++;
    GUI_TEXT* tmp = realloc(wnd->paintStacks.texts, wnd->paintStacks.textsSize*sizeof(GUI_TEXT));
    if (!tmp) { return NULL; }
    wnd->paintStacks.texts = tmp;
    wnd->paintStacks.texts[wnd->paintStacks.textsSize-1] = text;
    return &wnd->paintStacks.texts[wnd->paintStacks.textsSize-1];
}

GUI_IMAGE* DrawPermanentImage(GUI_IMAGE image, Window* wnd) {
    wnd->paintStacks.imagesSize++;
    GUI_IMAGE* tmp = realloc(wnd->paintStacks.images, wnd->paintStacks.imagesSize*sizeof(GUI_IMAGE));
    if (!tmp) { return NULL; }
    wnd->paintStacks.images = tmp;
    wnd->paintStacks.images[wnd->paintStacks.imagesSize-1] = image;
    return &wnd->paintStacks.images[wnd->paintStacks.imagesSize-1];
}

Window **DrawPermanentWindow(const Window *wnd, Window *parentWnd) {
    parentWnd->paintStacks.windowsSize++;
    Window* tmp = realloc(parentWnd->paintStacks.windows, parentWnd->paintStacks.windowsSize*sizeof(Window*));
    if (!tmp) { return NULL; }
    parentWnd->paintStacks.windows = tmp;
    parentWnd->paintStacks.windows[parentWnd->paintStacks.windowsSize-1] = wnd;
    return &parentWnd->paintStacks.windows[parentWnd->paintStacks.windowsSize-1];
}

GUI_BUTTON_LIKE* DrawPermanentButton(GUI_BUTTON_LIKE button, Window* wnd){
    wnd->paintStacks.buttonsSize++;
    GUI_BUTTON_LIKE* tmp = realloc(wnd->paintStacks.buttons, wnd->paintStacks.buttonsSize*sizeof(GUI_BUTTON_LIKE));
    if (!tmp) { return NULL; }
    wnd->paintStacks.buttons = tmp;
    wnd->paintStacks.buttons[wnd->paintStacks.buttonsSize-1] = button;
    return &wnd->paintStacks.buttons[wnd->paintStacks.buttonsSize-1];
}

GUI_RECT GetButtonPos(const int totalButtons, const GUI_POINT center, const int buttonSize, const int buttonNumber) {
    const int t = totalButtons*buttonSize;
    const GUI_POINT start = (GUI_POINT){center.x-t/2, center.y}; 
    return (GUI_RECT){start.x+(buttonNumber*buttonSize)-buttonSize, start.y, buttonSize, buttonSize};
}

void ClearGUI(PaintStacks* paintStacks, DO_AFTER_ENTRY* doAfters[5], BUTTON_COMMAND_ENTRY* buttonCommands[32]) {
    *paintStacks = (PaintStacks){0};
    *doAfters = NULL;
    *buttonCommands = NULL;
}

void GUIStart(Window* wnd) {
    WelcomeGUI(wnd, MainGUIStart);
}

void GUILoop() {

}
