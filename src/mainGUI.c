//
// Created by halet on 09/03/26
//

#include <stdlib.h>
#include "../include/mainGUI.h"

PaintStacks cleanupPaintStacks = {0};
PaintStacks fieldsPaintStacks = {0};
PaintStacks parsePaintStacks = {0};

void SetupCleanupPaintStacks(PaintStacks* ps) {

}

void StartPropertiesModeSelector(Window* wnd) {
    DrawPermanentButton((GUI_BUTTON_LIKE){"Cleanup",
        GetButtonPosWH(3, (GUI_POINT){50, 0}, 33, 100, 1),
        OSCreateButton(6, NULL, wnd)}, wnd);

    DrawPermanentButton((GUI_BUTTON_LIKE){"Fields Setup",
        GetButtonPosWH(3, (GUI_POINT){50, 0}, 33, 100, 2),
        OSCreateButton(6, NULL, wnd)}, wnd);

    DrawPermanentButton((GUI_BUTTON_LIKE){"Parse/Analyse",
        GetButtonPosWH(3, (GUI_POINT){50, 0}, 33, 100, 3),
        OSCreateButton(6, NULL, wnd)}, wnd);
}

void MainGUIStart(Window* wnd) {
    Window* propertiesModeSelector = malloc(sizeof(Window));
    Window* properties = malloc(sizeof(Window));
    Window* console = malloc(sizeof(Window));
    Window* visualiserScreen = malloc(sizeof(Window));

    *propertiesModeSelector = (Window){80, 0, 20, 10};
    *properties = (Window){80, 10, 20, 90};
    *console = (Window){0, 80, 80, 20};
    *visualiserScreen = (Window){0, 0, 80, 80};

    OSCreateChildWindow(2, "PoropertiesModeSelector", propertiesModeSelector);
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

    StartPropertiesModeSelector(propertiesModeSelector);
}
