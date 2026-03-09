
#include "../include/core/unwanted.h"
#include "../include/core/cleanup.h"
#include "../include/core/article.h"
#include "../include/core/articleParser.h"

#ifdef __WIN32__
#include "../include/WINGUI.h"
#else
#include "../include/LINUXGUI.h"
#endif
#include "../include/GUI.h"
#include "../include/welcomeGUI.h"

int main() {
    UNWANTED unwanted = {0};
    ModifyUnwanted_CHAR(&unwanted, UNWANTED_ADD, "!@#$%^&*()l1234567890 \n");
    ModifyUnwanted_CHAR(&unwanted, UNWANTED_DELETE, "l");

    ModifyUnwanted_STRING(&unwanted, UNWANTED_ADD, (char*[]){"foo", "bar", "ab"}, 3);
    ModifyUnwanted_STRING(&unwanted, UNWANTED_DELETE, (char*[]){"ab"}, 1);

    ModifyUnwanted_CONTAINER(
        &unwanted, UNWANTED_ADD,
        (char*[]){"<siteinfo>", "{{", "aa"},(char*[]){"</siteinfo>", "}}", "aa"},
        3
    );

    ModifyUnwanted_CONTAINER(
        &unwanted, UNWANTED_DELETE,
        (char*[]){"aa"},(char*[]){"aa"},
        1
    );

    Article *articles = NULL;
    unsigned int articleCount = 0;

    Article article = {0};
    AddField(&article, FIELD_INT, "e_Count");
    AddField(&article, FIELD_FLOAT, "Float Test");
    AddField(&article, FIELD_BOOL, "Bool Test");
    AddField(&article, FIELD_STRING, "String Test");

    //Ignore (for testing on both linux and windows)
    #ifdef __WIN32__

    CleanUpData("C:/Users/halet/Dev/wikianalyserTest.xml", unwanted);

    ParseArticles("C:/Users/halet/Dev/wikianalyserTest.xml", &articles, &article, &articleCount);

    #endif

    #ifdef __linux__

    CleanUpData("/home/haletas/Dev/wikianalyserTest.xml", unwanted);

    ParseArticles("/home/haletas/Dev/wikianalyserTest.xml", &articles, &article, &articleCount);

    #endif

    int i;
    for (i = 0; i < articleCount; i++) {
        zigPrintTest(&articles[i]);
    }

    Window window = {(GUI_RECT){}, paintStacks};


    OSCreateWindowClass();
    OSCreateWindow(&window);
    Window childWnd = {0, 0, 20, 100};
    Window childWnd1 = {80, 0, 20, 100};
    Window childWnd2 = {20, 80, 60, 20};
    Window childWnd3 = {20, 0, 60, 80};

    OSCreateChildWindow(42, "Menu", &childWnd);
    DrawPermanentWindow(childWnd, &window);
    OSCreateChildWindow(42, "Menu", &childWnd1);
    DrawPermanentWindow(childWnd1, &window);
    OSCreateChildWindow(42, "Menu", &childWnd2);
    DrawPermanentWindow(childWnd2, &window);
    OSCreateChildWindow(42, "Menu", &childWnd3);
    DrawPermanentWindow(childWnd3, &window);

    DrawPermanentRect((COLOUR_RECT){0,0,100,100, 255, 255, 255}, &childWnd);
    DrawPermanentRect((COLOUR_RECT){0,0,100,100, 255, 255, 255}, &childWnd1);
    DrawPermanentRect((COLOUR_RECT){0,0,100,100, 255, 255, 255}, &childWnd2);
    DrawPermanentRect((COLOUR_RECT){0,0,100,100, 255, 255, 255}, &childWnd3);

    // GUIStart(&childWnd);
    // GUIStart(&childWnd1);
    // GUIStart(&childWnd2);

    OSMessageLoop();

    return 0;
}

