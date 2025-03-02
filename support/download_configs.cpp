#include <iostream>
#include <fstream>
#include <string>
#include <yaml-cpp/yaml.h>
#include "download_configs.h"

using std::ifstream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

download_configs ReadDownloadConfigs(char* filePath) {
    struct download_configs structConfigs;

    ifstream configFs;
    configFs.open(filePath);
    if (!configFs.is_open()) {
        cerr << "Failed to open: " << filePath << endl;
        // cerr << strerror(errno) << endl;
        return structConfigs;
    }

    YAML::Node yamlConfigs = YAML::LoadFile(filePath);
    structConfigs.concurrentDownloads = yamlConfigs["concurrent_downloads"].as<int>();
    structConfigs.outputDir = yamlConfigs["output_dir"].as<string>();

    YAML::Node yamlFileArray = yamlConfigs["files"];
    for (size_t i = 0; i < yamlFileArray.size(); ++i) {
        structConfigs.downloadUrls.push_back(yamlFileArray[i].as<string>());
    }
    // cout << "Parsed file array: " << structConfigs.downloadUrls.size() << " " << yamlFileArray.size() << endl;

    return structConfigs;
}

ostream& operator<<(ostream& os, const download_configs& data) {
    os << "Output Directory: " << data.outputDir << endl
        << "Maximum Downloads: " << data.concurrentDownloads << endl
        << "File Count: " << data.downloadUrls.size() << endl;
    for (size_t i = 0; i < data.downloadUrls.size(); ++i) {
        os << "\t" << data.downloadUrls[i] << endl;
    }

    return os;
}

string ExtractFileName(const string& url) {
    size_t lastDotPos = url.rfind("/");
    if (lastDotPos == string::npos) {
        return "";
    }

    return url.substr(lastDotPos + 1);
}