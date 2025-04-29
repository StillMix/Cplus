#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "helper.h"

using namespace std;

vector<string> findFolders(const string& folderName) {
    vector<string> foundFolders;
    string command;

    if (isCommandAvailable("locate")) {
        command = "locate -r '" + folderName + "$' 2>/dev/null";
    } else if (isCommandAvailable("plocate")) {
        command = "plocate -r '" + folderName + "$' 2>/dev/null";
    } else {
        installPlocate();
        command = "plocate -r '" + folderName + "$' 2>/dev/null";
    }

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return foundFolders;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        string folderPath(buffer);
        if (!folderPath.empty() && folderPath.back() == '\n') {
            folderPath.pop_back();
        }

        if (folderPath.find(".cache") == string::npos) {
            if (isDirectory(folderPath)) {
                foundFolders.push_back(folderPath);
            }
        }
    }

    // Добавляем поиск архивов с таким именем
    command = "locate -r '" + folderName + "\\.zip$' 2>/dev/null";
    pipe = popen(command.c_str(), "r");
    if (pipe) {
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            string zipPath(buffer);
            if (!zipPath.empty() && zipPath.back() == '\n') {
                zipPath.pop_back();
            }
            foundFolders.push_back(zipPath);
        }
        pclose(pipe);
    }

    return foundFolders;
}

void openFolder(const string& folderPath) {
    #if defined(_WIN32) || defined(_WIN64)
        string command = "explorer \"" + folderPath + "\"";
    #else
        string command = "xdg-open \"" + folderPath + "\" > /dev/null 2>&1 &";
    #endif
    system(command.c_str());
}