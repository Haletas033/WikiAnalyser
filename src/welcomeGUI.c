//
// Created by halet on 2/18/2026.
//

#include "../include/welcomeGUI.h"

#include <stdio.h>
#include <string.h>

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

void performCheckText();
void failureOptions();
void success();

void deleteButtons();
void destroyWelcomeGUI() {
    deleteButtons();
    ClearGUI(&paintStacks, doAfters, buttonCommands);
}

void downloadFullWikipediaDump() {
    performCheckText();
}

void downloadTopNWikipediaDump() {
    performCheckText();
}

void downloadCustomWikipediaDump() {
    performCheckText();
}

void openWikipediaDump() {
    performCheckText();
}

void downloadZig() {
    success();
}

void openZig() {
    success();
}

void openMainGUI() {
    printf("Opening main GUI...\n");
    destroyWelcomeGUI();
}

void createButtons() {
    switch (dialoguePos) {
        case WIKI_OPTION(WIKI_FULL): DrawPermanentButton((GUI_BUTTON_LIKE){"Full", GetButtonPos(4,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 1), OSCreateButton(1, downloadFullWikipediaDump)});
            break;
        case WIKI_OPTION(WIKI_TOP_N): DrawPermanentButton((GUI_BUTTON_LIKE){"Top n", GetButtonPos(4,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 2), OSCreateButton(2, downloadTopNWikipediaDump)});
            break;
        case WIKI_OPTION(WIKI_CUSTOM): DrawPermanentButton((GUI_BUTTON_LIKE){"Custom", GetButtonPos(4,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 3), OSCreateButton(3, downloadCustomWikipediaDump)});
            break;
        case WIKI_OPTION(WIKI_OPEN): DrawPermanentButton((GUI_BUTTON_LIKE){"Open", GetButtonPos(4,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 4), OSCreateButton(4, openWikipediaDump)});
            break;
        case ZIG_OPTION(ZIG_DOWNLOAD): DrawPermanentButton((GUI_BUTTON_LIKE){"Download", GetButtonPos(2,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 1), OSCreateButton(1, downloadZig)});
            break;
        case ZIG_OPTION(ZIG_OPEN): DrawPermanentButton((GUI_BUTTON_LIKE){"Open", GetButtonPos(2,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 2), OSCreateButton(2, openZig)});
            break;
        case FINISH_SETUP: DrawPermanentButton((GUI_BUTTON_LIKE){"OK", GetButtonPos(1,
            (GUI_POINT){50, 60} ,DEFAULT_BUTTON_SIZE, 1), OSCreateButton(1, openMainGUI)});
            break;
        default: break;
    }
}

void changeGreetingText();

void(*changeTextFunc)(void) = changeGreetingText;

void animateText() {
    static int subStrPos = 1;
    memcpy(subStr, greetingDialogue[dialoguePos], subStrPos);
    subStr[subStrPos] = '\0';
    greetingText->text = subStr;
    subStrPos++;
    if (subStrPos == strlen(greetingDialogue[dialoguePos]) + 1) {
        subStrPos = 1;
        animDone = 1;
        dialoguePos++;

        OSDoAfterMillis(IDT_DO_AFTER, WAIT_AFTER_TEXT, changeTextFunc);
        OSKillTimer(IDT_DO_AFTER_2);
    }
}

void changeGreetingText() {
    if (animDone) {
        animDone = 0;
        createButtons();
        if (dialoguePos < WELCOME_TEXT_END)
            OSDoAfterMillis(IDT_DO_AFTER_2, TYPING_SPEED, animateText);
        OSKillTimer(IDT_DO_AFTER);
    }
}

void performCheck() {
    success();
}

void changePerformCheckText() {
    if (animDone) {
        animDone = 0;
        if (dialoguePos < PERFORM_CHECK_TEXT_END)
            OSDoAfterMillis(IDT_DO_AFTER_2, TYPING_SPEED, animateText);
        else performCheck();
        OSKillTimer(IDT_DO_AFTER);
    }
}

void changeFailureText() {
    if (animDone) {
        animDone = 0;
        createButtons();
        if (dialoguePos < ZIG_OPTIONS_TEXT_END)
            OSDoAfterMillis(IDT_DO_AFTER_2, TYPING_SPEED, animateText);
        OSKillTimer(IDT_DO_AFTER);
    }
}

void changeSuccessText() {
    if (animDone) {
        animDone = 0;
        createButtons();
        if (dialoguePos < SUCCESS_TEXT_END)
            OSDoAfterMillis(IDT_DO_AFTER_2, TYPING_SPEED, animateText);
        OSKillTimer(IDT_DO_AFTER);
    }
}

void deleteButtons() {
    OSDestroyButtonById(1);
    OSDestroyButtonById(2);
    OSDestroyButtonById(3);
    OSDestroyButtonById(4);
}

void failureOptions() {
    animDone = 1;
    dialoguePos = ZIG_OPTIONS_TEXT_START;
    changeTextFunc = changeFailureText;
    changeFailureText();
}

void success() {
    animDone = 1;
    deleteButtons();
    dialoguePos = SUCCESS_TEXT_START;
    changeTextFunc = changeSuccessText;
    changeSuccessText();
}

void performCheckText() {
    animDone = 1;
    deleteButtons();
    changeTextFunc = changePerformCheckText;
    changePerformCheckText();
}

void WelcomeGUI() {
    greetingText = DrawPermanentText((GUI_TEXT){"", 50, 50, 14});
    changeGreetingText();
}