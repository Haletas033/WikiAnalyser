#include "../include/fileLoader.h"

int main() {
    UNWANTED unwanted;
    ModifyUnwanted_CHAR(&unwanted, ADD, "aeiou");

    PSTRDATA data = GetData("");
    CleanUpData(data, unwanted);
    printf("%s\n", data);
    free(data);

    return 0;
}
