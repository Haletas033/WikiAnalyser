//
// Created by halet on 12/14/2025.
//

#include "../include/parser.h"

int ParseArticles(PCSTRFILEPATH szFilePath, Article article) {
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
            OnArticle(&uszBuffer[i], article);
        }
    }

    close(iFileDir);
    free(uszBuffer);
    return 0;
}
