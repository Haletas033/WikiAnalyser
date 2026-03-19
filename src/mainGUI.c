//
// Created by halet on 09/03/26
//

#include <stdlib.h>
#include "../include/mainGUI.h"

#include <stdio.h>

PaintStacks cleanupPaintStacks = {0};
PaintStacks fieldsPaintStacks = {0};
PaintStacks parsePaintStacks = {0};

Window* properties;

void SetupCleanupPaintStacks(PaintStacks* ps, Window* wnd) {
    DrawPermanentTextToPaintStacks((GUI_TEXT){"Unwanted Characters:", 50, 5, 35}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Input unwanted characters here...",
        (GUI_RECT){10,7, 80, 5},  OSCreateInputBox(10, wnd)}, ps);

    DrawPermanentTextToPaintStacks((GUI_TEXT){"Unwanted Strings:", 50, 15, 35}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Input unwanted strings here...",
        (GUI_RECT){10,17, 80, 5},  OSCreateInputBox(11, wnd)}, ps);

    DrawPermanentTextToPaintStacks((GUI_TEXT){"Unwanted Container heads:", 50, 25, 35}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Input unwanted container heads here...",
        (GUI_RECT){10,27, 80, 5},  OSCreateInputBox(12, wnd)}, ps);

    DrawPermanentTextToPaintStacks((GUI_TEXT){"Unwanted Container tails:", 50, 35, 35}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Input unwanted container tails here...",
        (GUI_RECT){10,37, 80, 5},  OSCreateInputBox(13, wnd)}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Apply",
        (GUI_RECT){10,45, 80, 10},  OSCreateButton(14, NULL, wnd)}, ps);
}

void SwitchWindowPaintStacksToCleanup(Window* _) {
    properties->paintStacks = cleanupPaintStacks;
}

void SwitchWindowPaintStacksToFields(Window* _) {
    properties->paintStacks = (PaintStacks){0};
}

void SwitchWindowPaintStacksToParse(Window* _) {
    properties->paintStacks = (PaintStacks){0};
}

void StartPropertiesModeSelector(Window* wnd) {
    DrawPermanentButton((GUI_BUTTON_LIKE){"Cleanup",
        GetButtonPosWH(3, (GUI_POINT){50, 0}, 33, 100, 1),
        OSCreateButton(6, SwitchWindowPaintStacksToCleanup, wnd)}, wnd);

    DrawPermanentButton((GUI_BUTTON_LIKE){"Fields Setup",
        GetButtonPosWH(3, (GUI_POINT){50, 0}, 33, 100, 2),
        OSCreateButton(7, SwitchWindowPaintStacksToFields, wnd)}, wnd);

    DrawPermanentButton((GUI_BUTTON_LIKE){"Parse/Analyse",
        GetButtonPosWH(3, (GUI_POINT){50, 0}, 33, 100, 3),
        OSCreateButton(8, SwitchWindowPaintStacksToParse, wnd)}, wnd);
}

void MainGUIStart(Window* wnd) {
    Window* propertiesModeSelector = malloc(sizeof(Window));
    properties = malloc(sizeof(Window));
    Window* console = malloc(sizeof(Window));
    Window* visualiserScreen = malloc(sizeof(Window));

    *propertiesModeSelector = (Window){80, 0, 20, 10};
    *properties = (Window){80, 10, 20, 90};
    *console = (Window){0, 80, 80, 20};
    *visualiserScreen = (Window){0, 0, 80, 80};

    OSCreateChildWindow(2, "PropertiesModeSelector", propertiesModeSelector);
    DrawPermanentWindow(propertiesModeSelector, wnd);
    OSCreateChildWindow(3, "Properties", properties);
    DrawPermanentWindow(properties, wnd);
    OSCreateChildWindow(4, "Console", console);
    DrawPermanentWindow(console, wnd);
    OSCreateChildWindow(5, "VisualiserScreen", visualiserScreen);
    DrawPermanentWindow(visualiserScreen, wnd);

    DrawPermanentText((GUI_TEXT){"No properties to display.", 50, 50, 30}, properties);
    DrawPermanentText((GUI_TEXT){"Console Initialized:", 17, 17, 20}, console);
    DrawPermanentText((GUI_TEXT){"Nothing to display yet...", 50, 50, 20}, visualiserScreen);

    SetupCleanupPaintStacks(&cleanupPaintStacks, properties);
    properties->paintStacks = cleanupPaintStacks;
    StartPropertiesModeSelector(propertiesModeSelector);
}
