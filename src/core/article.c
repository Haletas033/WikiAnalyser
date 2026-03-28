#include "../include/core/article.h"

#include <stdio.h>

void AddName(Article *article, enum Type type, const char* name) {
    char*** fieldNameType = NULL;
    unsigned int* fieldSize = 0;

    switch (type) {
        case FIELD_INT: {
            fieldNameType = &article->intFieldNames;
            fieldSize = &article->intFieldsSize;
            break;
        }
        case FIELD_FLOAT: {
            fieldNameType = &article->floatFieldNames;
            fieldSize = &article->floatFieldsSize;
            break;
        }
        case FIELD_BOOL: {
            fieldNameType = &article->boolFieldNames;
            fieldSize = &article->boolFieldsSize;
            break;
        }
        case FIELD_STRING: {
            fieldNameType = &article->stringFieldNames;
            fieldSize = &article->stringFieldsSize;
            break;
        }
        default: return;
    }

    char** tmp = realloc(*fieldNameType, (*fieldSize + 1) * sizeof(char*));

    if (!tmp) return;

    *fieldNameType = tmp;

    (*fieldNameType)[*fieldSize] = strdup(name);
}

void AddField(Article *article, enum Type type, const char* name) {
    void** fieldType = NULL;
    unsigned int* fieldSize = 0;
    ssize_t elementSize = 0;

    switch (type) {
        case FIELD_INT: {
            fieldType = (void**)&article->intFields;
            fieldSize = &article->intFieldsSize;
            elementSize = sizeof(int);
            break;
        }
        case FIELD_FLOAT: {
            fieldType = (void**)&article->floatFields;
            fieldSize = &article->floatFieldsSize;
            elementSize = sizeof(float);
            break;
        }
        case FIELD_BOOL: {
            fieldType = (void**)&article->boolFields;
            fieldSize = &article->boolFieldsSize;
            elementSize = sizeof(bool);
            break;
        }
        case FIELD_STRING: {
            fieldType = (void**)&article->stringFields;
            fieldSize = &article->stringFieldsSize;
            elementSize = sizeof(char*);
            break;
        }
        default: return;
    }

    void* tmp = realloc(*fieldType, (*fieldSize + 1) * elementSize);

    if (!tmp) return;

    *fieldType = tmp;

    if (type == FIELD_STRING) {
        ((char**)*fieldType)[*fieldSize] = NULL;
    } else {
        memset((char*)*fieldType + (*fieldSize * elementSize), 0, elementSize);
    }

    //Assign name then increment
    AddName(article, type, name);

    (*fieldSize)++;
}

void CopyFields(const Article *article, Article *newArticle) {
    int i;
    for (i = 0; i < article->intFieldsSize; i++) AddField(newArticle, FIELD_INT, article->intFieldNames[i]);
    for (i = 0; i < article->floatFieldsSize; i++) AddField(newArticle, FIELD_FLOAT, article->floatFieldNames[i]);
    for (i = 0; i < article->boolFieldsSize; i++) AddField(newArticle, FIELD_BOOL, article->boolFieldNames[i]);
    for (i = 0; i < article->stringFieldsSize; i++) AddField(newArticle, FIELD_STRING, article->stringFieldNames[i]);
}

Article FindArticleByName(const char* name, const Article* articles, const unsigned int articleCount) {
    int i;
    printf("i%s\n", name);
    for (i = 0; i < articleCount; i++) {
        printf("ii%s\n", articles[i].title);
        if (strcmp(articles[i].title, name) == 0) return articles[i];
    }
    return (Article){0};
}

Article SumArticles(const Article baseArticle, const Article* articles, const unsigned int articleCount) {
    Article article = baseArticle;
    int i;
    for (i = 0; i < articleCount; i++) {
        int j;
        for (j = 0; j < baseArticle.intFieldsSize; j++)
            article.intFields[j] += articles[i].intFields[j];
        for (j = 0; j < baseArticle.floatFieldsSize; j++)
            article.floatFields[j] += articles[i].floatFields[j];
    }
    article.title = "Summed Articles";
    return article;
}

Article AverageArticles(const Article baseArticle, const Article* articles, const unsigned int articleCount) {
    Article article = SumArticles(baseArticle, articles, articleCount);
    int j;
    for (j = 0; j < baseArticle.intFieldsSize; j++)
        article.intFields[j] /= articleCount;
    for (j = 0; j < baseArticle.floatFieldsSize; j++)
        article.floatFields[j] /= articleCount;
    article.title = "Averaged Articles";
    return article;
}

