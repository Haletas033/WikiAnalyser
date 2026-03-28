//
// Created by halet on 3/25/2026.
//

#ifndef GRAPHGUI_H
#define GRAPHGUI_H
#include "GUI.h"
#include "core/article.h"

typedef struct GraphData {
    int* intFieldIndices;
    int* floatFieldIndices;
    int* boolFieldIndices;
    int* stringFieldIndices;

    int* YIntFieldIndices;
    int* YFloatFieldIndices;
    unsigned int YIntFieldsCount;
    unsigned int YFloatFieldsCount;

    unsigned int intFieldsCount;
    unsigned int floatFieldsCount;
    unsigned int boolFieldsCount;
    unsigned int stringFieldsCount;

    Article article;
} GraphData;

void DrawTextValue(GraphData graphData, Window* wnd);
void DrawPieGraph(Article finalArticle, GraphData graphData, Window* wnd);
void DrawBarGraph(Article finalArticle, GraphData graphData, Window* wnd);
void DrawPercentageBarGraph(Article finalArticle, GraphData graphData, Window* wnd);
void DrawScatterGraph(Article finalArticle, GraphData graphData, Window* wnd);

#endif //GRAPHGUI_H
