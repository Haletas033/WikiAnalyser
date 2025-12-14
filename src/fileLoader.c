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
void ModifyUnwanted_CHAR(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char cNewCharacter) {

}

void ModifyUnwanted_STRING(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char* szNewString) {

}

void ModifyUnwanted_CONTAINER(UNWANTED* unwanted, UNWANTED_MODIFIER iModifier, char* szNewHead, char* szNewTail) {

}

void CleanUpData(PSTRDATA szData, UNWANTED unwanted) {

}