//
// Created by halet on 3/25/2026.
//

#ifndef GRAPHGUI_H
#define GRAPHGUI_H
#include "GUI.h"

void DrawTextValue(const char* fieldName, Window* wnd);
void DrawPieGraph(const char** fieldNames, Window* wnd);
void DrawBarGraph(const char** fieldNames, Window* wnd);
void DrawPercentageBarGraph(const char** fieldNames, Window* wnd);
void DrawScatterGraph(const char** XFieldNames, const char** YFieldNames, Window* wnd);

#endif //GRAPHGUI_H
