//
// Created by halet on 3/25/2026.
//

#ifndef GRAPHGUI_H
#define GRAPHGUI_H
#include "GUI.h"
#include "core/article.h"

typedef struct GraphData {
    const char** intFieldNames;
    const char** floatFieldNames;
    const char** boolFieldNames;
    const char** stringFieldNames;

    const char** YIntFieldNames;
    const char** YFloatFieldNames;

    unsigned int intFieldsCount;
    unsigned int floatFieldsCount;
    unsigned int boolFieldsCount;
    unsigned int stringFieldsCount;

    Article article;
} GraphData;

void DrawTextValue(GraphData graphData, Window* wnd);
void DrawPieGraph(GraphData graphData, Window* wnd);
void DrawBarGraph(GraphData graphData, Window* wnd);
void DrawPercentageBarGraph(GraphData graphData, Window* wnd);
void DrawScatterGraph(GraphData graphData, Window* wnd);

#endif //GRAPHGUI_H
