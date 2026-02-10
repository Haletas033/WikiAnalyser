//
// Created by halet on 12/31/2025.
//

#include "../include/core/unwanted.h"

//Handles character logic not unwantedCharacters only
void Add_CHAR(unsigned short* charactersSize, char* caCharacters[], const char caNewCharacters[]) {
    const unsigned short uOldSize = *charactersSize;
    const unsigned short uNewSize = strlen(caNewCharacters) + 1;
    const unsigned short uSize = uOldSize + uNewSize;

    char* buffer = malloc(uSize * sizeof(char));
    if (!buffer) return;

    memcpy(buffer, *caCharacters, uOldSize);
    memcpy(buffer + uOldSize, caNewCharacters, uNewSize);

    free((char*)*caCharacters);
    *caCharacters = buffer;
    *charactersSize = uSize;
}

//Handles string logic not unwantedStrings only
void Add_STRING(unsigned short* stringsSize, char** szaStrings[], char* szaNewStrings[], unsigned short count) {
    const unsigned short uOldSize = *stringsSize;
    const unsigned short uSize = uOldSize + count;

    char** buffer = malloc(uSize * sizeof(char*));
    if (!buffer) return;

    unsigned short i;
    for (i = 0; i < uOldSize; i++) {
        buffer[i] = strdup((*szaStrings)[i]);
        if (!buffer[i]) {
            unsigned short j;
            for (j = 0; j < i; j++) free(buffer[j]);
            free(buffer);
            return;
        }
    }


    for (i = 0; i < count; i++) {
        buffer[uOldSize + i] = malloc(strlen(szaNewStrings[i]) + 1);
        if (!buffer[uOldSize + i]) {
            unsigned short j;
            for (j = uOldSize; j < uOldSize + i; j++) free(buffer[j]);
            free(buffer);
            return;
        }
        strcpy(buffer[uOldSize + i], szaNewStrings[i]);
    }

    for (i = 0; i < uOldSize; i++) free((*szaStrings)[i]);
    free(*szaStrings);

    *szaStrings = buffer;
    *stringsSize = uSize;
}

void Delete_CHAR(unsigned short* charactersSize, char* caCharacters[], const char caNewCharacters[]) {
    char *newArray = NULL;

    int currNewArrayPos = 0;

    bool found = false;

    int i;
    for (i = 0; i < *charactersSize; i++) {
        found = false;
        int c;
        for (c = 0; c < strlen(caNewCharacters); c++) {
            if ((*caCharacters)[i] == caNewCharacters[c]) {
                found = true;
                break;
            }
        }
        if (!found) {
            char *tmp = realloc(newArray, currNewArrayPos + 1);
            if (!tmp) {
                free(newArray);
                return;
            }
            newArray = tmp;
            newArray[currNewArrayPos++] = (*caCharacters)[i];
        }
    }
    free(*caCharacters);
    *caCharacters = newArray;
    *charactersSize = currNewArrayPos;
}

void Delete_STRING(unsigned short* stringsSize, char** szaStrings[], char* szaNewStrings[], unsigned short count) {
    char **newArray = NULL;

    int currNewArrayPos = 0;

    bool found = false;

    int i;
    for (i = 0; i < *stringsSize; i++) {
        found = false;
        int c;
        for (c = 0; c < count; c++) {
            if (strcmp((*szaStrings)[i], szaNewStrings[c]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            char **tmp = realloc(newArray, (currNewArrayPos + 1) * sizeof(char*));
            if (!tmp) {
                free(newArray);
                return;
            }
            newArray = tmp;
            newArray[currNewArrayPos++] = (*szaStrings)[i];
        }
    }
    free(*szaStrings);
    *szaStrings = newArray;
    *stringsSize = currNewArrayPos;
}

void Delete_CONTAINER(unsigned short* stringsSize, char** szaHeads[], char** szaTails[], char* szaNewHeads[], unsigned short count) {
    char **newHeads = NULL;
    char **newTails = NULL;

    int currNewArrayPos = 0;

    bool found = false;

    int i;
    for (i = 0; i < *stringsSize; i++) {
        found = false;
        int c;
        for (c = 0; c < count; c++) {
            if (strcmp((*szaHeads)[i], szaNewHeads[c]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            char **tmpHeads = realloc(newHeads, (currNewArrayPos + 1) * sizeof(char*));
            char **tmpTails = realloc(newTails, (currNewArrayPos + 1) * sizeof(char*));
            if (!tmpHeads) {
                free(newHeads);
                return;
            }
            if (!tmpTails) {
                free(newTails);
                return;
            }
            newHeads = tmpHeads;
            newTails = tmpTails;
            newHeads[currNewArrayPos] = (*szaHeads)[i];
            newTails[currNewArrayPos] = (*szaTails)[i];
            currNewArrayPos++;
        }
    }
    free(*szaHeads);
    free(*szaTails);

    *szaHeads = newHeads;
    *szaTails = newTails;

    *stringsSize = currNewArrayPos;
}

void ModifyUnwanted_CHAR(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, const char caNewCharacters[]) {
    switch (iModifier) {
        case ADD: {
            Add_CHAR(&unwanted->m_ushortUnwantedCharactersSize, &unwanted->m_paUnwantedCharacters, caNewCharacters);
            break;
        }
        case DELETE: {
            Delete_CHAR(&unwanted->m_ushortUnwantedCharactersSize, &unwanted->m_paUnwantedCharacters, caNewCharacters);
            break;
        }
        case CLEAR: {
            unwanted->m_ushortUnwantedCharactersSize = 0;
            free(unwanted->m_paUnwantedCharacters);
            unwanted->m_paUnwantedCharacters = NULL;
            break;
        }
        default: break;
    }
}

//Purely for unwantedStrings
void ModifyUnwanted_STRING(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char* szaNewStrings[], unsigned short count) {
    if (count == 0 && iModifier != CLEAR) return;

    switch (iModifier) {
        case ADD: {
            Add_STRING(&unwanted->m_ushortUnwantedStringsSize, &unwanted->m_paUnwantedStrings , szaNewStrings, count);
            break;
        }
        case DELETE: {
            Delete_STRING(&unwanted->m_ushortUnwantedStringsSize, &unwanted->m_paUnwantedStrings , szaNewStrings, count);
            break;
        }
        case CLEAR: {
            unwanted->m_ushortUnwantedStringsSize = 0;
            free(unwanted->m_paUnwantedStrings);
            unwanted->m_paUnwantedStrings = NULL;
            break;
        }
        default: break;
    }

}

void ModifyUnwanted_CONTAINER(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier,
    char* szaNewHeads[], char* szaNewTails[],
    const unsigned short ushortContainerCount)
{
    if (ushortContainerCount == 0 && iModifier != CLEAR) return;

    switch (iModifier) {
        case ADD: {
            unsigned short fakeSize = unwanted->m_unwantedContainers.m_ushortUnwantedContainersSize;

            //Handle heads
            Add_STRING(
                &unwanted->m_unwantedContainers.m_ushortUnwantedContainersSize,
                &unwanted->m_unwantedContainers.m_paUnwantedHeads,
                szaNewHeads, ushortContainerCount
            );

            //Handle tails
            Add_STRING(
                &fakeSize /* prevent containerSize from changing twice by using fakeSize */,
                &unwanted->m_unwantedContainers.m_paUnwantedTails,
                szaNewTails, ushortContainerCount
            );
            break;
        }
        case DELETE: {
            Delete_CONTAINER(
                &unwanted->m_unwantedContainers.m_ushortUnwantedContainersSize,
                &unwanted->m_unwantedContainers.m_paUnwantedHeads,
                &unwanted->m_unwantedContainers.m_paUnwantedTails,
                szaNewHeads,
                ushortContainerCount
            );
            break;
        }
        case CLEAR: {
            //Clear heads
            free(unwanted->m_unwantedContainers.m_paUnwantedHeads);
            unwanted->m_unwantedContainers.m_paUnwantedHeads = NULL;

            //Clear tails
            free(unwanted->m_unwantedContainers.m_paUnwantedTails);
            unwanted->m_unwantedContainers.m_paUnwantedTails = NULL;

            unwanted->m_unwantedContainers.m_ushortUnwantedContainersSize = 0;
            break;
        }
        default: break;
    }
}