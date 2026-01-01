#include "../include/unwanted.h"
#include "../include/cleanup.h"
#include "../include/article.h"

int main() {
    UNWANTED unwanted = {0};
    ModifyUnwanted_CHAR(&unwanted, ADD, "!@#$%^&*()l1234567890\n");
    ModifyUnwanted_CHAR(&unwanted, DELETE, "l");

    ModifyUnwanted_STRING(&unwanted, ADD, (char*[]){"foo", "bar", "ab"}, 3);
    ModifyUnwanted_STRING(&unwanted, DELETE, (char*[]){"ab"}, 1);

    ModifyUnwanted_CONTAINER(
        &unwanted, ADD,
        (char*[]){"<<", "{{", "aa"},(char*[]){">>", "}}", "aa"},
        3
    );

    ModifyUnwanted_CONTAINER(
        &unwanted, DELETE,
        (char*[]){"aa"},(char*[]){"aa"},
        1
    );

    Article article = {0};

    AddField(&article, INT, "Int Test");
    AddField(&article, FLOAT, "Float Test");
    AddField(&article, BOOL, "Bool Test");
    AddField(&article, STRING, "String Test");

    printf("%d\n", article.intFields[0]);
    printf("%s\n", article.intFieldNames[0]);
    printf("%f\n", article.floatFields[0]);
    printf("%s\n", article.floatFieldNames[0]);
    printf("%d\n", article.boolFields[0]);
    printf("%s\n", article.boolFieldNames[0]);
    printf("\"%s\"\n", article.stringFields[0]);
    printf("%s\n", article.stringFieldNames[0]);


    CleanUpData("C:\\Users\\halet\\PycharmProjects\\NCEA\\mini.xml", unwanted);

    return 0;
}
