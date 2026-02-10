#include "../include/core/article.h"

void AddName(Article *article, enum Type type, const char* name) {
    char*** fieldNameType = NULL;
    unsigned int* fieldSize = 0;

    switch (type) {
        case INT: {
            fieldNameType = &article->intFieldNames;
            fieldSize = &article->intFieldsSize;
            break;
        }
        case FLOAT: {
            fieldNameType = &article->floatFieldNames;
            fieldSize = &article->floatFieldsSize;
            break;
        }
        case BOOL: {
            fieldNameType = &article->boolFieldNames;
            fieldSize = &article->boolFieldsSize;
            break;
        }
        case STRING: {
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
        case INT: {
            fieldType = (void**)&article->intFields;
            fieldSize = &article->intFieldsSize;
            elementSize = sizeof(int);
            break;
        }
        case FLOAT: {
            fieldType = (void**)&article->floatFields;
            fieldSize = &article->floatFieldsSize;
            elementSize = sizeof(float);
            break;
        }
        case BOOL: {
            fieldType = (void**)&article->boolFields;
            fieldSize = &article->boolFieldsSize;
            elementSize = sizeof(bool);
            break;
        }
        case STRING: {
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

    if (type == STRING) {
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
    for (i = 0; i < article->intFieldsSize; i++) AddField(newArticle, INT, article->intFieldNames[i]);
    for (i = 0; i < article->floatFieldsSize; i++) AddField(newArticle, FLOAT, article->floatFieldNames[i]);
    for (i = 0; i < article->boolFieldsSize; i++) AddField(newArticle, BOOL, article->boolFieldNames[i]);
    for (i = 0; i < article->stringFieldsSize; i++) AddField(newArticle, STRING, article->stringFieldNames[i]);
}

