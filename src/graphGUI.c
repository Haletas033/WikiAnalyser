//
// Created by halet on 3/25/2026.
//

#include "../include/graphGUI.h"

#include <time.h>

#include "../include/WINGUI.h"

void DrawPieGraph(const char** fieldNames, Window* wnd) {
    //testing values
    int values[] = {100, 56, 23, 124, 45};
    COLOUR colours[5];
    srand(time(NULL));

    int lastEndDeg = 0;
    int sum = 0;
    int cumulativeSum = 0;

    //Draw MainWheel
    int i;

    for (i = 0; i < 5; i++)
        sum += values[i];

    for (i = 0; i < 5; i++) {
        colours[i].r = rand() % 255;
        colours[i].g = rand() % 255;
        colours[i].b = rand() % 255;
    }

    for (i = 0; i < 5; i++) {

        cumulativeSum += values[i];
        int endDeg = ((float)cumulativeSum / sum) * 360;
        DrawPermanentPieSlice((COLOUR_PIE_SLICE){50, 50, 50, lastEndDeg, endDeg, colours[i]}, wnd);
        lastEndDeg = endDeg;
    }

    //Draw key window
    Window* key = malloc(sizeof(Window));
    *key = (Window){70, 50, 25, 45};
    OSCreateChildWindowOnChildWindow(2, "Key", key, wnd);
    DrawPermanentWindow(key, wnd);
    DrawPermanentText((GUI_TEXT){"Key:", 12, 5, 35}, key);

    //Draw key
    for (i = 0; i < 5; i++) {
        DrawPermanentRect((COLOUR_RECT){5, i*11+10, 15, i*11+20, colours[i]}, key);
    }
}


void DrawPercentageBarGraph(const char** fieldNames, Window* wnd) {
    //testing values
    int values[] = {100, 56, 23, 124, 45};
    COLOUR colours[5];
    srand(time(NULL));

    int lastEndPos = 0;
    int sum = 0;
    int cumulativeSum = 0;

    //Draw MainWheel
    int i;

    for (i = 0; i < 5; i++)
        sum += values[i];

    for (i = 0; i < 5; i++) {
        colours[i].r = rand() % 255;
        colours[i].g = rand() % 255;
        colours[i].b = rand() % 255;
    }

    for (i = 0; i < 5; i++) {

        cumulativeSum += values[i];
        int endPos = ((float)cumulativeSum / sum) * 80;
        DrawPermanentRect((COLOUR_RECT){10+lastEndPos, 40, 10+endPos, 60, colours[i]}, wnd);
        lastEndPos = endPos;
    }

    //Draw key window
    Window* key = malloc(sizeof(Window));
    *key = (Window){10, 65, 80, 30};
    OSCreateChildWindowOnChildWindow(2, "Key", key, wnd);
    DrawPermanentWindow(key, wnd);
    DrawPermanentText((GUI_TEXT){"Key:", 9, 10, 20}, key);

    //Draw key
    int j;
    for (i = 0; i < 5/5; i++) {
        for (j = 0; j < 5; j++) {
            DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);
        }
    }

    for (j = 0; j < 5%5; j++) {
        DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);
    }
}

void DrawBarGraph(const char** fieldNames, Window* wnd) {
    int values[] = {100, 56, 23, 124, 45};
    int maxValue = MININT;
    COLOUR colours[5];
    srand(time(NULL));

    int i;
    for (i = 0; i < 5; i++) {
        maxValue = values[i] > maxValue ? values[i] : maxValue;
    }

    for (i = 0; i < 5; i++) {
        colours[i].r = rand() % 255;
        colours[i].g = rand() % 255;
        colours[i].b = rand() % 255;
    }

    int baseWidth = 80/5;
    int gap = baseWidth / 10;
    for (i = 0; i < 5; i++)
        DrawPermanentRect((COLOUR_RECT){i*baseWidth+10, 50 -((float)values[i] / maxValue) * 40, i*baseWidth+baseWidth+10-gap, 60, colours[i]}, wnd);

    //Draw key window
    Window* key = malloc(sizeof(Window));
    *key = (Window){10, 65, 80, 30};
    OSCreateChildWindowOnChildWindow(2, "Key", key, wnd);
    DrawPermanentWindow(key, wnd);
    DrawPermanentText((GUI_TEXT){"Key:", 9, 10, 20}, key);

    //Draw key
    int j;
    for (i = 0; i < 5/5; i++) {
        for (j = 0; j < 5; j++) {
            DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);
        }
    }

    for (j = 0; j < 5%5; j++) {
        DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);
    }
}

void DrawScatterGraph(const char** XFieldNames, const char** YFieldNames, Window* wnd) {
    int XValues[] = {100, 0, 23, 124, 45};
    int YValues[] = {100, 0, 23, 124, 45};

    int maxXValue = INT_MIN;
    int maxYValue = INT_MIN;

    int i;
    for (i = 0; i < 5; i++) {
        maxXValue = XValues[i] > maxXValue ? XValues[i] : maxXValue;
        maxYValue = YValues[i] > maxYValue ? YValues[i] : maxYValue;
    }

    for (i = 0; i < 5; i++)
        DrawPermanentPoint((COLOUR_POINT){20+((float)XValues[i] / maxXValue)*65, 80-((float)YValues[i] / maxYValue)*65, 5, 255, 122, 0}, wnd);

    //Draw bounding box
    GUI_POINT boundingBox[5] = {{10,10}, {90, 10}, {90, 90}, {10, 90}, {10, 10}};
    DrawPermanentLineChain((COLOUR_LINE_CHAIN){boundingBox, 5, 2}, wnd);
}