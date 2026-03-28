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

#include "../include/graphGUI.h"

PaintStacks cleanupPaintStacks = {0};
PaintStacks fieldsPaintStacks = {0};
PaintStacks parsePaintStacks = {0};

Field intFields = {0};
Field floatFields = {0};
Field boolFields = {0};
Field stringFields = {0};

Article article;
Article* articles = NULL;
unsigned int articleCount;

Window* properties;
Window* visualiserScreen;

int memPerArticle = sizeof(Article);

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

void HideFieldsButtons() {
    //Hide main buttons
    OSShowButtonById(properties, 26, 0);
    OSShowButtonById(properties, 27, 0);
    OSShowButtonById(properties, 28, 0);
    //Hide generated Buttons
    int i;
    for (i = 0; i < intFields.fieldsSize; i++) {
        if (intFields.fieldXButtons[i] != NULL) OSShowButtonById(properties, i+32, 0);
        if (intFields.ifieldNameInputs[i] != NULL) OSShowButtonById(properties, i+80, 0);
    }
    for (i = 0; i < floatFields.fieldsSize; i++) {
        if (floatFields.fieldXButtons[i] != NULL) OSShowButtonById(properties, i+32, 0);
        if (floatFields.ifieldNameInputs[i] != NULL) OSShowButtonById(properties, i+80, 0);
    }
    for (i = 0; i < boolFields.fieldsSize; i++) {
        if (boolFields.fieldXButtons[i] != NULL) OSShowButtonById(properties, i+32, 0);
        if (boolFields.ifieldNameInputs[i] != NULL) OSShowButtonById(properties, i+80, 0);
    }
    for (i = 0; i < stringFields.fieldsSize; i++) {
        if (stringFields.fieldXButtons[i] != NULL) OSShowButtonById(properties, i+32, 0);
        if (stringFields.ifieldNameInputs[i] != NULL) OSShowButtonById(properties, i+80, 0);
    }
}

void HideParseButtons() {
    //Can be hidden this way as the paintStacks has changed and when it changed back it will automatically show the buttons again
    OSShowButtonById(properties, 15, 0);
    OSShowButtonById(properties, 16, 0);
    OSShowButtonById(properties, 17, 0);
    OSShowButtonById(properties, 18, 0);
    OSShowButtonById(properties, 19, 0);
    OSShowButtonById(properties, 22, 0);
    OSShowButtonById(properties, 23, 0);
    OSShowButtonById(properties, 24, 0);
    OSShowButtonById(properties, 25, 0);
}

void SwitchWindowPaintStacksToCleanup(Window* _) {
    properties->paintStacks = cleanupPaintStacks;
    HideFieldsButtons();
    HideParseButtons();
}

const char* items[] = {"Int Fields", "Float Fields", "Bool Fields", "String Fields"};

void DrawFieldWidgets(PaintStacks* ps, Field* field) {
    //Update memory per article
    char memPerArticleString[32];
    sprintf(memPerArticleString, "Memory per Article: %d bytes", memPerArticle);
    ps->texts[1].text = strdup(memPerArticleString);

    //Clear fields from paintStacks
    int i;
    for (i = 3; i < ps->buttonsSize; i++)
        ps->buttons[i].shouldShow = 0;

    for (i = 2; i < ps->textsSize; i++)
        memset(&ps->texts[i], 0, sizeof(GUI_TEXT));
    ps->textsSize-=(i-2);

    int pos = 0;
    for (i = 0; i < field->fieldsSize; i++) {
        if (field->fieldXButtons[i] != NULL) {
            pos++;
            DrawPermanentTextToPaintStacks((GUI_TEXT){"Name:", 20, 17+(4*pos), 35}, ps);

            ps->buttons[field->fieldNameInputsGUI[i]] = (GUI_BUTTON_LIKE){"Input field name...",
                (GUI_RECT){30,15+4*pos, 50, 4},  ps->buttons[field->fieldNameInputsGUI[i]].buttonLoc, 1};
            ps->buttons[field->fieldXButtonsGUI[i]] = (GUI_BUTTON_LIKE){"X",
                (GUI_RECT){80,15+4*pos, 10, 4},  ps->buttons[field->fieldXButtonsGUI[i]].buttonLoc, 1};
        }
    }

    properties->paintStacks = fieldsPaintStacks;
}

void DrawFieldWidgetsOnDropDownChange(Window* wnd) {
    const int index = OSGetDropdownCurrentlySelected(28, wnd);
    Field *field;

    switch (index) {
        case 0: field = &intFields;    break;
        case 1: field = &floatFields;  break;
        case 2: field = &boolFields;   break;
        case 3: field = &stringFields; break;
        default: return;
    }

    DrawFieldWidgets(&fieldsPaintStacks, field);
}

void RemoveField(Window* wnd) {
    const int index = OSGetDropdownCurrentlySelected(28, wnd);
    Field *field;

    switch (index) {
        case 0: field = &intFields;    memPerArticle-=sizeof(int);   break;
        case 1: field = &floatFields;  memPerArticle-=sizeof(float); break;
        case 2: field = &boolFields;   memPerArticle-=sizeof(int);   break;
        case 3: field = &stringFields; memPerArticle-=sizeof(char*); break;
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
        case 0: field = &intFields;    memPerArticle+=sizeof(int);   break;
        case 1: field = &floatFields;  memPerArticle+=sizeof(float); break;
        case 2: field = &boolFields;   memPerArticle+=sizeof(int);   break;
        case 3: field = &stringFields; memPerArticle+=sizeof(char*); break;
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

void ApplyFieldType(Window* wnd, Field field, int offset, enum Type type) {
    int i;
    for (i = 0; i < field.fieldsSize; i++) {
        if (field.ifieldNameInputs[i] != NULL) {
            AddField(&article, type, OSGetInputBoxTextById(wnd, (i+80)*offset));
        }
    }
}

void ApplyFieldInput(Window* wnd) {
    ApplyFieldType(wnd, intFields, 1, FIELD_INT);
    ApplyFieldType(wnd, floatFields, 2, FIELD_FLOAT);
    ApplyFieldType(wnd, boolFields, 3, FIELD_BOOL);
    ApplyFieldType(wnd, stringFields, 4, FIELD_STRING);
}

void SetupFieldsPaintStacks(PaintStacks* ps, Window* wnd) {
    DrawPermanentTextToPaintStacks((GUI_TEXT){"Field Type", 50, 5, 35}, ps);

    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Select an option...", 10, 7, 70, 3, OSCreateDropdown(28, DrawFieldWidgetsOnDropDownChange, wnd, items, 4)}, ps);

    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"+",
        (GUI_RECT){80,7, 10, 3},  OSCreateButton(27, AddFieldInput, wnd)}, ps);
    DrawPermanentTextToPaintStacks((GUI_TEXT){"Memory per Article: 88 bytes", 50, 12, 35}, ps);

    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Apply",
        (GUI_RECT){10,15, 80, 3},  OSCreateButton(26, ApplyFieldInput, wnd)}, ps);
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
    char fullCommand[2048];
    char userDataPath[512];
    sprintf(userDataPath, "%s\\UserData", exeDir);

    //Build to temp
    sprintf(fullCommand, "zig build-lib %s\\src\\main.zig -dynamic -O ReleaseSmall -femit-h -femit-bin=%s\\main_temp.dll",
        currentProjectPath, userDataPath);

    if (OSShellExecute(userDataPath, fullCommand) != 0) return;

    //Free old
    OSFreeLibrary();

    //Replace
    char oldPath[512]; sprintf(oldPath, "%s\\main.dll", userDataPath);
    char tempPath[512]; sprintf(tempPath, "%s\\main_temp.dll", userDataPath);
    DeleteFile(oldPath);
    MoveFile(tempPath, oldPath);

    char libPath[512]; sprintf(libPath, "%s\\main.dll", userDataPath);
    OnArticle = OSLoadLibrary(libPath, "OnArticle");
}

void RunParser(Window* _) {
    OSCreateThreadForParse(GetINIField("UserData/data.ini", "DumpPath"), &articles, &article, &articleCount);
}

GraphData graphData = {0};
const char* valueFields[128];
const char* keyFields[128];

void CreateGraphValueData(Window* wnd, const unsigned int id, const unsigned int intSize, const unsigned int floatSize,
    unsigned int* graphIntSize, unsigned int* graphFloatSize,
    int** graphIntValues, int** graphFloatValues
    ) {
    const unsigned int maxValueFields = intSize+floatSize;
    int selectedIds[maxValueFields];
    int currentlySelectedCount = OSGetDropdownMultiSelectCurrentlySelected(id, wnd, selectedIds, maxValueFields);

    int i;
    for (i = 0; i < currentlySelectedCount; i++) {
        if (selectedIds[i] < intSize) {
            (*graphIntSize)++;
            *graphIntValues = realloc(*graphIntValues, sizeof(int) * *graphIntSize);
            (*graphIntValues)[*graphIntSize-1] = selectedIds[i];
        } else {
            (*graphFloatSize)++;
            *graphFloatValues = realloc(*graphFloatValues, sizeof(int) * *graphFloatSize);
            (*graphFloatValues)[*graphFloatSize-1] = selectedIds[i];
        }
    }
}

void CreateKeyData(Window* wnd, const unsigned int id, const unsigned int keySize,
    unsigned int* graphKeysSize, int** graphKeysValues
    ) {
    int selectedIds[keySize];
    int currentlySelectedCount = OSGetDropdownMultiSelectCurrentlySelected(id, wnd, selectedIds, keySize);

    int i;
    for (i = 0; i < currentlySelectedCount; i++) {
        (*graphKeysSize)++;
        *graphKeysValues = realloc(*graphKeysValues, sizeof(char*) * *graphKeysSize);
        (*graphKeysValues)[*graphKeysSize-1] = selectedIds[i];
    }
}

char* articleName;
void Analyse(Window* wnd) {
    const int articleChoiceIndex = OSGetDropdownCurrentlySelected(19, wnd);
    Article finalArticle;

    switch (articleChoiceIndex) {
        case 0: finalArticle = FindArticleByName(articleName, articles, articleCount); break;
        case 1: finalArticle = AverageArticles(article, articles, articleCount); break;
        case 2: finalArticle = SumArticles(article, articles, articleCount);   break;
        default: return;
    }

    if (finalArticle.title == NULL) return;

    free(graphData.intFieldIndices);
    free(graphData.floatFieldIndices);
    free(graphData.boolFieldIndices);
    free(graphData.stringFieldIndices);
    graphData = (GraphData){0};

    const int index = OSGetDropdownCurrentlySelected(25, wnd);

    CreateGraphValueData(wnd, 23, articles[0].intFieldsSize, articles[0].floatFieldsSize,
       &graphData.intFieldsCount, &graphData.floatFieldsCount, &graphData.intFieldIndices, &graphData.floatFieldIndices);
    if (index == 3) {
        CreateGraphValueData(wnd, 22, articles[0].intFieldsSize, articles[0].floatFieldsSize,
            &graphData.YIntFieldsCount, &graphData.YFloatFieldsCount, &graphData.YIntFieldIndices, &graphData.YFloatFieldIndices);
    } else {
        CreateKeyData(wnd, 22, articles[0].stringFieldsSize, &graphData.stringFieldsCount, &graphData.stringFieldIndices);
    }

    ClearGUI(&visualiserScreen->paintStacks);

    if (index != 3)
        OSDestroyButtonById(visualiserScreen, 2);

    switch (index) {
        case 0: DrawPieGraph(finalArticle, graphData, visualiserScreen);   break;
        case 1: DrawPercentageBarGraph(finalArticle, graphData, visualiserScreen); break;
        case 2: DrawBarGraph(finalArticle, graphData, visualiserScreen);   break;
        case 3: DrawScatterGraph(finalArticle, graphData, visualiserScreen); break;
        default: return;
    }
}

void CreateStandardGraphInputs(Window* wnd) {
    int i = 0;
    for (; i < article.intFieldsSize; i++)
        valueFields[i] = article.intFieldNames[i];

    int j;
    for (j = 0; j < article.floatFieldsSize; j++, i++)
        valueFields[i] = article.floatFieldNames[j];

    const int numberFieldsCount = i+j;

    for (i = 0; i < article.stringFieldsSize; i++) {
        keyFields[i] = article.stringFieldNames[i];
    }

    DrawPermanentTextToPaintStacks((GUI_TEXT){"X Fields", 30, 58 ,25}, &parsePaintStacks);
    DrawPermanentTextToPaintStacks((GUI_TEXT){"Key Names (Optional)", 70, 58 ,25}, &parsePaintStacks);

    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"X Fields",
        (GUI_RECT){10,60, 40, 25}, OSCreateMultiSelectDropdown(23, wnd, valueFields, numberFieldsCount)}, &parsePaintStacks);

    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Key Names (Optional)",
        (GUI_RECT){50,60, 40, 25}, OSCreateMultiSelectDropdown(22, wnd, keyFields, i)}, &parsePaintStacks);

    properties->paintStacks = parsePaintStacks;
}

void CreateScatterGraphInputs(Window* wnd) {
    int i = 0;
    for (; i < article.intFieldsSize; i++)
        valueFields[i] = article.intFieldNames[i];

    int j;
    for (j = 0; j < article.floatFieldsSize; j++, i++)
        valueFields[i] = article.floatFieldNames[j];

    DrawPermanentTextToPaintStacks((GUI_TEXT){"X Fields", 30, 58 ,25}, &parsePaintStacks);
    DrawPermanentTextToPaintStacks((GUI_TEXT){"Y Fields", 70, 58 ,25}, &parsePaintStacks);

    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"X Fields",
        (GUI_RECT){10,60, 40, 25}, OSCreateMultiSelectDropdown(23, wnd, valueFields, i+j)}, &parsePaintStacks);

    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Y Fields",
        (GUI_RECT){50,60, 40, 25}, OSCreateMultiSelectDropdown(22, wnd, valueFields, i+j)}, &parsePaintStacks);

    properties->paintStacks = parsePaintStacks;
}

void CreateGraphInputs(Window* wnd) {
    OSDestroyButtonById(wnd, 23);
    OSDestroyButtonById(wnd, 22);
    int i;
    for (i = 0; i < parsePaintStacks.textsSize; i++)
        memset(&parsePaintStacks.texts[i], 0, sizeof(GUI_TEXT));
    parsePaintStacks.textsSize-=i;
    wnd->paintStacks = parsePaintStacks;


    const int index = OSGetDropdownCurrentlySelected(25, wnd);

    switch (index) {
        case 0: CreateStandardGraphInputs(wnd);   break;
        case 1: CreateStandardGraphInputs(wnd); break;
        case 2: CreateStandardGraphInputs(wnd);   break;
        case 3: CreateScatterGraphInputs(wnd); break;
        default: return;
    }
}

void HandleArticleNamePrompt(Window* wnd) {
    const char* name = OSGetInputBoxTextById(wnd, 20);
    if (name[0] == '\0') return;
    articleName = strdup(name);
    ExitNamePrompt(wnd);
}

void HandleArticleChoiceChange(Window* wnd) {
    const int index = OSGetDropdownCurrentlySelected(19, wnd);

    if (index == 0) {
        //Name prompt
        properties->paintStacks = (PaintStacks){0};
        HideParseButtons();

        Window* namePrompt = malloc(sizeof(Window));
        *namePrompt = (Window){10, 40, 80, 20};
        OSCreateChildWindowOnChildWindow(9, "NamePrompt", namePrompt, properties);
        DrawPermanentWindow(namePrompt, properties);
        DrawPermanentText((GUI_TEXT){"Enter the Article name", 50, 5, 50}, namePrompt);
        DrawPermanentButton((GUI_BUTTON_LIKE){"Enter Name...", 0, 25, 100, 50, OSCreateInputBox(20, namePrompt)}, namePrompt);
        DrawPermanentButton((GUI_BUTTON_LIKE){"EXIT", 0, 75, 50, 25, OSCreateButton(21, ExitNamePrompt, namePrompt)}, namePrompt);
        DrawPermanentButton((GUI_BUTTON_LIKE){"OK", 50, 75, 50, 25, OSCreateButton(22, HandleArticleNamePrompt, namePrompt)}, namePrompt);
    }
}

const char* articleChoices[3] = {"By Name", "All Averaged", "All Summed"};
const char* graphChoices[5] = {"Pie Graph", "Percentage Bar", "Bar Graph", "Scatter Plot", "Text"};
void SetupParsePaintStacks(PaintStacks* ps, Window* wnd) {
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"New Project",
        (GUI_RECT){10,5, 80, 10},  OSCreateButton(15, NewProject, wnd)}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Open Project",
        (GUI_RECT){10,15, 80, 10},  OSCreateButton(16, OpenProject, wnd)}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Build",
        (GUI_RECT){10,25, 80, 10},  OSCreateButton(17, BuildProject, wnd)}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Parse",
        (GUI_RECT){10,35, 80, 10},  OSCreateButton(18, RunParser, wnd)}, ps);

    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Article Choice",
        (GUI_RECT){10,47, 80, 5}, OSCreateDropdown(19, HandleArticleChoiceChange, wnd, articleChoices, 3)}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Graph Choice",
        (GUI_RECT){10,50, 80, 5}, OSCreateDropdown(25, CreateGraphInputs, wnd, graphChoices, 5)}, ps);
    DrawPermanentButtonToPaintStacks((GUI_BUTTON_LIKE){"Analyse",
        (GUI_RECT){10,85, 80, 10},  OSCreateButton(24, Analyse, wnd)}, ps);
}

void SwitchWindowPaintStacksToParse(Window* _) {
    properties->paintStacks = parsePaintStacks;
    HideCleanupButtons();
    HideFieldsButtons();
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
    visualiserScreen = malloc(sizeof(Window));

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
