#include "../include/unwanted.h"
#include "../include/cleanup.h"
#include "../include/article.h"
#include "../include/parser.h"

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

    Article *articles;
    unsigned int articleCount = 0;

    Article article = {0};

    AddField(&article, INT, "e_Count");
    AddField(&article, FLOAT, "Float Test");
    AddField(&article, BOOL, "Bool Test");
    AddField(&article, STRING, "String Test");

    CleanUpData("C:\\Users\\halet\\PycharmProjects\\NCEA\\mini.xml", unwanted);

    ParseArticles("C:\\Users\\halet\\PycharmProjects\\NCEA\\mini.xml",  &articles, &article, &articleCount);

    int i;
    for (i = 0; i < articleCount; i++) {
        zigPrintTest(&articles[i]);
    }

    return 0;
}
