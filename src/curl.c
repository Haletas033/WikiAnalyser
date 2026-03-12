//
// Created by halet on 3/11/2026.
//

#include "../include/curl.h"

//Write callback
size_t WriteData(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
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
}
