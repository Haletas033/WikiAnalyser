//
// Created by halet on 12/14/2025.
//

#ifndef ARTICLE_H
#define ARTICLE_H

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

#define HANDLE_FILE_ERROR(f) do { if (f) fclose(f); return NULL; } while(0)

typedef const char* PCSTRFILEPATH;

typedef struct Article {

} Article;

PCSTRDATA GetData(PCSTRFILEPATH szFilePath);


#endif //ARTICLE_H
