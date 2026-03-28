//
// Created by halet on 2/18/2026.
//

#include "../include/welcomeGUI.h"

#include <stdio.h>
#include <string.h>

#include "../include/curl.h"
#include "../include/topN.h"
#include "../include/core/articleParser.h"
Window* rootWindow = NULL;

void(*callback)(Window* wnd);
GUI_BUTTON_LIKE* progressBar;
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
    "Great choice. Just running a test to make sure everything is correct",
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

void performCheckText(Window* wnd, const int indeterminate, void(*func)(void));
void performCheck(Window* wnd);
void failureOptions(Window* wnd);
void success(Window* wnd);
void handleProgressBarCompletion();
void handleFullDumpCompletion();

void deleteButtons(Window* wnd);
void showButtons(Window* wnd, unsigned int shouldShow);
void destroyWelcomeGUI(Window* wnd) {
    deleteButtons(wnd);
    ClearGUIFull(&wnd->paintStacks, doAfters, buttonCommands);
}

char currentDumpPath[512] = {0};
char currentDumpFile[512] = {0};

void downloadFullWikipediaDump(Window* wnd) {
    const char* path = OSGetDirectoryPath();
    if (path == NULL) return;

    sprintf(currentDumpPath, "%s", path);
    sprintf(currentDumpFile, "%s\\wikiDump.bz2", path);

    OSCreateThreadForDownloadTo("https://dumps.wikimedia.org/enwiki/latest/enwiki-latest-pages-articles-multistream.xml.bz2", path, "/wikiDump.bz2");
    if (path[0] != '\0') {
        char dumpFilePath[512];
        sprintf(dumpFilePath, "%s\\enwiki-latest-pages-articles-multistream.xml", currentDumpPath);
        SetINIField("UserData/data.ini", "DumpPath", dumpFilePath);
        performCheckText(wnd, 1, handleFullDumpCompletion);
    }
}

void handleExitButton(Window* wnd) {
    ClearGUI(&wnd->paintStacks);
    wnd->windowRect = (GUI_RECT){0};

    OSDestroyButtonById(wnd, 9);
    OSDestroyButtonById(wnd, 10);
    OSDestroyButtonById(wnd, 11);

    showButtons(rootWindow, 1);
}

void handleTopNWikiDump(Window* wnd) {
    const char* inputText = OSGetInputBoxTextById(wnd, 9);
    handleExitButton(wnd);
    showButtons(rootWindow, 0);
    const char* path = OSGetDirectoryPath();
    if (path == NULL) return;
    if (path[0] != '\0') {
        ArticleViews* hashmap = LoadTopNFile("SystemData/topN.topn");
        OSCreateThreadForDownloadSpecialExportTo(GetTop(atoi(inputText), hashmap), atoi(inputText), path, "/top100.xml");

        char fullPath[512]; sprintf(fullPath, "%s/top100.xml", path);
        SetINIField("UserData/data.ini", "DumpPath", fullPath);
    }
    performCheckText(rootWindow, 0, handleProgressBarCompletion);
}

void downloadTopNWikipediaDump(Window* wnd) {
    Window* childWnd = malloc(sizeof(Window));
    *childWnd = (Window){25, 25, 50, 50};
    OSCreateChildWindow(41, "Input", childWnd);

    showButtons(wnd, 0);

    DrawPermanentWindow(childWnd, wnd);
    DrawPermanentText((GUI_TEXT){"Input the amount of articles you want", 50, 25, 18}, childWnd);
    DrawPermanentButton((GUI_BUTTON_LIKE){"Input", 40, 40, 20, 20, OSCreateInputBox(9, childWnd)},childWnd);
    DrawPermanentButton((GUI_BUTTON_LIKE){"EXIT", 0, 75, 50, 25, OSCreateButton(10, handleExitButton, childWnd)},childWnd);
    DrawPermanentButton((GUI_BUTTON_LIKE){"OK", 50, 75, 50, 25, OSCreateButton(11, handleTopNWikiDump, childWnd)},childWnd);
}



void handleCustomWikiDump(Window* wnd) {
    const char* inputText = OSGetInputBoxTextById(wnd, 9);
    handleExitButton(wnd);
    showButtons(rootWindow, 0);
    const char* path = OSGetDirectoryPath();
    if (path == NULL) return;
    if (path[0] != '\0') {
        const char** names = parseCommaSeperated(inputText);
        //Get length of names
        int len = 0;
        while (names[len] != NULL) len++;
        CurlDownloadWithSpecialExportTo(names, len, path, "/top100.xml");
        SetINIField("UserData/data.ini", "DumpPath", path);
    }
    performCheckText(rootWindow, 0, handleProgressBarCompletion);
}

void downloadCustomWikipediaDump(Window* wnd) {
    Window* childWnd = malloc(sizeof(Window));
    *childWnd = (Window){25, 25, 50, 50};
    OSCreateChildWindow(42, "Input", childWnd);

    showButtons(wnd, 0);

    DrawPermanentWindow(childWnd, wnd);
    DrawPermanentText((GUI_TEXT){"Input a list of comma seperated article names", 50, 5, 18}, childWnd);
    DrawPermanentText((GUI_TEXT){"with underscores for spaces:", 50, 10, 18}, childWnd);
    DrawPermanentButton((GUI_BUTTON_LIKE){"Input", 0, 25, 100, 50, OSCreateInputBox(9, childWnd)},childWnd);
    DrawPermanentButton((GUI_BUTTON_LIKE){"EXIT", 0, 75, 50, 25, OSCreateButton(10, handleExitButton, childWnd)},childWnd);
    DrawPermanentButton((GUI_BUTTON_LIKE){"OK", 50, 75, 50, 25, OSCreateButton(11, handleCustomWikiDump, childWnd)},childWnd);
}

void openWikipediaDump(Window* wnd) {
    const char* path = OSGetFilePath();
    if (path[0] != '\0') {
        SetINIField("UserData/data.ini", "DumpPath", path);
        performCheckText(wnd, -1, handleProgressBarCompletion);
    }
}

void zigDownloadSuccess() {
    handleProgressBarCompletion();
    success(rootWindow);
}

void handleZigDownloadCompletion() {
    progressBar = NULL;
    OSDestroyButtonById(rootWindow, 29);
    char exeDir[512]; OSGetEXEDir(exeDir, 512);
    char zigInstallPath[512];
    char zigZipPath[512];
    char zigExePath[512];

    sprintf(zigInstallPath, "%s\\UserData\\zig", exeDir);
    sprintf(zigZipPath, "%s\\zig.zip", zigInstallPath);

    char extractCmd[1024];
    sprintf(extractCmd, "powershell -Command \"Expand-Archive -Path '%s' -DestinationPath '%s' -Force\"", zigZipPath, zigInstallPath);
    OSCreateThreadForSystemCall(extractCmd, handleProgressBarCompletion);

    sprintf(zigExePath, "%s\\zig-windows-x86_64-0.14.0\\zig.exe", zigInstallPath);
    SetINIField("UserData/data.ini", "ZigPath", zigExePath);

    greetingText->text = "Extracting... (This may take a while)";
}

void downloadZig(Window* wnd) {
    char exeDir[512]; OSGetEXEDir(exeDir, 512);
    char zigPath[512];
    sprintf(zigPath, "%s\\UserData\\zig", exeDir);

    OSCreateDirectory(zigPath);
    OSCreateThreadForDownloadTo("https://ziglang.org/download/0.14.0/zig-windows-x86_64-0.14.0.zip", zigPath, "/zig.zip");
    performCheckText(wnd, 1, handleZigDownloadCompletion);
}

void openZig(Window* wnd) {
    const char* path = OSGetFilePath();
    if (path[0] != '\0') {
        SetINIField("UserData/data.ini", "ZigPath", path);
        performCheck(wnd);
    }
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
    char cmd[512];
    sprintf(cmd, "\"%s\" version > nul 2>&1", GetINIField("UserData/data.ini", "ZigPath"));
    if (system("zig version > nul 2>&1") == 1 || system(cmd) == 0)
        success(wnd);
    else
        failureOptions(wnd);
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

void showButtons(Window* wnd, const unsigned int shouldShow) {
    ShowButtonLike(&wnd->paintStacks.buttons[0], shouldShow);
    ShowButtonLike(&wnd->paintStacks.buttons[1], shouldShow);
    ShowButtonLike(&wnd->paintStacks.buttons[2], shouldShow);
    ShowButtonLike(&wnd->paintStacks.buttons[3], shouldShow);
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

void handleProgressBarCompletion() {
    progressBar = NULL;
    OSDestroyButtonById(rootWindow, 29);
    changeTextFunc = changePerformCheckText;
    changePerformCheckText(rootWindow);
}

void handleFullDumpCompletion() {
    progressBar = NULL;
    OSDestroyButtonById(rootWindow, 29);
    char extractCmd[1024];
    sprintf(extractCmd, "tar -xjf \"%s\" -C \"%s\"", currentDumpFile, currentDumpPath);
    OSCreateThreadForSystemCall(extractCmd, handleProgressBarCompletion);
}

void performCheckText(Window* wnd, const int indeterminate, void(*func)(void)) {
    animDone = 1;
    deleteButtons(wnd);
    if (indeterminate != -1)
        progressBar = DrawPermanentButton((GUI_BUTTON_LIKE){"Download Progress", 10, 40, 80, 20, OSCreateProgressBar(29, func, wnd, indeterminate)}, wnd);
    else {
        changeTextFunc = changePerformCheckText;
        changePerformCheckText(rootWindow);
    }
}

void WelcomeGUI(Window* wnd, void(*funcCallback)(Window* wnd)) {
    rootWindow = wnd;
    callback = funcCallback;
    greetingText = DrawPermanentText((GUI_TEXT){"", 50, 50, 14}, wnd);
    changeGreetingText(wnd);
}