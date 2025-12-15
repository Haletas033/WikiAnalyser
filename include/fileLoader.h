//
// Created by halet on 12/15/2025.
//

#ifndef FILELOADER_H
#define FILELOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include <stdbool.h>

#define HANDLE_FILE_ERROR(f) do { if (f) fclose(f); return NULL; } while(0)

typedef const char* PCSTRFILEPATH;

typedef const char* PARRAYCSTR_UNWANTED_CHARACTERS;
typedef unsigned short USHORT_UNWANTED_CHARACTERS_COUNT;

typedef char** PARRAYSTR_UNWANTED_STRINGS;
typedef unsigned short USHORT_UNWANTED_STRINGS_COUNT;

typedef struct UNWANTED_CONTAINERS {
    PARRAYSTR_UNWANTED_STRINGS m_paUnwantedHeads;
    PARRAYSTR_UNWANTED_STRINGS m_paUnwantedTails;
} UNWANTED_CONTAINERS;

typedef struct UNWANTED {
    PARRAYCSTR_UNWANTED_CHARACTERS m_paUnwantedCharacters;
    USHORT_UNWANTED_CHARACTERS_COUNT m_ushortUnwantedCharactersSize;

    PARRAYSTR_UNWANTED_STRINGS m_paUnwantedStrings;
    USHORT_UNWANTED_STRINGS_COUNT m_ushortUnwantedStringsSize;

    UNWANTED_CONTAINERS m_unwantedContainers;
} UNWANTED;

typedef enum UNWANTED_MODIFIER {
    ADD,
    DELETE,
    CLEAR
} UNWANTED_MODIFIER;

void ModifyUnwanted_CHAR(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, const char caNewCharacters[]);
void ModifyUnwanted_STRING(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, const char* szaNewStrings[], unsigned short count);
void ModifyUnwanted_CONTAINER(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char* szaNewHeads[], char* szaNewTails[]);

PSTRDATA GetData(PCSTRFILEPATH szFilePath);
void CleanUpData(PSTRDATA szData, UNWANTED unwanted);

#endif //FILELOADER_H
