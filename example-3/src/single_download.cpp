#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <string>
#include <yaml.h>
#include <errno.h>
#include <string.h>
#include "support/download_configs.h"

using std::ifstream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

int DownloadProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

int main(int argc, char* argv[]) {
    struct download_configs configs = ReadDownloadConfigs(argv[1]);
    cout << configs;

    CURL* easyHandle = curl_easy_init();
    if (easyHandle == NULL) {
        cerr << "Download handle init failed." << endl;
        return 1;
    }

    FILE* pDownloadFile;
    curl_easy_setopt(easyHandle, CURLOPT_XFERINFOFUNCTION, DownloadProgressCallback);
    for (const string& url : configs.downloadUrls) {
        string fileName = ExtractFileName(url);
        cout << "Downloading " << url << ". File name " << fileName << endl;
        pDownloadFile = fopen(fileName.c_str(), "wb");
        curl_easy_setopt(easyHandle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, pDownloadFile);
        curl_easy_setopt(easyHandle, CURLOPT_NOPROGRESS, 1);
        CURLcode res = curl_easy_perform(easyHandle);
        cout << "Download performed: " << res << endl;
        fclose(pDownloadFile);
    }

    curl_easy_cleanup(easyHandle);

    return 0;
}

int DownloadProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    cout << "Download Now: " << dlnow << endl;
    return CURL_PROGRESSFUNC_CONTINUE;
}