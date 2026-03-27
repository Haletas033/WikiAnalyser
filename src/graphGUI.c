//
// Created by halet on 3/25/2026.
//

#include "../include/graphGUI.h"

#include <time.h>

#include "../include/mainGUI.h"
#include "../include/WINGUI.h"

void DrawPieGraph(GraphData graphData, Window* wnd) {
    //testing values
    int values[graphData.intFieldsCount];
    COLOUR colours[graphData.intFieldsCount];
    srand(time(NULL));

    int lastEndDeg = 0;
    int sum = 0;
    int cumulativeSum = 0;

    int i;
    for (i = 0; i < graphData.intFieldsCount; i++)
        values[i] = articles[0].intFields[graphData.intFieldIndices[i]];

    //Draw MainWheel
    for (i = 0; i < graphData.intFieldsCount; i++)
        sum += values[i];

    for (i = 0; i < graphData.intFieldsCount; i++) {
        colours[i].r = rand() % 255;
        colours[i].g = rand() % 255;
        colours[i].b = rand() % 255;
    }

    for (i = 0; i < graphData.intFieldsCount; i++) {

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
    for (i = 0; i < graphData.intFieldsCount; i++) {
        DrawPermanentRect((COLOUR_RECT){5, i*11+10, 15, i*11+20, colours[i]}, key);
        if (graphData.stringFieldsCount == 0)
            DrawPermanentText((GUI_TEXT){article.intFieldNames[graphData.intFieldIndices[i]], 24, i*11+14, 25}, key);
        else
            DrawPermanentText((GUI_TEXT){articles[0].stringFields[graphData.stringFieldIndices[i]], 24, i*11+14, 25}, key);
    }
}


void DrawPercentageBarGraph(GraphData graphData, Window* wnd) {
    //testing values
    int values[graphData.intFieldsCount];
    COLOUR colours[graphData.intFieldsCount];
    srand(time(NULL));

    int lastEndPos = 0;
    int sum = 0;
    int cumulativeSum = 0;

    //Draw bars
    int i;
    for (i = 0; i < graphData.intFieldsCount; i++)
        values[i] = articles[0].intFields[graphData.intFieldIndices[i]];

    for (i = 0; i < graphData.intFieldsCount; i++)
        sum += values[i];

    for (i = 0; i < graphData.intFieldsCount; i++) {
        colours[i].r = rand() % 255;
        colours[i].g = rand() % 255;
        colours[i].b = rand() % 255;
    }

    for (i = 0; i < graphData.intFieldsCount; i++) {

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
    for (i = 0; i < graphData.intFieldsCount/5; i++) {
        for (j = 0; j < 5; j++) {
            DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);
            if (graphData.stringFieldsCount == 0)
                DrawPermanentText((GUI_TEXT){article.intFieldNames[graphData.intFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
            else
                DrawPermanentText((GUI_TEXT){articles[0].stringFields[graphData.stringFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
        }
    }

    for (j = 0; j < graphData.intFieldsCount%5; j++) {
        DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);
        if (graphData.stringFieldsCount == 0)
            DrawPermanentText((GUI_TEXT){article.intFieldNames[graphData.intFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
        else
            DrawPermanentText((GUI_TEXT){articles[0].stringFields[graphData.stringFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
    }
}

void DrawBarGraph(GraphData graphData, Window* wnd) {
    int values[graphData.intFieldsCount];
    int maxValue = MININT;

    int i;
    for (i = 0; i < graphData.intFieldsCount; i++)
        values[i] = articles[0].intFields[graphData.intFieldIndices[i]];

    COLOUR colours[graphData.intFieldsCount];
    srand(time(NULL));

    for (i = 0; i < graphData.intFieldsCount; i++) {
        maxValue = values[i] > maxValue ? values[i] : maxValue;
    }

    for (i = 0; i < graphData.intFieldsCount; i++) {
        colours[i].r = rand() % 255;
        colours[i].g = rand() % 255;
        colours[i].b = rand() % 255;
    }

    int baseWidth = 80/graphData.intFieldsCount;
    int gap = baseWidth / 10;
    for (i = 0; i < graphData.intFieldsCount; i++)
        DrawPermanentRect((COLOUR_RECT){i*baseWidth+10, 50 -((float)values[i] / maxValue) * 40, i*baseWidth+baseWidth+10-gap, 60, colours[i]}, wnd);

    //Draw key window
    Window* key = malloc(sizeof(Window));
    *key = (Window){10, 65, 80, 30};
    OSCreateChildWindowOnChildWindow(2, "Key", key, wnd);
    DrawPermanentWindow(key, wnd);
    DrawPermanentText((GUI_TEXT){"Key:", 9, 10, 20}, key);

    //Draw key
    int j;
    for (i = 0; i < graphData.intFieldsCount/5; i++) {
        for (j = 0; j < 5; j++) {
            DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);
            if (graphData.stringFieldsCount == 0)
                DrawPermanentText((GUI_TEXT){article.intFieldNames[graphData.intFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
            else
                DrawPermanentText((GUI_TEXT){articles[0].stringFields[graphData.stringFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
        }
    }

    for (j = 0; j < graphData.intFieldsCount%5; j++) {
        DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);
        if (graphData.stringFieldsCount == 0)
            DrawPermanentText((GUI_TEXT){article.intFieldNames[graphData.intFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
        else
            DrawPermanentText((GUI_TEXT){articles[0].stringFields[graphData.stringFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
    }
}

void DrawScatterGraph(GraphData graphData, Window* wnd) {
    int XValues[graphData.intFieldsCount];

    int i;
    for (i = 0; i < graphData.intFieldsCount; i++)
        XValues[i] = articles[0].intFields[graphData.intFieldIndices[i]];


    int YValues[graphData.intFieldsCount];
    for (i = 0; i < graphData.intFieldsCount; i++)
        YValues[i] = articles[0].intFields[graphData.YIntFieldIndices[i]];


    int maxXValue = INT_MIN;
    int maxYValue = INT_MIN;

    for (i = 0; i < graphData.intFieldsCount; i++) {
        maxXValue = XValues[i] > maxXValue ? XValues[i] : maxXValue;
        maxYValue = YValues[i] > maxYValue ? YValues[i] : maxYValue;
    }

    for (i = 0; i < graphData.intFieldsCount; i++)
        DrawPermanentPoint((COLOUR_POINT){20+((float)XValues[i] / maxXValue)*65, 80-((float)YValues[i] / maxYValue)*65, 5, 255, 122, 0}, wnd);

    //Draw bounding box
    GUI_POINT boundingBox[5] = {{10,10}, {90, 10}, {90, 90}, {10, 90}, {10, 10}};
    DrawPermanentLineChain((COLOUR_LINE_CHAIN){boundingBox, 5, 2}, wnd);
}