//
// Created by halet on 12/15/2025.
//

//
// Created by halet on 12/14/2025.
//

#include "../include/fileLoader.h"

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

            int c;
            for (c = 0; c < unwanted.m_ushortUnwantedCharactersSize; c++) {
                if (uszBuffer[i] == unwanted.m_paUnwantedCharacters[c]) {
                    isUnwanted = true;
                    break;
                }
            }

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