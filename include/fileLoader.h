//
// Created by halet on 12/15/2025.
//

#ifndef FILELOADER_H
#define FILELOADER_H

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

#define HANDLE_FILE_ERROR(f) do { if (f) fclose(f); return NULL; } while(0)

typedef const char* PCSTRFILEPATH;
typedef const char** PARRAYCSTR_UNWANTED_CHARACTERS;
typedef const char** PARRAYCSTR_UNWANTED_STRINGS;

typedef struct UNWANTED_CONTAINERS {
    PARRAYCSTR_UNWANTED_CHARACTERS m_paUnwantedHeads;
    PARRAYCSTR_UNWANTED_CHARACTERS m_paUnwantedTails;
} UNWANTED_CONTAINERS;

typedef struct UNWANTED {
    PARRAYCSTR_UNWANTED_CHARACTERS m_paUnwantedCharacters;
    PARRAYCSTR_UNWANTED_STRINGS m_paUnwantedStrings;
    UNWANTED_CONTAINERS m_unwantedContainers;
} UNWANTED;

typedef enum UNWANTED_MODIFIER {
    ADD,
    DELETE,
    CLEAR
} UNWANTED_MODIFIER;

void ModifyUnwanted_CHAR(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char cNewCharacter);
void ModifyUnwanted_STRING(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char* szNewString);
void ModifyUnwanted_CONTAINER(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char* szNewHead, char* szNewTail);

PSTRDATA GetData(PCSTRFILEPATH szFilePath);
void CleanUpData(PCSTRDATA* szData, UNWANTED unwanted);

#endif //FILELOADER_H
