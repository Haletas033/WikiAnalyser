//
// Created by halet on 2/11/2026.
//


#include "../include/GUI.h"


#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../include/welcomeGUI.h"

#ifdef __WIN32__
#include "../include/WINGUI.h"
#endif

PaintStacks paintStacks = {0};
DO_AFTER_ENTRY doAfters[5] = {0};
BUTTON_COMMAND_ENTRY buttonCommands[32] = {0};

//Draw functions that add themselves to paintStacks until manually removed
COLOUR_RECT* DrawPermanentRect(COLOUR_RECT colourRect) {
    paintStacks.colourRectsSize++;
    COLOUR_RECT* tmp = realloc(paintStacks.colourRects, paintStacks.colourRectsSize*sizeof(COLOUR_RECT));
    if (!tmp) { return NULL; }
    paintStacks.colourRects = tmp;
    paintStacks.colourRects[paintStacks.colourRectsSize-1] = colourRect;
    return &paintStacks.colourRects[paintStacks.colourRectsSize-1];
}

COLOUR_LINE* DrawPermanentLine(COLOUR_LINE colourLine) {
    paintStacks.colourLinesSize++;
    COLOUR_LINE* tmp = realloc(paintStacks.colourLines, paintStacks.colourLinesSize*sizeof(COLOUR_LINE));
    if (!tmp) { return NULL; }
    paintStacks.colourLines = tmp;
    paintStacks.colourLines[paintStacks.colourLinesSize-1] = colourLine;
    return &paintStacks.colourLines[paintStacks.colourLinesSize-1];
}

COLOUR_LINE_CHAIN* DrawPermanentLineChain(COLOUR_LINE_CHAIN colourLineChain) {
    paintStacks.colourLinesChainsSize++;
    COLOUR_LINE_CHAIN* tmp = realloc(paintStacks.colourLineChains, paintStacks.colourLinesChainsSize*sizeof(COLOUR_LINE_CHAIN));
    if (!tmp) { return NULL; }
    paintStacks.colourLineChains = tmp;

    //Deep copy points
    GUI_POINT* points = malloc(colourLineChain.linePath.pointsSize * sizeof(GUI_POINT));
    if (!points) return NULL;
    memcpy(points, colourLineChain.linePath.points, colourLineChain.linePath.pointsSize * sizeof(GUI_POINT));
    colourLineChain.linePath.points = points;

    paintStacks.colourLineChains[paintStacks.colourLinesChainsSize-1] = colourLineChain;
    return &paintStacks.colourLineChains[paintStacks.colourLinesChainsSize-1];
}

COLOUR_POINT* DrawPermanentPoint(COLOUR_POINT colourPoint) {
    paintStacks.colourPointsSize++;
    COLOUR_POINT* tmp = realloc(paintStacks.colourPoints, paintStacks.colourPointsSize*sizeof(COLOUR_POINT));
    if (!tmp) { return NULL; }
    paintStacks.colourPoints = tmp;
    paintStacks.colourPoints[paintStacks.colourPointsSize-1] = colourPoint;
    return &paintStacks.colourPoints[paintStacks.colourPointsSize-1];
}

GUI_TEXT* DrawPermanentText(GUI_TEXT text) {
    paintStacks.textsSize++;
    GUI_TEXT* tmp = realloc(paintStacks.texts, paintStacks.textsSize*sizeof(GUI_TEXT));
    if (!tmp) { return NULL; }
    paintStacks.texts = tmp;
    paintStacks.texts[paintStacks.textsSize-1] = text;
    return &paintStacks.texts[paintStacks.textsSize-1];
}

GUI_IMAGE* DrawPermanentImage(GUI_IMAGE image) {
    paintStacks.imagesSize++;
    GUI_IMAGE* tmp = realloc(paintStacks.images, paintStacks.imagesSize*sizeof(GUI_IMAGE));
    if (!tmp) { return NULL; }
    paintStacks.images = tmp;
    paintStacks.images[paintStacks.imagesSize-1] = image;
    return &paintStacks.images[paintStacks.imagesSize-1];
}

GUI_BUTTON_LIKE* DrawPermanentButton(GUI_BUTTON_LIKE button){
    paintStacks.buttonsSize++;
    GUI_BUTTON_LIKE* tmp = realloc(paintStacks.buttons, paintStacks.buttonsSize*sizeof(GUI_BUTTON_LIKE));
    if (!tmp) { return NULL; }
    paintStacks.buttons = tmp;
    paintStacks.buttons[paintStacks.buttonsSize-1] = button;
    return &paintStacks.buttons[paintStacks.buttonsSize-1];
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

void GUIStart() {
    WelcomeGUI();
}

void GUILoop() {

}
