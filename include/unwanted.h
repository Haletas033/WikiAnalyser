//
// Created by halet on 12/31/2025.
//

#ifndef UNWANTED_H
#define UNWANTED_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

typedef const char* PCSTRFILEPATH;

typedef char* PARRAYCSTR_UNWANTED_CHARACTERS;
typedef unsigned short USHORT_UNWANTED_CHARACTERS_COUNT;

typedef char** PARRAYSTR_UNWANTED_STRINGS;
typedef unsigned short USHORT_UNWANTED_STRINGS_COUNT;

typedef struct UNWANTED_CONTAINERS {
    PARRAYSTR_UNWANTED_STRINGS m_paUnwantedHeads;
    PARRAYSTR_UNWANTED_STRINGS m_paUnwantedTails;

    USHORT_UNWANTED_STRINGS_COUNT m_ushortUnwantedContainersSize;
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

void Add_CHAR(unsigned short* charactersSize, char* caCharacters[], const char caNewCharacters[]);
void Add_STRING(unsigned short* stringsSize, char** szaStrings[], char* szaNewStrings[], unsigned short count);

void ModifyUnwanted_CHAR(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, const char caNewCharacters[]);
void ModifyUnwanted_STRING(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char* szaNewStrings[], unsigned short count);
void ModifyUnwanted_CONTAINER(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char* szaNewHeads[], char* szaNewTails[], unsigned short ushortContainerCount);

#endif //UNWANTED_H
