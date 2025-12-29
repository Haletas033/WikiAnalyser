//
// Created by halet on 12/15/2025.
//

//
// Created by halet on 12/14/2025.
//

#include "../include/fileLoader.h"

//Handles character logic not unwantedCharacters only
void Modify_CHAR(unsigned short* charactersSize, char* caCharacters[], const char caNewCharacters[]) {
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
void Modify_STRING(unsigned short* stringsSize, char** szaStrings[], char* szaNewStrings[], unsigned short count) {
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

void ModifyUnwanted_CHAR(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, const char caNewCharacters[]) {
    switch (iModifier) {
        case ADD: {
            Modify_CHAR(&unwanted->m_ushortUnwantedCharactersSize, &unwanted->m_paUnwantedCharacters, caNewCharacters);
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
            Modify_STRING(&unwanted->m_ushortUnwantedStringsSize, &unwanted->m_paUnwantedStrings , szaNewStrings, count);
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
    const unsigned short ushortHeadCount,const unsigned short ushortTailCount)
{
    if (ushortHeadCount == 0 || ushortTailCount == 0 && iModifier != CLEAR) return;

    switch (iModifier) {
        case ADD: {
            //Handle heads
            Modify_STRING(
                &unwanted->m_unwantedContainers.m_ushortUnwantedHeadsSize,
                &unwanted->m_unwantedContainers.m_paUnwantedHeads,
                szaNewHeads, ushortHeadCount
            );

            //Handle tails
            Modify_STRING(
                &unwanted->m_unwantedContainers.m_ushortUnwantedTailsSize,
                &unwanted->m_unwantedContainers.m_paUnwantedTails,
                szaNewTails, ushortTailCount
            );
            break;
        }
        case CLEAR: {
            //Clear heads
            unwanted->m_unwantedContainers.m_ushortUnwantedHeadsSize = 0;
            free(unwanted->m_unwantedContainers.m_paUnwantedHeads);
            unwanted->m_unwantedContainers.m_paUnwantedHeads = NULL;

            //Clear tails
            unwanted->m_unwantedContainers.m_ushortUnwantedTailsSize = 0;
            free(unwanted->m_unwantedContainers.m_paUnwantedTails);
            unwanted->m_unwantedContainers.m_paUnwantedTails = NULL;
            break;
        }
        default: break;
    }
}

void CleanUpCharacters(const UNWANTED* unwanted, bool* isUnwanted, const unsigned char* uszBuffer, ssize_t i) {
    int c;
    for (c = 0; c < unwanted->m_ushortUnwantedCharactersSize; c++) {
        if (uszBuffer[i] == unwanted->m_paUnwantedCharacters[c]) {
            *isUnwanted = true;
            break;
        }
    }
}

void CleanUpStrings(const UNWANTED* unwanted, bool* isUnwanted, const unsigned char* uszBuffer, ssize_t* i) {
    int sz;
    for (sz = 0; sz < unwanted->m_ushortUnwantedStringsSize; sz++) {
        const char* s = unwanted->m_paUnwantedStrings[sz];
        size_t ullLen = strlen(unwanted->m_paUnwantedStrings[sz]);

        if (strncmp((const char*)(uszBuffer + *i), s, ullLen) == 0) {
            *isUnwanted = true;
            *i+=ullLen;
            break;
        }
    }
}

void CleanUpContainers(const UNWANTED* unwanted, bool* isUnwanted, const unsigned char* uszBuffer, ssize_t* i) {
    int sz;

    //Check if we are on a head
    size_t ullLen = 0;
    for (sz = 0; sz < unwanted->m_unwantedContainers.m_ushortUnwantedHeadsSize; sz++) {
        const char* s = unwanted->m_unwantedContainers.m_paUnwantedHeads[sz];
         ullLen = strlen(unwanted->m_unwantedContainers.m_paUnwantedHeads[sz]);

        if (strncmp((const char*)(uszBuffer + *i), s, ullLen) == 0) {
            *isUnwanted = true;
            *i+=ullLen;
            break;
        }
    }

    //Scan until corresponding tail is found

    //Run if a head is found
    if (sz != unwanted->m_unwantedContainers.m_ushortUnwantedHeadsSize) {
        while (strncmp((const char*)(uszBuffer + *i), unwanted->m_unwantedContainers.m_paUnwantedTails[sz], ullLen) != 0) {
            (*i)++;

            //Stop after 32mb to prevent crashes if tail isn't found
            if (*i + ullLen > MEGA_BYTES(32)) {
                return;
            }
        }
    }
    *i+=ullLen;
}

int CleanUpData(PCSTRFILEPATH szFilePath, const UNWANTED unwanted) {
    int iFileDir = open(szFilePath, O_RDWR);
    if (iFileDir < 0) return -1;

    unsigned char* uszBuffer = malloc(MEGA_BYTES(32));
    if (!uszBuffer) {
        close(iFileDir);
        return -1;
    }

    off_t lWritePos = 0;
    off_t lReadPos = 0;


    while (true) {
        if (lseek(iFileDir, lReadPos, SEEK_SET) == (off_t)-1) break;

        ssize_t llReadOffset = read(iFileDir, uszBuffer, MEGA_BYTES(32));
        if (llReadOffset <= 0) break;

        size_t ullWriteOffset = 0;


        ssize_t i;
        for (i = 0; i < llReadOffset; i++) {
            bool isUnwanted = false;

            CleanUpContainers(&unwanted, &isUnwanted, uszBuffer, &i);
            CleanUpStrings(&unwanted, &isUnwanted, uszBuffer, &i);
            CleanUpCharacters(&unwanted, &isUnwanted, uszBuffer, i);

            if (!isUnwanted) {
                uszBuffer[ullWriteOffset++] = uszBuffer[i];
            }
        }

        if (lseek(iFileDir, lWritePos, SEEK_SET) == (off_t)-1) break;

        if (write(iFileDir, uszBuffer, ullWriteOffset) != (ssize_t)ullWriteOffset) break;

        lReadPos += llReadOffset;
        lWritePos += ullWriteOffset;
    }

    if (ftruncate(iFileDir, lWritePos) != 0) {
        close(iFileDir);
        free(uszBuffer);
        return -1;
    }

    close(iFileDir);
    free(uszBuffer);
    return 0;
}