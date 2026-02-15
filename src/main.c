
#include "../include/core/unwanted.h"
#include "../include/core/cleanup.h"
#include "../include/core/article.h"
#include "../include/core/articleParser.h"

#ifdef __WIN32__
#include "../include/WINGUI.h"
#else
#include "../include/LINUXGUI.h"
#endif
#include "../include/GUI.h"

int main() {
    UNWANTED unwanted = {0};
    ModifyUnwanted_CHAR(&unwanted, UNWANTED_ADD, "!@#$%^&*()l1234567890 \n");
    ModifyUnwanted_CHAR(&unwanted, UNWANTED_DELETE, "l");

    ModifyUnwanted_STRING(&unwanted, UNWANTED_ADD, (char*[]){"foo", "bar", "ab"}, 3);
    ModifyUnwanted_STRING(&unwanted, UNWANTED_DELETE, (char*[]){"ab"}, 1);

    ModifyUnwanted_CONTAINER(
        &unwanted, UNWANTED_ADD,
        (char*[]){"<siteinfo>", "{{", "aa"},(char*[]){"</siteinfo>", "}}", "aa"},
        3
    );

    ModifyUnwanted_CONTAINER(
        &unwanted, UNWANTED_DELETE,
        (char*[]){"aa"},(char*[]){"aa"},
        1
    );

    Article *articles = NULL;
    unsigned int articleCount = 0;

    Article article = {0};
    AddField(&article, FIELD_INT, "e_Count");
    AddField(&article, FIELD_FLOAT, "Float Test");
    AddField(&article, FIELD_BOOL, "Bool Test");
    AddField(&article, FIELD_STRING, "String Test");

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

    OSCreateWindowClass();
    OSCreateWindow();
    GUIStart();
    OSMessageLoop();

    return 0;
}

