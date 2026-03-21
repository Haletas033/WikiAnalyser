//
// Created by halet on 12/14/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "article.h"

#ifndef PARSER_H
#define PARSER_H

#define MEGA_BYTES(m) m*1024*1024

typedef const char* PCSTRFILEPATH;
typedef const char* PCSTRDATA;
typedef char* PSTRDATA;

const char** parseCommaSeperated(const char* inputText);

int ParseArticles(PCSTRFILEPATH szFilePath, Article **articles, Article *baseArticle, unsigned int *articleCount);
//Zig functions ahead
typedef void (*OnArticleFn)(unsigned char* currentChar, Article article);
extern OnArticleFn OnArticle;

#endif //PARSER_H
