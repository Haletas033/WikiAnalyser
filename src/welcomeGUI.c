//
// Created by halet on 2/18/2026.
//

#include "../include/welcomeGUI.h"

#include <stdio.h>
#include <string.h>

#include "../include/curl.h"
#include "../include/topN.h"


void(*callback)(Window* wnd);

GUI_TEXT* greetingText;
const char* greetingDialogue[18] = {
    "Welcome to WikiAnalyser",
    "It seems it is your first time here",
    "Or your data is missing...",
    "Anyways, to begin you are going to have to download a wikipedia dump",
    "Well, I guess any .xml that has the same layout as the Wikipedia ones",
    "For this you have a few options. Here they are",
    "1. Download all of Wikipedia ~100gb",
    "2. Download the top n articles",
    "3. Download a custom set of articles",
    "4. Open a pre-existing .xml file on your computer",
    /*After choice */
    "Great choice. Just running test to make sure everything is correct",
    /*After test*/
    "Test failed. Could not find Zig on your computer",
    "You have 2 options",
    "Download Zig",
    "or",
    "Open Zig",
    /*After tests pass*/
    "Great! You are all set",
    "If you want. You can find tutorials here. Have fun (:"
};

int dialoguePos = 0;
char subStr[1000];
int animDone = 1;

enum buttons {
    WIKI_FULL,
    WIKI_TOP_N,
    WIKI_CUSTOM,
    WIKI_OPEN,
    ZIG_DOWNLOAD,
    NULL_BUTTON, /*Skips the 'Or' in the failure dialog*/
    ZIG_OPEN
};

void performCheckText(Window* wnd);
void failureOptions(Window* wnd);
void success(Window* wnd);

void deleteButtons(Window* wnd);
void destroyWelcomeGUI(Window* wnd) {
    deleteButtons(wnd);
    ClearGUI(&wnd->paintStacks, doAfters, buttonCommands);
}

void downloadFullWikipediaDump(Window* wnd) {
    const char* path = OSGetDirectoryPath();
    if (path == NULL) return;
    CurlDownloadTo("https://dumps.wikimedia.org/enwiki/latest/enwiki-latest-pages-articles-multistream.xml.bz2", path, "/wikiDump.bz2");
    if (path[0] != '\0') {
        SetINIField("UserData/data.ini", "DumpPath", path);
        performCheckText(wnd);
    }
}

void downloadTopNWikipediaDump(Window* wnd) {
    const char* path = OSGetDirectoryPath();
    if (path == NULL) return;
    if (path[0] != '\0') {
        ArticleViews* hashmap = LoadTopNFile("SystemData/topN.topn");
        CurlDownloadWithSpecialExportTo(GetTop(10000, hashmap), 10000, path, "/top100.xml");
        SetINIField("UserData/data.ini", "DumpPath", path);
        performCheckText(wnd);
    }
}

void downloadCustomWikipediaDump(Window* wnd) {
    performCheckText(wnd);
}

void openWikipediaDump(Window* wnd) {
    const char* path = OSGetFilePath();
    if (path[0] != '\0') {
        SetINIField("UserData/data.ini", "DumpPath", path);
        performCheckText(wnd);
    }
}

void downloadZig(Window* wnd) {
    success(wnd);
}

void openZig(Window* wnd) {
    success(wnd);
}

void openMainGUI(Window* wnd) {
    destroyWelcomeGUI(wnd);
    callback(wnd);
}

void createButtons(Window* wnd) {
    switch (dialoguePos) {
        case WIKI_OPTION(WIKI_FULL): DrawPermanentButton((GUI_BUTTON_LIKE){"Full", GetButtonPos(4,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 1), OSCreateButton(1, downloadFullWikipediaDump, wnd)}, wnd);
            break;
        case WIKI_OPTION(WIKI_TOP_N): DrawPermanentButton((GUI_BUTTON_LIKE){"Top n", GetButtonPos(4,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 2), OSCreateButton(2, downloadTopNWikipediaDump, wnd)}, wnd);
            break;
        case WIKI_OPTION(WIKI_CUSTOM): DrawPermanentButton((GUI_BUTTON_LIKE){"Custom", GetButtonPos(4,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 3), OSCreateButton(3, downloadCustomWikipediaDump, wnd)}, wnd);
            break;
        case WIKI_OPTION(WIKI_OPEN): DrawPermanentButton((GUI_BUTTON_LIKE){"Open", GetButtonPos(4,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 4), OSCreateButton(4, openWikipediaDump, wnd)}, wnd);
            break;
        case ZIG_OPTION(ZIG_DOWNLOAD): DrawPermanentButton((GUI_BUTTON_LIKE){"Download", GetButtonPos(2,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 1), OSCreateButton(1, downloadZig, wnd)}, wnd);
            break;
        case ZIG_OPTION(ZIG_OPEN): DrawPermanentButton((GUI_BUTTON_LIKE){"Open", GetButtonPos(2,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 2), OSCreateButton(2, openZig, wnd)}, wnd);
            break;
        case FINISH_SETUP: DrawPermanentButton((GUI_BUTTON_LIKE){"OK", GetButtonPos(1,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 1), OSCreateButton(1, openMainGUI, wnd)}, wnd);
            break;
        default: break;
    }
}

void changeGreetingText(Window* wnd);

void(*changeTextFunc)(Window* wnd) = changeGreetingText;

void animateText(Window* wnd) {
    static int subStrPos = 1;
    memcpy(subStr, greetingDialogue[dialoguePos], subStrPos);
    subStr[subStrPos] = '\0';
    greetingText->text = subStr;
    subStrPos++;
    if (subStrPos == strlen(greetingDialogue[dialoguePos]) + 1) {
        subStrPos = 1;
        animDone = 1;
        dialoguePos++;

        OSDoAfterMillis(wnd, IDT_DO_AFTER, WAIT_AFTER_TEXT, changeTextFunc);
        OSKillTimer(wnd, IDT_DO_AFTER_2);
    }
}

void changeGreetingText(Window* wnd) {
    if (animDone) {
        animDone = 0;
        createButtons(wnd);
        if (dialoguePos < WELCOME_TEXT_END)
            OSDoAfterMillis(wnd, IDT_DO_AFTER_2, TYPING_SPEED, animateText);

        OSKillTimer(wnd, IDT_DO_AFTER);

    }
}

void performCheck(Window* wnd) {
    success(wnd);
}

void changePerformCheckText(Window* wnd) {
    if (animDone) {
        animDone = 0;
        if (dialoguePos < PERFORM_CHECK_TEXT_END)
            OSDoAfterMillis(wnd, IDT_DO_AFTER_2, TYPING_SPEED, animateText);
        else performCheck(wnd);
        OSKillTimer(wnd, IDT_DO_AFTER);
    }
}

void changeFailureText(Window* wnd) {
    if (animDone) {
        animDone = 0;
        createButtons(wnd);
        if (dialoguePos < ZIG_OPTIONS_TEXT_END)
            OSDoAfterMillis(wnd, IDT_DO_AFTER_2, TYPING_SPEED, animateText);
        OSKillTimer(wnd, IDT_DO_AFTER);
    }
}

void changeSuccessText(Window* wnd) {
    if (animDone) {
        animDone = 0;
        createButtons(wnd);
        if (dialoguePos < SUCCESS_TEXT_END)
            OSDoAfterMillis(wnd, IDT_DO_AFTER_2, TYPING_SPEED, animateText);
        OSKillTimer(wnd, IDT_DO_AFTER);
    }
}

void deleteButtons(Window* wnd) {
    OSDestroyButtonById(wnd, 1);
    OSDestroyButtonById(wnd, 2);
    OSDestroyButtonById(wnd, 3);
    OSDestroyButtonById(wnd, 4);
}

void failureOptions(Window* wnd) {
    animDone = 1;
    dialoguePos = ZIG_OPTIONS_TEXT_START;
    changeTextFunc = changeFailureText;
    changeFailureText(wnd);
}

void success(Window* wnd) {
    animDone = 1;
    deleteButtons(wnd);
    dialoguePos = SUCCESS_TEXT_START;
    changeTextFunc = changeSuccessText;
    changeSuccessText(wnd);
}

void performCheckText(Window* wnd) {
    animDone = 1;
    deleteButtons(wnd);
    changeTextFunc = changePerformCheckText;
    changePerformCheckText(wnd);
}

void WelcomeGUI(Window* wnd, void(*funcCallback)(Window* wnd)) {
    callback = funcCallback;
    greetingText = DrawPermanentText((GUI_TEXT){"", 50, 50, 14}, wnd);
    changeGreetingText(wnd);
}