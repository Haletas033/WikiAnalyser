//
// Created by halet on 2/11/2026.
//

#ifndef GUI_H
#define GUI_H

#include <stdlib.h>

typedef struct GUI_RECT { int x, y, w, h; } GUI_RECT;

typedef struct GUI_POINT { int x, y; } GUI_POINT;

typedef struct GUI_LINE { GUI_POINT p1, p2; int thickness; } GUI_LINE;

typedef struct GUI_LINE_CHAIN { GUI_POINT* points; unsigned int pointsSize; int thickness; } GUI_LINE_CHAIN;

typedef struct GUI_TEXT {const char* text; GUI_POINT pos; int size;} GUI_TEXT;

typedef struct GUI_IMAGE {GUI_RECT rect; void* imgLoc;} GUI_IMAGE;

typedef struct GUI_BUTTON_LIKE {const char* text; GUI_RECT rect; void* buttonLoc;} GUI_BUTTON;

typedef struct COLOUR{ int r, g, b; } COLOUR;

typedef struct COLOUR_RECT { GUI_RECT rect; COLOUR colour; } COLOUR_RECT;

typedef struct COLOUR_LINE { GUI_LINE line; COLOUR colour; } COLOUR_LINE;

typedef struct COLOUR_LINE_CHAIN { GUI_LINE_CHAIN linePath; COLOUR colour; } COLOUR_LINE_CHAIN;

typedef struct COLOUR_POINT { GUI_POINT point; int radius; COLOUR colour; } COLOUR_POINT;



typedef struct PaintStacks {
    COLOUR_RECT* colourRects;
    COLOUR_LINE* colourLines;
    COLOUR_LINE_CHAIN* colourLineChains;
    COLOUR_POINT* colourPoints;
    GUI_TEXT* texts;
    GUI_IMAGE* images;
    GUI_BUTTON* buttons;

    unsigned int colourRectsSize;
    unsigned int colourLinesSize;
    unsigned int colourLinesChainsSize;
    unsigned int colourPointsSize;
    unsigned int textsSize;
    unsigned int imagesSize;
    unsigned int buttonsSize;
} PaintStacks;

extern PaintStacks paintStacks;

COLOUR_RECT* DrawPermanentRect(COLOUR_RECT colourRect);
COLOUR_LINE* DrawPermanentLine(COLOUR_LINE colourLine);
COLOUR_LINE_CHAIN* DrawPermanentLineChain(COLOUR_LINE_CHAIN colourLineChain);
COLOUR_POINT* DrawPermanentPoint(COLOUR_POINT colourPoint);
GUI_TEXT* DrawPermanentText(GUI_TEXT text);
GUI_IMAGE* DrawPermanentImage(GUI_IMAGE image);
GUI_BUTTON* DrawPermanentButton(GUI_BUTTON button);

void GUIStart();

void GUILoop();


#endif //GUI_H
