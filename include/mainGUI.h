//
// Created by halet on 09/03/26/
//

#ifndef MAINGUI_H
#define MAINGUI_H
#include "GUI.h"
#ifdef __WIN32__
#include "../include/WINGUI.h"
#endif

typedef struct Field {
    int fieldsSize;
    void* ifieldNameInputs[128];
    void* fieldXButtons[128];
    int fieldNameInputsGUI[128];
    int fieldXButtonsGUI[128];
    int freeSlots[128]; int freeSlotsSize;
} Field;

extern PaintStacks cleanupPaintStacks;
extern PaintStacks fieldsPaintStacks;
extern PaintStacks parsePaintStacks;

void MainGUIStart(Window* wnd);

#endif //MAINGUI_H
