#include "../include/fileLoader.h"

int main() {
    UNWANTED unwanted = {0};
    ModifyUnwanted_CHAR(&unwanted, ADD, "ABCDEFGHIJKLMNOPQRSTUVWXYZacbedfghijklmnopqrstuvwxyz\n ");

    ModifyUnwanted_STRING(&unwanted, ADD, (const char*[]){"&lt", "&gt", "lf", "rt", "&amp"}, 5);

    printf(unwanted.m_paUnwantedStrings[0]);

    CleanUpData("", unwanted);


    return 0;
}
