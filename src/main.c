#include "../include/unwanted.h"
#include "../include/cleanup.h"
#include "../include/article.h"
#include "../include/parser.h"

int main() {
    UNWANTED unwanted = {0};
    ModifyUnwanted_CHAR(&unwanted, ADD, "!@#$%^&*()l1234567890 \n");
    ModifyUnwanted_CHAR(&unwanted, DELETE, "l");

    ModifyUnwanted_STRING(&unwanted, ADD, (char*[]){"foo", "bar", "ab"}, 3);
    ModifyUnwanted_STRING(&unwanted, DELETE, (char*[]){"ab"}, 1);

    ModifyUnwanted_CONTAINER(
        &unwanted, ADD,
        (char*[]){"<siteinfo>", "{{", "aa"},(char*[]){"</siteinfo>", "}}", "aa"},
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

    //Ignore (for testing on both linux and windows)
    #ifdef __WIN32__

    CleanUpData("C:/Users/halet/Dev/wikianalyserTest.xml", unwanted);

    ParseArticles("C:/Users/halet/Dev/wikianalyserTest.xml", &articles, &article, &articleCount);

    #endif

    #ifdef __linux__

    CleanUpData("/home/haletas/Dev/wikianalyserTest.xml", unwanted);

    ParseArticles("/home/haletas/Dev/wikianalyserTest.xml", &articles, &article, &articleCount);

    #endif

    int i;
    for (i = 0; i < articleCount; i++) {
        zigPrintTest(&articles[i]);
    }

    return 0;
}
