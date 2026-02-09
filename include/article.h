//
// Created by halet on 12/14/2025.
//

#ifndef ARTICLE_H
#define ARTICLE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef char* STRTITLE;

typedef unsigned int UINTFIELDSIZE;
typedef char** PARRAYSTRFIELDNAMES;

typedef int* PARRAYINTFIELDS;
typedef float* PARRAYFLOATFIELDS;
typedef bool* PARRAYBOOLFIELDS;
typedef char** PARRAYSTRFIELDS;

enum Type {
    INT,
    FLOAT,
    STRING,
    BOOL
};

typedef struct Article {
    //Automatic fields
    STRTITLE title;

    //Custom fields
    PARRAYINTFIELDS intFields;
    PARRAYFLOATFIELDS floatFields;
    PARRAYBOOLFIELDS boolFields;
    PARRAYSTRFIELDS stringFields;

    UINTFIELDSIZE intFieldsSize;
    UINTFIELDSIZE floatFieldsSize;
    UINTFIELDSIZE boolFieldsSize;
    UINTFIELDSIZE stringFieldsSize;

    PARRAYSTRFIELDNAMES intFieldNames;
    PARRAYSTRFIELDNAMES floatFieldNames;
    PARRAYSTRFIELDNAMES boolFieldNames;
    PARRAYSTRFIELDNAMES stringFieldNames;


} Article;

void AddName(Article *article, enum Type type, const char* name);
void AddField(Article *article, enum Type type, const char* name);
void CopyFields(const Article *article, Article *newArticle);

//Zig functions ahead
void zigPrintTest(Article *article);


#endif //ARTICLE_H
