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
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MEGA_BYTES(m) m*1024*1024

typedef const char* PCSTRFILEPATH;

typedef const char* PARRAYCSTR_UNWANTED_CHARACTERS;
typedef unsigned short USHORT_UNWANTED_CHARACTERS_COUNT;

typedef char** PARRAYSTR_UNWANTED_STRINGS;
typedef unsigned short USHORT_UNWANTED_STRINGS_COUNT;

typedef struct UNWANTED_CONTAINERS {
    PARRAYSTR_UNWANTED_STRINGS m_paUnwantedHeads;
    USHORT_UNWANTED_STRINGS_COUNT m_ushortUnwantedHeadsSize;

    PARRAYSTR_UNWANTED_STRINGS m_paUnwantedTails;
    USHORT_UNWANTED_STRINGS_COUNT m_ushortUnwantedTailsSize;
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
void ModifyUnwanted_STRING(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char* szaNewStrings[], unsigned short count);
void ModifyUnwanted_CONTAINER(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char* szaNewHeads[], char* szaNewTails[], unsigned short ushortHeadCount,const unsigned short ushortTailCount);

void CleanUpCharacters(const UNWANTED* unwanted, bool* isUnwanted, const unsigned char* uszBuffer, ssize_t i);
void CleanUpStrings(const UNWANTED* unwanted, bool* isUnwanted, const unsigned char* uszBuffer, ssize_t* i);
void CleanUpContainers(const UNWANTED* unwanted, bool* isUnwanted, const unsigned char* uszBuffer, ssize_t* i);
int CleanUpData(PCSTRFILEPATH szFilePath, UNWANTED unwanted);

#endif //FILELOADER_H
