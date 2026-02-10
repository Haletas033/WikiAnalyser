//
// Created by halet on 12/15/2025.
//

#ifndef FILELOADER_H
#define FILELOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "articleParser.h"
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "unwanted.h"

#define MEGA_BYTES(m) m*1024*1024

typedef const char* PCSTRFILEPATH;

void CleanUpCharacters(const UNWANTED* unwanted, bool* isUnwanted, const unsigned char* uszBuffer, ssize_t i);
void CleanUpStrings(const UNWANTED* unwanted, bool* isUnwanted, const unsigned char* uszBuffer, ssize_t* i);
void CleanUpContainers(const UNWANTED* unwanted, bool* isUnwanted, const unsigned char* uszBuffer, ssize_t* i);
int CleanUpData(PCSTRFILEPATH szFilePath, UNWANTED unwanted);

#endif //FILELOADER_H
