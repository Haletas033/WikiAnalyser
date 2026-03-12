//
// Created by halet on 3/12/2026.
//

#include "../include/topN.h"
#include <zlib.h>
#include "../external/uthash.h"

typedef struct {
    char name[256];
    int views;
    UT_hash_handle hh;
} ArticleViews;

void HandleTopNFile(const char* pageviews, const char* pageViewsName) {
    CurlDownloadTo(pageviews, "UserData/tmp/", pageViewsName);
    char* fullPath = malloc(strlen("UserData/tmp/")+strlen(pageViewsName)+1);
    strcpy(fullPath, "UserData/tmp/");
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

    ArticleViews* entry;
    HASH_FIND_STR(hashmap, "C++", entry);
    printf("%d\n", entry->views);

    gzclose(file);
    free(fullPath);
}

void CreateTopNFile(){
    const time_t t = time(NULL);
    const struct tm* tm = localtime(&t);

    const char* pageviewsRoot = "https://dumps.wikimedia.org/other/pageviews";
    char* pageViewsName = malloc(64);
    char* pageviews = malloc(strlen(pageviewsRoot)+64);

    //Loop over every day in the current year
    int i;
    for (i = 0; i < tm->tm_mon+1; i++) {
        sprintf(pageViewsName, "pageviews-%d%02d01-000000.gz", tm->tm_year + 1900, i+1);
        sprintf(pageviews, "%s/%d/%d-%02d/%s", pageviewsRoot, tm->tm_year + 1900, tm->tm_year + 1900, i+1, pageViewsName);
        HandleTopNFile(pageviews, pageViewsName);
    }

    free(pageviews);
}

