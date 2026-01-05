//
// Created by halet on 12/15/2025.
//

#include "../include/cleanup.h"



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
            *i+=ullLen-1;
            break;
        }
    }
}

void CleanUpContainers(const UNWANTED* unwanted, bool* isUnwanted, const unsigned char* uszBuffer, ssize_t* i) {
    int sz;

    //Check if we are on a head
    size_t ullLen = 0;
    for (sz = 0; sz < unwanted->m_unwantedContainers.m_ushortUnwantedContainersSize; sz++) {
        const char* s = unwanted->m_unwantedContainers.m_paUnwantedHeads[sz];
         ullLen = strlen(unwanted->m_unwantedContainers.m_paUnwantedHeads[sz]);

        if (strncmp((const char*)(uszBuffer + *i), s, ullLen) == 0) {
            *isUnwanted = true;
            *i+=ullLen-1;
            break;
        }
    }

    //Scan until corresponding tail is found

    //Run if a head is found
    if (sz != unwanted->m_unwantedContainers.m_ushortUnwantedContainersSize) {
        //Switch ullLen to the tail length
        ullLen = strlen(unwanted->m_unwantedContainers.m_paUnwantedTails[sz]);
        while (strncmp((const char*)(uszBuffer + *i), unwanted->m_unwantedContainers.m_paUnwantedTails[sz], ullLen) != 0) {
            (*i)++;

            //Stop after 32mb to prevent crashes if tail isn't found
            if (*i + ullLen > MEGA_BYTES(32)) {
                return;
            }
        }
        *i+=ullLen-1;
    }

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