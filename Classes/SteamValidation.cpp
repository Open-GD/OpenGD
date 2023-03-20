#include "SteamValidation.h"

#include <vector>
#include <string>
#include <fstream>

bool SteamValidation::isGDBought() 
{
    std::vector<std::string> gd_files = {
        "zlib.dll", "GeometryDash.exe", "websockets.dll", 
        "steam_api.dll", "sqlite3.dll", "sdkencryptedappticket.dll", 
        "pthreadVCE2.dll", "libtiff.dll", "libExtensions.dll",
        "libcurl.dll", "libcocos2d.dll", "glew32.dll",
        "fmod.dll"
    };
    int i = 0;
    
    std::string steam_gd_path = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Geometry Dash\\";

    while(i < gd_files.size())
    {
        std::ifstream current_file(steam_gd_path + gd_files[i]);

        if(!current_file.good())
        {
            return false;
        }

        i++;
    }

    return true;
}