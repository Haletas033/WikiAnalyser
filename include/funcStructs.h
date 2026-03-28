//
// Created by halet on 3/22/2026.
//

#ifndef FUNCSTRUCTS_H
#define FUNCSTRUCTS_H
#include "core/cleanup.h"

typedef struct DownloadToStruct {
    const char* url;
    const char* filePath;
    const char* fileName;
} DownloadToStruct;

typedef struct DownloadSpecialExportToStruct {
    const char** name;
    unsigned int count;
    const char* filePath;
    const char* fileName;
} DownloadSpecialExportToStruct;

typedef struct ParseStruct {
    PCSTRFILEPATH szFilePath;
    Article **articles;
    Article *baseArticle;
    unsigned int *articleCount;
} ParseStruct;

typedef struct SystemStruct {
    const char* cmd;
    void(*func)(void);
} SystemStruct;

#endif //FUNCSTRUCTS_H
