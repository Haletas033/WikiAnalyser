//
// Created by halet on 12/14/2025.
//

#ifndef PARSER_H
#define PARSER_H

typedef const char* PCSTRDATA;
typedef char* PSTRDATA;

typedef struct Parser {
    PCSTRDATA m_szData;
} Parser;

void InitParser(Parser* pParser, PCSTRDATA szData);


#endif //PARSER_H
