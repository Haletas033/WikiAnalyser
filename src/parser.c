//
// Created by halet on 12/14/2025.
//

#include "../include/parser.h"

char* getAutomaticField(const unsigned char* uszBuffer, ssize_t* i, const char* fieldStart, const char* fieldEnd) {
    if (strncmp((const char*)uszBuffer + *i, fieldStart, strlen(fieldStart)) == 0) {
        *i+=strlen(fieldStart);
        //Read until </title>
        const int iStartIndex = *i;
        const size_t llBufferLen = strlen((const char*)uszBuffer);
        while (*i < llBufferLen && strncmp((const char*)uszBuffer + *i, fieldEnd, strlen(fieldEnd)) != 0) {
            (*i)++;
        }


        const int iTitleLen = *i - iStartIndex;
        char* slice = malloc(iTitleLen + 1);

        *i+=strlen(fieldEnd); //Skip over the endPart

        if (!slice) {
            return "Failed to get title slice";
        }

        strncpy(slice, (const char*)uszBuffer + iStartIndex, iTitleLen);
        slice[iTitleLen] = '\0';

        return slice;
    }

    return NULL;
}

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
            //Get new page
            if (strncmp((const char*)uszBuffer + i, "<page>", strlen("<page>")) == 0) {
                i+=strlen("<page>");
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
            }

            //Switch article on new page
            if (nextPos != -1) {
                OnArticle(&uszBuffer[i], (*articles)[nextPos]);
            }

            //Fill in title field
            char* title = getAutomaticField(uszBuffer, &i, "<title>", "</title>");
            if (title != NULL) {
                (*articles)[nextPos].title = title;
            }
        }
    }

    close(iFileDir);
    free(uszBuffer);
    return 0;
}
