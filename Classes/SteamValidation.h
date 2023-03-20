#pragma once

#include <vector>
#include <string>

#include <axmol.h>
#include <network/HttpClient.h>

/* this namespace includes functions, that checks if you bought the game and copies original game resources to folder with opengd */
namespace SteamValidation {
    class Downloader {
    public:
        void onDownload(ax::network::HttpClient* sender, ax::network::HttpResponse* response);
    };
    
    std::vector<std::string> getPossibleGDPaths();

    bool isGDBought();
    void showErrorMessage();
    void copyResources();
}