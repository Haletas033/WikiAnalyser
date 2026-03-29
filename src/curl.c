//
// Created by halet on 3/11/2026.
//

#include "../include/curl.h"

#include "../include/GUI.h"

//Write callback
size_t WriteData(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void GetPageXML(CURL* handle, const unsigned int count, const unsigned int i, char* body, const char** name, const char* bodyTail, const char* url, FILE* wikiDump) {
    int j;
    for (j = 0; j < count; j++) {
        strcat(body, name[i*100+j]);
        strcat(body, "%0A");
    }
    strcat(body, bodyTail);

    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_POST, 1L);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, body);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteData);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, wikiDump);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "WikiAnalyser/1.0 (haletas033@gmail.com)");

    curl_easy_perform(handle);
}

void CurlDownloadTo(const char* url, const char* filePath, const char* fileName) {
    CURL* handle = curl_easy_init();
    if (handle == NULL)
        return;

    char* tmp = malloc(strlen(filePath) + strlen(fileName) + 1);
    strcpy(tmp, filePath);
    strcat(tmp, fileName);

    FILE* wikiDumpZip = fopen(tmp, "wb");
    if (wikiDumpZip == NULL) {
        free(tmp);
        curl_easy_cleanup(handle);
        return;
    }

    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteData);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, wikiDumpZip);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_perform(handle);


    free(tmp);
    fclose(wikiDumpZip);
    curl_easy_cleanup(handle);

    progress = -1;
}

void CurlDownloadWithSpecialExportTo(const char** name, unsigned int count, const char* filePath, const char* fileName) {
    maxProgress = count-10;

    CURL* handle = curl_easy_init();
    if (handle == NULL)
        return;

    char* tmp = malloc(strlen(filePath) + strlen(fileName) + 1);
    strcpy(tmp, filePath);
    strcat(tmp, fileName);

    FILE* wikiDump = fopen(tmp, "w");
    if (wikiDump == NULL) {
        free(tmp);
        curl_easy_cleanup(handle);
        return;
    }

    const char* url = "https://en.wikipedia.org/wiki/Special:Export";
    const char* bodyRoot = "action=submit&pages=";
    const char* bodyTail = "&curonly=1";
    char* body = malloc(strlen(bodyRoot) + 512*100 + strlen(bodyTail));
    strcpy(body, bodyRoot);

    int i;
    for (i = 0; i < count/100; i++) {
        GetPageXML(handle, 100, i, body, name, bodyTail, url, wikiDump);

        free(body);
        body = malloc(strlen(bodyRoot) + 512*100 + strlen(bodyTail));
        strcpy(body, bodyRoot);
        progress += 100;
    }

    if (count % 100 != 0) {
        GetPageXML(handle, count % 100, i, body, name, bodyTail, url, wikiDump);
        progress+= count % 100;
    }

    curl_easy_cleanup(handle);

    fclose(wikiDump);
    free(tmp);
    free(body);
}