#include "../include/fileLoader.h"

int main() {
    UNWANTED unwanted = {0};
    ModifyUnwanted_CHAR(&unwanted, ADD, "Ajvfriopqw");
    ModifyUnwanted_CHAR(&unwanted, DELETE, "A");

    ModifyUnwanted_STRING(&unwanted, ADD, (char*[]){"&lt", "&gt", "lf", "rt", "&amp", "ab"}, 6);
    ModifyUnwanted_STRING(&unwanted, DELETE, (char*[]){"ab"}, 1);

    ModifyUnwanted_CONTAINER(
        &unwanted, ADD,
        (char*[]){"<<", "{{", "aa"},(char*[]){">>", "}}", "aa"},
        3
    );

    ModifyUnwanted_CONTAINER(
        &unwanted, DELETE,
        (char*[]){"{{"},(char*[]){"}}"},
        1
    );

    CleanUpData("C:\\Users\\halet\\PycharmProjects\\NCEA\\mini.xml", unwanted);


    return 0;
}
