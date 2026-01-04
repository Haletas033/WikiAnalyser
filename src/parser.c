//
// Created by halet on 12/14/2025.
//

#include "../include/parser.h"

int ParseArticles(PCSTRFILEPATH szFilePath, Article **articles, Article *baseArticle, unsigned int *articleCount) {
    unsigned int nextPos = -1;

    int iFileDir = open(szFilePath, O_RDWR);
    if (iFileDir < 0) return -1;

    unsigned char* uszBuffer = malloc(MEGA_BYTES(32));
    if (!uszBuffer) {
        close(iFileDir);
        return -1;
    }
    while (true) {
        ssize_t llReadOffset = read(iFileDir, uszBuffer, MEGA_BYTES(32));
        if (llReadOffset <= 0) break;

        ssize_t i;
        for (i = 0; i < llReadOffset; i++) {
            if (strncmp((const char*)uszBuffer + i, "<page>", strlen("<page>")) == 0) {
                i+=6;
                nextPos++;
                (*articleCount)++;
                Article newArticle = {0};
                CopyFields(baseArticle, &newArticle);

                Article* tmp = realloc(*articles, (nextPos+1)*sizeof(Article));

                if (!tmp) {
                    break;
                }

                *articles = tmp;

                (*articles)[nextPos] = newArticle;
                //Create new article increment a next pos of some sorts
            }
            OnArticle(&uszBuffer[i], (*articles)[nextPos]);
        }
    }

    close(iFileDir);
    free(uszBuffer);
    return 0;
}
