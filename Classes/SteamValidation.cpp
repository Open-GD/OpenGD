#include "SteamValidation.h"

#include <fstream>

#include <filesystem>

// #if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
// #include <windows.h>
// #endif


bool SteamValidation::isGDBought() 
{   
    #if (AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID) || (AX_TARGET_PLATFORM == AX_PLATFORM_IOS) || (AX_TARGET_PLATFORM == AX_PLATFORM_MAC) || (AX_TARGET_PLATFORM == AX_PLATFORM_UNKNOWN)
    return false;
    #endif

    #if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32) || (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX)
    std::vector<std::string> gd_files = {
        "zlib1.dll", "GeometryDash.exe", "websockets.dll", 
        "steam_api.dll", "sqlite3.dll", "sdkencryptedappticket.dll", 
        "pthreadVCE2.dll", "libtiff.dll", "libExtensions.dll",
        "libcurl.dll", "libcocos2d.dll", "glew32.dll",
        "fmod.dll"
    };
    #else
    std::vector<std::string gd_files = {};
    #endif
    int i = 0;
    
    auto paths = SteamValidation::getPossibleGDPaths();

    while(i < paths.size())
    {
        int ii = 0;
        while(ii < gd_files.size())
        {
            std::string p = paths[i] + gd_files[ii];
            std::ifstream current_file(p);

            if(current_file.good())
            {
                return true;
            }

            current_file.close();

            ii++;
        }
        i++;
    }

    return false;
}

void SteamValidation::showErrorMessage()
{
    ax::ccMessageBox("Please, buy original GD first to support RobTop!", "Error");
    auto paths = SteamValidation::getPossibleGDPaths();
    std::string str = "If you already bought the game, copy its contents to ";
    str += paths[0];
    ax::ccMessageBox(str.c_str(), "Question");
}
void SteamValidation::copyResources()
{
    #if (AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID) || (AX_TARGET_PLATFORM == AX_PLATFORM_IOS) || (AX_TARGET_PLATFORM == AX_PLATFORM_MAC) || (AX_TARGET_PLATFORM == AX_PLATFORM_UNKNOWN)
    return false;
    #endif

    auto paths = SteamValidation::getPossibleGDPaths();

    std::string availablePath = "a";
    
    int i = 0;
    while(i < paths.size())
    {
        std::string p = paths[i] + "Resources/achievement_01.ogg";
        std::ifstream current_file(p);

        if(current_file.good())
        {
            availablePath = paths[i];
            break;
        }

        i++;
    }

    if(!availablePath.compare("a"))
    {
        ax::ccMessageBox("Couldn't get correct folder for copying Resources folder!", "Error");
        exit(1);
    }

    std::ifstream test_file1("Resources/achievement_01.ogg");
    std::ifstream test_file3("achievement_01.ogg");
    std::ifstream test_file2("Resources/object.json");
    std::ifstream test_file4("object.json");

    if(!test_file2.good() && !test_file4.good()) 
    {
        auto req = new ax::network::HttpRequest();
        req->setUrl("https://gist.githubusercontent.com/SergeyMC9730/8c5af2ccca4b856051d4c5f40531e259/raw/d5a8881a7c39bda24f16c8cf6fd62fef83c75f91/object.json");
        req->setRequestType(ax::network::HttpRequest::Type::GET);
        req->setResponseCallback(AX_CALLBACK_2(SteamValidation::Downloader::onDownload, new SteamValidation::Downloader()));
        ax::network::HttpClient::getInstance()->send(req);
        req->release();
    }

    if(!test_file1.good() && !test_file3.good())
    {
        //ax::ccMessageBox("Resources are gonna be copied from your GD installation", "Message");
        
        try {
            std::filesystem::copy(availablePath + "Resources", "./Resources");
        } catch (std::filesystem::filesystem_error &e) {
            std::string err = "Unable to copy resources: ";
            err += e.what();
        }
    }

    ax::Sprite *testspr = ax::Sprite::create("bigFont.png");
    if(!testspr) {
        try {
            std::filesystem::copy(availablePath + "Resources", "./");
            std::filesystem::remove_all("./Resources");
        } catch (std::filesystem::filesystem_error &e) {
            std::string err = "Unable to move resources: ";
            err += e.what();
        }
    } else {
        testspr->release();
    }

    test_file1.close();
    test_file2.close();
    test_file3.close();
    test_file4.close();
}

void SteamValidation::Downloader::onDownload(
    ax::network::HttpClient* sender, ax::network::HttpResponse* response) 
{
    if(response->getResponseCode() == 200) 
    {
        auto str = response->getResponseData()->data();

        std::ofstream objectfile;
        objectfile.open("Resources/object.json");
        objectfile << str;
        objectfile.close();
        objectfile.open("object.json");
        objectfile << str;
        objectfile.close();
    }
}

std::vector<std::string> SteamValidation::getPossibleGDPaths()
{
    #if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
    return {"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Geometry Dash\\"};
    #elif (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX)
    return {
        "~/.var/app/com.valvesoftware.Steam/.steam/root/steamapps/common/Geometry Dash/",
        "~/.steam/root/steamapps/common/Geometry Dash/",
        "~/.var/app/com.valvesoftware.Steam/data/Steam/steamapps/common/Geometry Dash/",
    };
    #elif (AX_TARGET_PLATFORM == AX_PLATFORM_MAC)
    return {"~/Library/Application Support/Steam/steamapps/common/Geometry Dash/"};
    #else
    return {};
    #endif
}
