//
// Created by halet on 3/25/2026.
//

#include "../include/graphGUI.h"

#include <time.h>

#include "../include/mainGUI.h"
#include "../include/WINGUI.h"

void DrawPieGraph(Article finalArticle, GraphData graphData, Window* wnd) {
    //testing values
    const int numberCount = graphData.intFieldsCount + graphData.floatFieldsCount;
    float values[numberCount];
    COLOUR colours[numberCount];
    srand(time(NULL));

    int lastEndDeg = 0;
    float sum = 0;
    float cumulativeSum = 0;

    int i;
    for (i = 0; i < graphData.intFieldsCount; i++)
        values[i] = finalArticle.intFields[graphData.intFieldIndices[i]];
    for (; i < numberCount; i++)
        values[i] = finalArticle.floatFields[graphData.floatFieldIndices[i - graphData.intFieldsCount]];

    //Draw MainWheel
    for (i = 0; i < numberCount; i++)
        sum += values[i];

    for (i = 0; i < numberCount; i++) {
        colours[i].r = rand() % 255;
        colours[i].g = rand() % 255;
        colours[i].b = rand() % 255;
    }

    for (i = 0; i < numberCount; i++) {

        cumulativeSum += values[i];
        int endDeg = cumulativeSum / sum * 360;
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
    for (i = 0; i < numberCount; i++) {
        DrawPermanentRect((COLOUR_RECT){5, i*11+10, 15, i*11+20, colours[i]}, key);

        const int idx = i < graphData.intFieldsCount ? i : i - graphData.intFieldsCount;
        char** fieldNames = i<graphData.intFieldsCount ? article.intFieldNames : article.floatFieldNames;
        const int* fieldIndices = i<graphData.intFieldsCount ? graphData.intFieldIndices : graphData.floatFieldIndices;

        if (graphData.stringFieldsCount == 0)
            DrawPermanentText((GUI_TEXT){fieldNames[fieldIndices[idx]], 24, i*11+14, 25}, key);
        else
            DrawPermanentText((GUI_TEXT){finalArticle.stringFields[graphData.stringFieldIndices[i]], 24, i*11+14, 25}, key);
    }
}


void DrawPercentageBarGraph(Article finalArticle, GraphData graphData, Window* wnd) {
    //testing values
    const int numberCount = graphData.intFieldsCount+graphData.floatFieldsCount;
    float values[numberCount];
    COLOUR colours[numberCount];
    srand(time(NULL));

    int lastEndPos = 0;
    float sum = 0;
    float cumulativeSum = 0;

    //Draw bars
    int i;
    for (i = 0; i < graphData.intFieldsCount; i++)
        values[i] = finalArticle.intFields[graphData.intFieldIndices[i]];
    for (; i < numberCount; i++)
        values[i] = finalArticle.floatFields[graphData.floatFieldIndices[i - graphData.intFieldsCount]];

    for (i = 0; i < numberCount; i++)
        sum += values[i];

    for (i = 0; i < numberCount; i++) {
        colours[i].r = rand() % 255;
        colours[i].g = rand() % 255;
        colours[i].b = rand() % 255;
    }

    for (i = 0; i < numberCount; i++) {

        cumulativeSum += values[i];
        int endPos = cumulativeSum / sum * 80.0f;
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
    for (i = 0; i < numberCount/5; i++) {
        for (j = 0; j < 5; j++) {
            DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);

            const int idx = i*5+j < graphData.intFieldsCount ? i*5+j : i*5+j - graphData.intFieldsCount;
            char** fieldNames = i*5+j<graphData.intFieldsCount ? article.intFieldNames : article.floatFieldNames;
            const int* fieldIndices = i*5+j<graphData.intFieldsCount ? graphData.intFieldIndices : graphData.floatFieldIndices;

            if (graphData.stringFieldsCount == 0)
                DrawPermanentText((GUI_TEXT){fieldNames[fieldIndices[idx]], 5+(5*i)*5+12, j*12+24, 15}, key);
            else
                DrawPermanentText((GUI_TEXT){finalArticle.stringFields[graphData.stringFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
        }
    }

    for (j = 0; j < numberCount%5; j++) {
        DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);

        const int idx = i*5+j < graphData.intFieldsCount ? i*5+j : i*5+j - graphData.intFieldsCount;
        char** fieldNames = i*5+j<graphData.intFieldsCount ? article.intFieldNames : article.floatFieldNames;
        const int* fieldIndices = i*5+j<graphData.intFieldsCount ? graphData.intFieldIndices : graphData.floatFieldIndices;

        if (graphData.stringFieldsCount == 0)
            DrawPermanentText((GUI_TEXT){fieldNames[fieldIndices[idx]], 5+(5*i)*5+12, j*12+24, 15}, key);
        else
            DrawPermanentText((GUI_TEXT){finalArticle.stringFields[graphData.stringFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
    }
}

void DrawBarGraph(Article finalArticle, GraphData graphData, Window* wnd) {
    const int numberCount = graphData.intFieldsCount + graphData.floatFieldsCount;
    float values[numberCount];
    float maxValue = MININT;

    int i;
    for (i = 0; i < graphData.intFieldsCount; i++)
        values[i] = finalArticle.intFields[graphData.intFieldIndices[i]];
    for (; i < numberCount; i++)
        values[i] = finalArticle.floatFields[graphData.floatFieldIndices[i - graphData.intFieldsCount]];

    COLOUR colours[numberCount];
    srand(time(NULL));

    for (i = 0; i < numberCount; i++) {
        maxValue = values[i] > maxValue ? values[i] : maxValue;
    }

    for (i = 0; i < numberCount; i++) {
        colours[i].r = rand() % 255;
        colours[i].g = rand() % 255;
        colours[i].b = rand() % 255;
    }

    int baseWidth = 80/numberCount;
    int gap = baseWidth / 10;
    for (i = 0; i < numberCount; i++)
        DrawPermanentRect((COLOUR_RECT){i*baseWidth+10, 50 -values[i] / maxValue * 40, i*baseWidth+baseWidth+10-gap, 60, colours[i]}, wnd);

    //Draw key window
    Window* key = malloc(sizeof(Window));
    *key = (Window){10, 65, 80, 30};
    OSCreateChildWindowOnChildWindow(2, "Key", key, wnd);
    DrawPermanentWindow(key, wnd);
    DrawPermanentText((GUI_TEXT){"Key:", 9, 10, 20}, key);

    //Draw key
    int j;
    for (i = 0; i < numberCount/5; i++) {
        for (j = 0; j < 5; j++) {
            DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);

            const int idx = i*5+j < graphData.intFieldsCount ? i*5+j : i*5+j - graphData.intFieldsCount;
            char** fieldNames = i*5+j<graphData.intFieldsCount ? article.intFieldNames : article.floatFieldNames;
            const int* fieldIndices = i*5+j<graphData.intFieldsCount ? graphData.intFieldIndices : graphData.floatFieldIndices;

            if (graphData.stringFieldsCount == 0)
                DrawPermanentText((GUI_TEXT){fieldNames[fieldIndices[idx]], 5+(5*i)*5+12, j*12+24, 15}, key);
            else
                DrawPermanentText((GUI_TEXT){finalArticle.stringFields[graphData.stringFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
        }
    }

    for (j = 0; j < numberCount%5; j++) {
        DrawPermanentRect((COLOUR_RECT){5+(5*i)*5, j*12+20, (5*i)*5+10, j*12+30, colours[i*5+j]}, key);

        const int idx = i*5+j < graphData.intFieldsCount ? i*5+j : i*5+j - graphData.intFieldsCount;
        char** fieldNames = i*5+j<graphData.intFieldsCount ? article.intFieldNames : article.floatFieldNames;
        const int* fieldIndices = i*5+j<graphData.intFieldsCount ? graphData.intFieldIndices : graphData.floatFieldIndices;

        if (graphData.stringFieldsCount == 0)
            DrawPermanentText((GUI_TEXT){fieldNames[fieldIndices[idx]], 5+(5*i)*5+12, j*12+24, 15}, key);
        else
            DrawPermanentText((GUI_TEXT){finalArticle.stringFields[graphData.stringFieldIndices[i*5+j]], 5+(5*i)*5+12, j*12+24, 15}, key);
    }
}

void DrawScatterGraph(Article finalArticle, GraphData graphData, Window* wnd) {
    const int numberCount = graphData.intFieldsCount + graphData.floatFieldsCount;
    float XValues[numberCount];

    int i;
    for (i = 0; i < graphData.intFieldsCount; i++)
        XValues[i] = finalArticle.intFields[graphData.intFieldIndices[i]];
    for (; i < numberCount; i++)
        XValues[i] = finalArticle.floatFields[graphData.floatFieldIndices[i - graphData.intFieldsCount]];


    float YValues[numberCount];
    for (i = 0; i < graphData.intFieldsCount; i++)
        YValues[i] = finalArticle.intFields[graphData.YIntFieldIndices[i]];
    for (; i < numberCount; i++)
        YValues[i] = finalArticle.floatFields[graphData.YFloatFieldIndices[i - graphData.YIntFieldsCount]];


    float maxXValue = INT_MIN;
    float maxYValue = INT_MIN;

    for (i = 0; i < numberCount; i++) {
        maxXValue = XValues[i] > maxXValue ? XValues[i] : maxXValue;
        maxYValue = YValues[i] > maxYValue ? YValues[i] : maxYValue;
    }

    for (i = 0; i < numberCount; i++)
        DrawPermanentPoint((COLOUR_POINT){20+XValues[i] / maxXValue*65, 80-YValues[i] / maxYValue*65, 5, 255, 122, 0}, wnd);

    //Draw bounding box
    GUI_POINT boundingBox[5] = {{10,10}, {90, 10}, {90, 90}, {10, 90}, {10, 10}};
    DrawPermanentLineChain((COLOUR_LINE_CHAIN){boundingBox, 5, 2}, wnd);
}