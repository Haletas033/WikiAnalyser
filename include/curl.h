//
// Created by halet on 3/11/2026.
//

#ifndef CURL_H
#define CURL_H

#include <curl/curl.h>

void CurlDownloadTo(const char* url, const char* filePath, const char* fileName);
void CurlDownloadWithSpecialExportTo(const char** name, unsigned int count, const char* filePath, const char* fileName);

#endif //CURL_H
