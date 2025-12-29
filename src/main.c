#include "../include/fileLoader.h"

int main() {
    UNWANTED unwanted = {0};
    ModifyUnwanted_CHAR(&unwanted, ADD, "ABCDEFGHIJKLMNOPQRSTUVWXYZacbedfghijklmnopqrstuvwxyz\n ");

    ModifyUnwanted_STRING(&unwanted, ADD, (char*[]){"&lt", "&gt", "lf", "rt", "&amp"}, 5);

    ModifyUnwanted_CONTAINER(
        &unwanted, ADD,
        (char*[]){"<<"},(char*[]){">>"},
        1
    );

    printf(unwanted.m_paUnwantedStrings[0]);

    printf(unwanted.m_unwantedContainers.m_paUnwantedTails[0]);

    CleanUpData("C:\\Users\\halet\\PycharmProjects\\NCEA\\mini.xml", unwanted);


    return 0;
}
