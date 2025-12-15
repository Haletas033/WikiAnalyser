#include "../include/fileLoader.h"

int main() {
    UNWANTED unwanted = {0};
    ModifyUnwanted_CHAR(&unwanted, ADD, "⌐├íW├╢");

    ModifyUnwanted_STRING(&unwanted, ADD, (const char*[]){"&lt", "&gt", "lf", "rt"}, 4);

    printf(unwanted.m_paUnwantedStrings[0]);

    PSTRDATA data = GetData("");
    CleanUpData(data, unwanted);
    printf("%s\n", data);
    free(data);

    return 0;
}
