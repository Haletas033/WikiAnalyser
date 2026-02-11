//
// Created by halet on 2/11/2026.
//

#ifndef GUI_H
#define GUI_H

#include <stdlib.h>

typedef struct GUI_RECT { int x, y, w, h; } GUI_RECT;

typedef struct GUI_POINT { int x, y; } GUI_POINT;

typedef struct GUI_LINE { GUI_RECT x, y, thickness; } GUI_LINE;

typedef struct COLOUR{ int r, g, b; } COLOUR;

typedef struct COLOUR_RECT { GUI_RECT rect; COLOUR colour; } COLOUR_RECT;

typedef struct COLOUR_LINE { GUI_LINE line; COLOUR colour; } COLOUR_LINE;

typedef struct COLOUR_POINT { GUI_POINT point; COLOUR colour; } COLOUR_POINT;

typedef struct PaintStacks {
    COLOUR_RECT* colourRects;
    COLOUR_LINE* colourLines;
    COLOUR_POINT* colourPoints;

    unsigned int colourRectsSize;
    unsigned int colourLinesSize;
    unsigned int colourPointsSize;
} PaintStacks;

extern PaintStacks paintStacks;

void DrawRect(COLOUR_RECT colourRect);


#endif //GUI_H
