//
// Created by halet on 12/15/2025.
//

//
// Created by halet on 12/14/2025.
//

#include "../include/fileLoader.h"

PSTRDATA GetData(PCSTRFILEPATH szFilePath) {
    FILE* pFile = fopen(szFilePath, "r");
    if (!pFile) HANDLE_FILE_ERROR(NULL);

    if (fseek(pFile, 0, SEEK_END) != 0) HANDLE_FILE_ERROR(pFile);

    const long iFileSize = ftell(pFile);
    if (iFileSize < 0) HANDLE_FILE_ERROR(pFile);

    rewind(pFile);

    char* szBuffer = malloc(iFileSize + 1);
    if (!szBuffer) HANDLE_FILE_ERROR(pFile);

    const size_t uBytes = fread(szBuffer, 1, iFileSize, pFile);
    if (uBytes != (size_t)iFileSize) {
        free(szBuffer);
        return NULL;
    }

    fclose(pFile);
    szBuffer[iFileSize] = '\0';

    return szBuffer;
}

void ModifyUnwanted_CHAR(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, const char caNewCharacters[]) {
    const unsigned short uOldSize = unwanted->m_ushortUnwantedCharactersSize;
    const unsigned short uNewSize = strlen(caNewCharacters) + 1;
    const unsigned short uSize = uOldSize + uNewSize;

    char* buffer = malloc(uSize * sizeof(char));
    if (!buffer) return;

    memcpy(buffer, unwanted->m_paUnwantedCharacters, uOldSize);
    memcpy(buffer + uOldSize, caNewCharacters, uNewSize);

    free((char*)unwanted->m_paUnwantedCharacters);
    unwanted->m_paUnwantedCharacters = buffer;
    unwanted->m_ushortUnwantedCharactersSize = uSize;
}

void ModifyUnwanted_STRING(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, const char* szaNewStrings[], const unsigned short count) {
    if (count == 0) return;

    const unsigned short uOldSize = unwanted->m_ushortUnwantedStringsSize;
    const unsigned short uSize = uOldSize + count;

    char** buffer = malloc(uSize * sizeof(char*));
    if (!buffer) return;

    unsigned short i;
    for (i = 0; i < uOldSize; i++) {
        buffer[i] = unwanted->m_paUnwantedStrings[i];
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

    free(unwanted->m_paUnwantedStrings);
    unwanted->m_paUnwantedStrings = buffer;
    unwanted->m_ushortUnwantedStringsSize = uSize;
}

void ModifyUnwanted_CONTAINER(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char* szaNewHeads[], char* szaNewTails[]) {

}

void CleanUpData(PSTRDATA szData, const UNWANTED unwanted) {
    const char* src = szData;
    char* dst = szData;

    while (*src != '\0') {
        bool isUnwanted = false;

        int c;
        for (c = 0; c < unwanted.m_ushortUnwantedCharactersSize; c++) {
            if (*src == unwanted.m_paUnwantedCharacters[c]) {
                isUnwanted = true;
                break;
            }
        }

        if (!isUnwanted) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
}