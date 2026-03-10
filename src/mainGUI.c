//
// Created by halet on 09/03/26
//

#include <stdlib.h>
#include "../include/mainGUI.h"

void MainGUIStart(Window* wnd) {
    Window* properties = malloc(sizeof(Window));
    Window* console = malloc(sizeof(Window));
    Window* visualiserScreen = malloc(sizeof(Window));

    *properties = (Window){80, 0, 20, 100};
    *console = (Window){0, 80, 80, 20};
    *visualiserScreen = (Window){0, 0, 80, 80};

    OSCreateChildWindow(1, "Properties", properties);
    DrawPermanentWindow(properties, wnd);
    OSCreateChildWindow(2, "Console", console);
    DrawPermanentWindow(console, wnd);
    OSCreateChildWindow(3, "VisualiserScreen", visualiserScreen);
    DrawPermanentWindow(visualiserScreen, wnd);

    DrawPermanentText((GUI_TEXT){"No properties to display.", 50, 50, 30}, properties);
    DrawPermanentText((GUI_TEXT){"Console Initialized:", 17, 17, 20}, console);
    DrawPermanentText((GUI_TEXT){"Nothing to display yet...", 50, 50, 20}, visualiserScreen);
}
