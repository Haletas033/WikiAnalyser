//
// Created by halet on 3/12/2026.
//

#ifndef TOPN_H
#define TOPN_H

#include "../include/curl.h"
#include "../external/uthash.h"

typedef struct {
    char name[256];
    int views;
    UT_hash_handle hh;
} ArticleViews;

void CreateTopNFile();
ArticleViews* LoadTopNFile(const char* path);
const char** GetTop(unsigned int n, ArticleViews* hashmap);

#endif //TOPN_H
