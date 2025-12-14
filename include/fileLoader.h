//
// Created by halet on 12/15/2025.
//

#ifndef FILELOADER_H
#define FILELOADER_H

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "parser.h"

#define HANDLE_FILE_ERROR(f) do { if (f) fclose(f); return NULL; } while(0)

typedef const char* PCSTRFILEPATH;

PCSTRDATA GetData(PCSTRFILEPATH szFilePath);


#endif //FILELOADER_H
