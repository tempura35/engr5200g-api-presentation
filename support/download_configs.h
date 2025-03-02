#ifndef DOWNLOAD_CONFIGS_H
#define DOWNLOAD_CONFIGS_H

#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;
using std::ostream;
using std::endl;

struct download_configs {
    string outputDir;
    int concurrentDownloads;
    vector<string> downloadUrls;
};

download_configs ReadDownloadConfigs(char* filePath);

ostream& operator<<(ostream& os, const download_configs& data);

string ExtractFileName(const string& url);

#endif
