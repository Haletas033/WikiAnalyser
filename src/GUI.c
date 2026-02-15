//
// Created by halet on 2/11/2026.
//


#include "../include/GUI.h"


#include <string.h>

#ifdef __WIN32__
#include "../include/WINGUI.h"
#endif

PaintStacks paintStacks = {0};

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

GUI_BUTTON* DrawPermanentButton(GUI_BUTTON button){
    paintStacks.buttonsSize++;
    GUI_BUTTON* tmp = realloc(paintStacks.buttons, paintStacks.buttonsSize*sizeof(GUI_BUTTON));
    if (!tmp) { return NULL; }
    paintStacks.buttons = tmp;
    paintStacks.buttons[paintStacks.buttonsSize-1] = button;
    return &paintStacks.buttons[paintStacks.buttonsSize-1];
}

COLOUR_POINT* point;
void GUIStart() {
    DrawPermanentRect((COLOUR_RECT){80, 0, 100, 100, 255, 122, 0});

    GUI_POINT points[3] = {(GUI_POINT){0, 0}, (GUI_POINT){50, 70}, (GUI_POINT){100, 0}};
    DrawPermanentLineChain((COLOUR_LINE_CHAIN){points, 3, 3, 0,0, 0});

    point = DrawPermanentPoint((COLOUR_POINT){50, 50, 50, 255, 255, 0});

    DrawPermanentText((GUI_TEXT){"Hello, World!", 50, 50, 12});

    DrawPermanentImage((GUI_IMAGE){25, 25, 50, 50, OSCreateImage("test.bmp")});

    void* button = OSCreateButton();
    DrawPermanentButton((GUI_BUTTON){"OK", 0, 0, 10, 10, button});

    void* checkbox = OSCreateCheckBox();
    DrawPermanentButton((GUI_BUTTON){"OK", 0, 10, 10, 10, checkbox});
}

void GUILoop() {
}
