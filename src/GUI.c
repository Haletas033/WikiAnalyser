//
// Created by halet on 2/11/2026.
//


#include "../include/GUI.h"


#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __WIN32__
#include "../include/WINGUI.h"
#endif

PaintStacks paintStacks = {0};
DO_AFTER_ENTRY doAfters[5] = {0};

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

GUI_TEXT* greetingText;
const char* greetingDialogue[6] = {
    "Welcome to WikiAnalyser",
    "It seems it is your first time here",
    "Or your data is missing...",
    "Anyways, to begin you are going to have to download a wikipedia dump",
    "Well, I guess any .xml that has the same layout as the Wikipedia ones",
    "Below are the 4 different options you have. You can either download all of wikipedia, the top n articles, a custom set of article, or open any .xml file on your computer"
};

int dialoguePos = 0;
char subStr[1000];
int animDone = 1;
void animateText() {
    static int subStrPos = 1;
    strncpy(subStr, greetingDialogue[dialoguePos], subStrPos);
    subStr[subStrPos] = '\0';
    greetingText->text = subStr;
    subStrPos++;
    if (subStrPos == strlen(greetingDialogue[dialoguePos]) + 1) {
        OSKillTimer(IDT_DO_AFTER_2);
        subStrPos = 0;
        animDone = 1;
    }
}

void changeText() {
    if (animDone) {
        animDone = 0;
        greetingText->text = greetingDialogue[dialoguePos];
        OSDoAfterMillis(IDT_DO_AFTER_2, 100, animateText);
        if (animDone) dialoguePos++;

        if (dialoguePos == 6)
            OSKillTimer(IDT_DO_AFTER);
    }
}

void GUIStart() {
    greetingText = DrawPermanentText((GUI_TEXT){"Welcome to WikiAnalyser", 50, 50, 14});
    OSDoAfterMillis(IDT_DO_AFTER, 100, changeText);

}

void GUILoop() {

}
