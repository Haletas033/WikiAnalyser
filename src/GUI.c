//
// Created by halet on 2/11/2026.
//

#include "../include/GUI.h"

PaintStacks paintStacks = {0};

void DrawRect(COLOUR_RECT colourRect) {
    paintStacks.colourRectsSize++;
    COLOUR_RECT* tmp = realloc(paintStacks.colourRects, paintStacks.colourRectsSize*sizeof(COLOUR_RECT));

    if (!tmp) {
        return;
    }

    paintStacks.colourRects = tmp;

    paintStacks.colourRects[paintStacks.colourRectsSize-1] = colourRect;
}