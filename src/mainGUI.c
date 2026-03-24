//
// Created by halet on 09/03/26
//

#include <stdlib.h>
#include "../include/mainGUI.h"
#include "../include/core/unwanted.h"
#include "../include/core/cleanup.h"

#include "../rawZigForC/build_zig.h"
#include "../rawZigForC/build_zig_zon.h"
#include "../rawZigForC/zig_root.h"
#include "../rawZigForC/zig_main.h"

#include <stdio.h>

PaintStacks cleanupPaintStacks = {0};
PaintStacks fieldsPaintStacks = {0};
PaintStacks parsePaintStacks = {0};

Window* properties;

char* currentProjectPath;

void ApplyCleanup(Window* _) {
    UNWANTED unwanted = {0}; //Clear

    //Get unwanted chars
    ModifyUnwanted_CHAR(&unwanted, UNWANTED_ADD, OSGetInputBoxTextById(properties, 10));

    //Get unwanted Strings
    const char** unwantedStrings = parseCommaSeperated(OSGetInputBoxTextById(properties, 11));
    int unwantedStringsLen = 0;
    while (unwantedStrings[unwantedStringsLen] != NULL) unwantedStringsLen++;
    ModifyUnwanted_STRING(&unwanted, UNWANTED_ADD, unwantedStrings, unwantedStringsLen);

    //Get unwanted heads
    const char** unwantedHeads = parseCommaSeperated(OSGetInputBoxTextById(properties, 12));
    int unwantedHeadsLen = 0;
    while (unwantedHeads[unwantedHeadsLen] != NULL) unwantedHeadsLen++;

    //Get unwanted tails
    const char** unwantedTails = parseCommaSeperated(OSGetInputBoxTextById(properties, 13));
    int unwantedTailsLen = 0;
    while (unwantedTails[unwantedTailsLen] != NULL) unwantedTailsLen++;

    const unsigned int containerLen = unwantedHeadsLen < unwantedTailsLen ? unwantedHeadsLen : unwantedTailsLen;
    ModifyUnwanted_CONTAINER(&unwanted, UNWANTED_ADD, unwantedHeads, unwantedTails, containerLen);

    //Run cleanup
    CleanUpData(GetINIField("UserData/data.ini", "DumpPath"), unwanted);
}

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
        (GUI_RECT){10,45, 80, 10},  OSCreateButton(14, ApplyCleanup, wnd)}, ps);
}

void HideCleanupButtons() {
    //Can be hidden this way as the paintStacks has changed and when it changed back it will automatically show the buttons again
    OSShowButtonById(properties, 10, 0);
    OSShowButtonById(properties, 11, 0);
    OSShowButtonById(properties, 12, 0);
    OSShowButtonById(properties, 13, 0);
    OSShowButtonById(properties, 14, 0);
}

void HideParseButtons() {
    //Can be hidden this way as the paintStacks has changed and when it changed back it will automatically show the buttons again
    OSShowButtonById(properties, 15, 0);
    OSShowButtonById(properties, 16, 0);
    OSShowButtonById(properties, 17, 0);
    OSShowButtonById(properties, 18, 0);
    OSShowButtonById(properties, 19, 0);
}

void SwitchWindowPaintStacksToCleanup(Window* _) {
    properties->paintStacks = cleanupPaintStacks;
    HideParseButtons();
}

const char* items[] = {"Int Fields", "Float Fields", "Bool Fields", "String Fields"};



typedef struct Field {
    int fieldsSize;
    void* ifieldNameInputs[128];
    void* fieldXButtons[128];
    int fieldNameInputsGUI[128];
    int fieldXButtonsGUI[128];
    int freeSlots[128]; int freeSlotsSize;
} Field;

Field intFields = {0};
Field floatFields = {0};
Field boolFields = {0};
Field stringFields = {0};

void DrawFieldWidgets(PaintStacks* ps, Field* field) {
    //Clear fields from paintStacks

    int i;
    for (i = 2; i < ps->buttonsSize; i++)
        ps->buttons[i].shouldShow = 0;

    for (i = 1; i < ps->textsSize; i++)
        memset(&ps->texts[i], 0, sizeof(GUI_TEXT));
    ps->textsSize-=(i-1);

    int pos = 0;
    for (i = 0; i < field->fieldsSize; i++) {
        if (field->fieldXButtons[i] != NULL) {
            pos++;
            DrawPermanentTextToPaintStacks((GUI_TEXT){"Name:", 20, 12+(4*pos), 35}, ps);

            ps->buttons[field->fieldNameInputsGUI[i]] = (GUI_BUTTON_LIKE){"Input field name...",
                (GUI_RECT){30,10+4*pos, 50, 4},  ps->buttons[field->fieldNameInputsGUI[i]].buttonLoc, 1};
            ps->buttons[field->fieldXButtonsGUI[i]] = (GUI_BUTTON_LIKE){"X",
                (GUI_RECT){80,10+4*pos, 10, 4},  ps->buttons[field->fieldXButtonsGUI[i]].buttonLoc, 1};
        }
    }

    properties->paintStacks = fieldsPaintStacks;
}

void RemoveField(Window* wnd) {
    const int index = OSGetDropdownCurrentlySelected(28, wnd);
    Field *field;

    switch (index) {
        case 0: field = &intFields; break;
        case 1: field = &floatFields; break;
        case 2: field = &boolFields; break;
        case 3: field = &stringFields; break;
        default: return;
    }

    OSDestroyButtonById(wnd, currentButtonId+48);
    OSDestroyButtonById(wnd, currentButtonId);

    field->ifieldNameInputs[currentButtonId/(index+1)-80] = NULL;
    field->fieldXButtons[currentButtonId/(index+1)-32] = NULL;

    field->freeSlots[field->freeSlotsSize] = currentButtonId/(index+1);
    field->freeSlotsSize++;

    DrawFieldWidgets(&fieldsPaintStacks, field);
}

void AddFieldInput(Window* wnd) {
    const int index = OSGetDropdownCurrentlySelected(28, wnd);
    Field *field;

    switch (index) {
        case 0: field = &intFields; break;
        case 1: field = &floatFields; break;
        case 2: field = &boolFields; break;
        case 3: field = &stringFields; break;
        default: return;
    }

    int chosenNameInputPos = field->fieldsSize + 80;
    int chosenXButtonsPos = field->fieldsSize + 32;

    if (field->freeSlotsSize > 0) {
        chosenNameInputPos = field->freeSlots[field->freeSlotsSize-1] + 48;
        chosenXButtonsPos = field->freeSlots[field->freeSlotsSize-1];
        field->freeSlotsSize--;
    }

    field->ifieldNameInputs[chosenNameInputPos-80] = OSCreateInputBox(chosenNameInputPos*(index+1), wnd);
    field->fieldXButtons[chosenXButtonsPos-32] = OSCreateButton(chosenXButtonsPos*(index+1), RemoveField, wnd);

    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Input field name...",
        (GUI_RECT){0},  field->ifieldNameInputs[chosenNameInputPos-80]}, &fieldsPaintStacks);
    field->fieldNameInputsGUI[chosenNameInputPos-80] = fieldsPaintStacks.buttonsSize-1;
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"X",
        (GUI_RECT){0},  field->fieldXButtons[chosenXButtonsPos-32]}, &fieldsPaintStacks);
    field->fieldXButtonsGUI[chosenXButtonsPos-32] = fieldsPaintStacks.buttonsSize-1;

    if (field->freeSlotsSize == 0) field->fieldsSize++;

    DrawFieldWidgets(&fieldsPaintStacks, field);
}

void SetupFieldsPaintStacks(PaintStacks* ps, Window* wnd) {
    DrawPermanentTextToPaintStacks((GUI_TEXT){"Field Type", 50, 5, 35}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Select an option...", 10, 7, 70, 3, OSCreateDropdown(28, wnd, items, 4)}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"+",
        (GUI_RECT){80,7, 10, 3},  OSCreateButton(27, AddFieldInput, wnd)}, ps);
}

void SwitchWindowPaintStacksToFields(Window* _) {
    properties->paintStacks = fieldsPaintStacks;
    HideCleanupButtons();
    HideParseButtons();
}

void ExitNamePrompt(Window* wnd) {
    properties->paintStacks = parsePaintStacks;
    OSDestroyButtonById(properties, 9);
    OSDestroyButtonById(wnd, 20);
    OSDestroyButtonById(wnd, 21);
    OSDestroyButtonById(wnd, 22);
}

void HandleNamePrompt(Window* wnd) {
    const char* name = OSGetInputBoxTextById(wnd, 20);
    if (name[0] == '\0') return;

    char namePath[256]; sprintf(namePath, "%s/%s", "UserData/Projects", name);
    OSCreateDirectory(namePath);
    char namePathSrc[256]; sprintf(namePathSrc, "%s/%s/%s", "UserData/Projects", name, "src");
    OSCreateDirectory(namePathSrc);

    char buildPath[256]; sprintf(buildPath, "%s/%s", namePath, "build.zig");
    char buildZonPath[256]; sprintf(buildZonPath, "%s/%s", namePath, "build.zig.zon");
    char rootPath[256]; sprintf(rootPath, "%s/%s", namePath, "src/root.zig");
    char mainPath[256]; sprintf(mainPath, "%s/%s", namePath, "src/main.zig");

    //Zig files
    //Open
    FILE* build = fopen(buildPath, "w");
    FILE* buildZon = fopen(buildZonPath, "w");
    FILE* root = fopen(rootPath, "w");
    FILE* main = fopen(mainPath, "w");
    //Write
    fwrite(zig_build_zig, 1, zig_build_zig_len, build);
    fwrite(zig_build_zig_zon, 1, zig_build_zig_zon_len, buildZon);
    fwrite(zig_src_root_zig, 1, zig_src_root_zig_len, root);
    fwrite(zig_src_main_zig, 1, zig_src_main_zig_len, main);
    //Close
    fclose(build);
    fclose(buildZon);
    fclose(root);
    fclose(main);

    ExitNamePrompt(wnd);
}

void NewProject (Window* _) {
    properties->paintStacks = (PaintStacks){0};
    HideParseButtons();
    //Name prompt
    Window* namePrompt = malloc(sizeof(Window));
    *namePrompt = (Window){10, 40, 80, 20};
    OSCreateChildWindowOnChildWindow(9, "NamePrompt", namePrompt, properties);
    DrawPermanentWindow(namePrompt, properties);
    DrawPermanentText((GUI_TEXT){"Enter the Project Name:", 50, 5, 50}, namePrompt);
    DrawPermanentButton((GUI_BUTTON_LIKE){"Enter Project Name...", 0, 25, 100, 50, OSCreateInputBox(20, namePrompt)}, namePrompt);
    DrawPermanentButton((GUI_BUTTON_LIKE){"EXIT", 0, 75, 50, 25, OSCreateButton(21, ExitNamePrompt, namePrompt)}, namePrompt);
    DrawPermanentButton((GUI_BUTTON_LIKE){"OK", 50, 75, 50, 25, OSCreateButton(22, HandleNamePrompt, namePrompt)}, namePrompt);
}

void OpenProject(Window* _) {
    currentProjectPath = OSGetDirectoryPathInsideWikiAnalyser("UserData\\Projects");
    OSOpenAs(currentProjectPath, "src\\main.zig");
}

void BuildProject(Window* _) {
    char exeDir[512]; OSGetEXEDir(exeDir, 512);
    char fullCommand[1024]; sprintf(fullCommand, "%s %s\\src\\main.zig %s=%s\\main.dll", "zig build-lib", currentProjectPath, "-dynamic -O ReleaseSmall -femit-h -femit-bin", exeDir);

    if (OSShellExecute(currentProjectPath, fullCommand) != 0) return;

    char libPath[512]; sprintf(libPath, "%s\\%s", exeDir, "main.dll");
    OnArticle = OSLoadLibrary(libPath, "OnArticle");
}

void RunParser(Window* _) {
    Article **articles = malloc(sizeof(Article*));
    unsigned int* articleCount = malloc(sizeof(int));
    Article* article = calloc(1, sizeof(Article));
    AddField(article, FIELD_INT, "e_Count");
    AddField(article, FIELD_FLOAT, "Float Test");
    AddField(article, FIELD_BOOL, "Bool Test");
    AddField(article, FIELD_STRING, "String Test");
    OSCreateThreadForParse(GetINIField("UserData/data.ini", "DumpPath"), articles, article, &articleCount);
}

void SetupParsePaintStacks(PaintStacks* ps, Window* wnd) {
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"New Project",
        (GUI_RECT){10,5, 80, 10},  OSCreateButton(15, NewProject, wnd)}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Open Project",
        (GUI_RECT){10,15, 80, 10},  OSCreateButton(16, OpenProject, wnd)}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Build",
        (GUI_RECT){10,25, 80, 10},  OSCreateButton(17, BuildProject, wnd)}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Parse",
        (GUI_RECT){10,35, 80, 10},  OSCreateButton(18, RunParser, wnd)}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Run Analyser (advanced)",
        (GUI_RECT){10,45, 80, 10},  OSCreateButton(19, ApplyCleanup, wnd)}, ps);
}

void SwitchWindowPaintStacksToParse(Window* _) {
    properties->paintStacks = parsePaintStacks;
    HideCleanupButtons();
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

    //DrawPermanentButton((GUI_BUTTON_LIKE){"Download Progress", 10, 40, 80, 20, OSCreateProgressBar(29, NULL, visualiserScreen, 1)}, visualiserScreen);
    DrawPermanentText((GUI_TEXT){"Console Initialized:", 17, 17, 20}, console);

    SetupCleanupPaintStacks(&cleanupPaintStacks, properties);
    SetupFieldsPaintStacks(&fieldsPaintStacks, properties);
    SetupParsePaintStacks(&parsePaintStacks, properties);
    properties->paintStacks = cleanupPaintStacks;
    StartPropertiesModeSelector(propertiesModeSelector);
}
