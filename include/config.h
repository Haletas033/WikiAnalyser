//
// Created by halet on 10.03.26.
//

#ifndef CONFIG_H
#define CONFIG_H

extern const char* dataINI;

const char* GetINIField(const char* iniFile, const char* key);
void SetINIField(const char* iniFile, const char* key, char* value);


#endif //CONFIG_H
