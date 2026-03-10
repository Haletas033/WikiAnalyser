//
// Created by halet on 10.03.26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* dataINI = "DumpPath=\nZigPath=\n";

char* SetValueOfKey(char* line, const char* key, char* value) {
    char* tmpLine = strdup(line);
    char* result  = strtok(tmpLine, "=");
    if (strcmp(key, result) == 0) {
        char* newLine = malloc(strlen(key) + strlen(value) + 2);
        sprintf(newLine, "%s=%s", key, value);
        free(tmpLine);
        free(line);
        return newLine;
    }
    free(tmpLine);
    return line;
}

const char* GetValueOfKey(char* line, const char* key) {
    if (strcmp(strtok(line, "="), key) == 0)
        return strdup(strtok(NULL, "="));
    return NULL;
}

const char* GetINIField(const char* iniFile, const char* key) {
    FILE* file = fopen(iniFile, "r");

    char c;
    char buff[512];
    unsigned int buffPos = 0;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            buff[buffPos] = '\0';
            const char* value = GetValueOfKey(buff, key);
            if (value != NULL) {
                fclose(file);
                return value;
            }
            buffPos = 0;
        } else {
            buff[buffPos] = c;
            buffPos++;
        }
    }
    fclose(file);
    return NULL;
}

void SetINIField(const char* iniFile, const char* key, char* value) {
    FILE* file = fopen(iniFile, "r");

    char* lines[256];

    char c;
    char buff[512];
    unsigned int buffPos = 0;
    unsigned int linePos = 0;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            buff[buffPos] = '\0';
            lines[linePos] = SetValueOfKey(strdup(buff), key, value);
            linePos++;
            buffPos = 0;
        } else {
            buff[buffPos] = c;
            buffPos++;
        }
    }

    fclose(file);

    file = fopen(iniFile, "w");

    int i;
    for (i = 0; i < linePos; i++)
        fprintf(file, "%s\n", lines[i]);

    for (i = 0; i < linePos; i++)
        free(lines[i]);

    fclose(file);

}
