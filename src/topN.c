//
// Created by halet on 3/12/2026.
//

#include "../include/topN.h"
#include <zlib.h>
#include "../include/core/article.h"

unsigned int SkipNoise(const char* name) {
    return (
        strncmp(name, "Talk:", 5) == 0 ||
        strncmp(name, "File:", 5) == 0 ||
        strncmp(name, "Help:", 5) == 0 ||
        strncmp(name, "Portal:", 7) == 0 ||
        strncmp(name, "Special:", 8) == 0 ||
        strncmp(name, "Template:", 9) == 0 ||
        strncmp(name, "Wikipedia:", 10) == 0 ||
        strcmp(name, "-") == 0
    );
}

ArticleViews* HandleTopNFile(const char* pageviews, const char* pageViewsName) {
    CurlDownloadTo(pageviews, "SystemData/tmp/", pageViewsName);
    char* fullPath = malloc(strlen("SystemData/tmp/")+strlen(pageViewsName)+1);
    strcpy(fullPath, "SystemData/tmp/");
    strcat(fullPath, pageViewsName);
    gzFile file = gzopen(fullPath, "rb");

    ArticleViews* hashmap = NULL;

    while (1) {
        char line[512];

        if (gzgets(file, line, 512) == NULL)
            break;

        if (strncmp(line, "en ", 3) != 0)
            continue;

        //Add name and value to hashmap
        strtok(line, " ");
        const char* name = strtok(NULL, " ");
        int views = atoi(strtok(NULL, " "));

        if (SkipNoise(name)) continue;

        ArticleViews* entry;
        HASH_FIND_STR(hashmap, name, entry);
        if (entry == NULL) {
            entry = malloc(sizeof(ArticleViews));
            strcpy(entry->name, name);
            entry->views = views;
            HASH_ADD_STR(hashmap, name, entry);
        } else {
            entry->views += views;
        }
    }

    gzclose(file);

    remove(fullPath);

    free(fullPath);

    return hashmap;
}

int cmp(ArticleViews* a, ArticleViews* b) {
    return b->views - a->views;
}

ArticleViews* MergeAndSortHashmaps(ArticleViews* topN[12], unsigned int months) {
    ArticleViews* master = NULL;

    int i;
    for (i = 0; i < months; i++) {
        ArticleViews *entry, *tmp;
        HASH_ITER(hh, topN[i], entry, tmp) {
            ArticleViews* existing;
            HASH_FIND_STR(master, entry->name, existing);
            if (existing == NULL) {
                ArticleViews* newEntry = malloc(sizeof(ArticleViews));
                strcpy(newEntry->name, entry->name);
                newEntry->views = entry->views;
                HASH_ADD_STR(master, name, newEntry);
            } else {
                existing->views += entry->views;
            }
        }
    }

    HASH_SORT(master, cmp);

    return master;
}

void CreateTopNFile(){
    const time_t t = time(NULL);
    const struct tm* tm = localtime(&t);

    const char* pageviewsRoot = "https://dumps.wikimedia.org/other/pageviews";
    char* pageviewsName = malloc(64);
    char* pageviews = malloc(strlen(pageviewsRoot)+64);

    ArticleViews* topN[12];

    //Loop over every month in the current year
    int i;
    for (i = 0; i < tm->tm_mon+1; i++) {
        sprintf(pageviewsName, "pageviews-%d%02d01-000000.gz", tm->tm_year + 1900, i+1);
        sprintf(pageviews, "%s/%d/%d-%02d/%s", pageviewsRoot, tm->tm_year + 1900, tm->tm_year + 1900, i+1, pageviewsName);
        topN[i] = HandleTopNFile(pageviews, pageviewsName);
    }

    ArticleViews* master = MergeAndSortHashmaps(topN, tm->tm_mon+1);
    for (i = 0; i < tm->tm_mon+1; i++) {
         ArticleViews *entry, *tmp;
         HASH_ITER(hh, topN[i], entry, tmp) {
             HASH_DEL(topN[i], entry);
             free(entry);
         }
    }

    free(pageviews);
    free(pageviewsName);

    //Write to binary file
    FILE* topNFile = fopen("SystemData/topN.topn", "wb");

    ArticleViews *entry, *tmp;
    HASH_ITER(hh, master, entry, tmp) {
        fwrite(entry, sizeof(ArticleViews), 1, topNFile);
    }

    fclose(topNFile);
}

ArticleViews* LoadTopNFile(const char* path) {
    FILE* topNFile = fopen(path, "rb");
    if (topNFile == NULL)
        return NULL;


    ArticleViews* hashmap = NULL;
    ArticleViews* article = malloc(sizeof(ArticleViews));
    while (fread(article, sizeof(ArticleViews),1, topNFile) == 1) {
        HASH_ADD_STR(hashmap, name, article);
        article = malloc(sizeof(ArticleViews));
    }

    free(article);
    fclose(topNFile);
    return hashmap;
}

const char** GetTop(const unsigned int n, ArticleViews* hashmap) {
    char** topN = malloc(sizeof(char*)*n);

    unsigned int counter = 0;
    ArticleViews *entry, *tmp;
    HASH_ITER(hh, hashmap, entry, tmp) {
        if (counter == n) break;
        topN[counter] = entry->name;
        counter++;
    }

    return topN;
}