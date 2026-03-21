
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
#include "../include/welcomeGUI.h"
#include <curl/curl.h>

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

    //Create user data
    OSCreateDirectory("UserData");
    OSCreateDirectory("UserData/Projects");
    OSCreateDirectory("SystemData");
    OSCreateDirectory("SystemData/tmp");

    unsigned int shouldOpen = 0;
    FILE* data = fopen("UserData/data.ini", "r");
    if (data == NULL) shouldOpen = 1;
    if (data != NULL) {
        fseek(data, 0, SEEK_END);
        if (ftell(data) == 0) shouldOpen = 1;
        fclose(data);
    }
    if (shouldOpen) {
        data = fopen("UserData/data.ini", "w");
        fprintf(data, dataINI);
        fclose(data);
    }


    Window window = {0,0,100,100};


    OSCreateWindowClass();
    OSCreateWindow(&window);

    curl_global_init(CURL_GLOBAL_ALL);

    GUIStart(&window);

    OSMessageLoop();

    curl_global_cleanup();

    return 0;
}

