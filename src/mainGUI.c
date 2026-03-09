//
// Created by halet on 09/03/26
//

#include <stdlib.h>
#include "../include/mainGUI.h"

void MainGUIStart(Window* wnd) {
    Window* childWnd = malloc(sizeof(Window));
    Window* childWnd1 = malloc(sizeof(Window));
    Window* childWnd2 = malloc(sizeof(Window));
    Window* childWnd3 = malloc(sizeof(Window));
    *childWnd = (Window){0, 0, 20, 100};
    *childWnd1 = (Window){80, 0, 20, 100};
    *childWnd2 = (Window){20, 80, 60, 20};
    *childWnd3 = (Window){20, 0, 60, 80};

    OSCreateChildWindow(42, "Menu", childWnd);
    DrawPermanentWindow(childWnd, wnd);
    OSCreateChildWindow(43, "Menu", childWnd1);
    DrawPermanentWindow(childWnd1, wnd);
    OSCreateChildWindow(44, "Menu", childWnd2);
    DrawPermanentWindow(childWnd2, wnd);
    OSCreateChildWindow(45, "Menu", childWnd3);
    DrawPermanentWindow(childWnd3, wnd);
}
