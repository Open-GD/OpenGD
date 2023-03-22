#include "GJGameLevel.h"
#include "base64.h"

#include <cstring>
#include <iostream>

#include <ZipUtils.h>

GJGameLevel* GJGameLevel::createWithResponse(std::string backendResponse)
{
	GJGameLevel* level = new GJGameLevel();
	if (!level)
		return nullptr;
	//                                                                                                                          -
	// 1:6508283:2:ReTraY:3:VGhhbmtzIGZvciBwbGF5aW5nIEdlb21ldHJ5IERhc2g=:4:{levelString}:5:3:6:4993756:8:10:9:10:10:39431612:12:0:13:21:14:4125578:17::43:3:25::18:2:19:7730:42:0:45:20000:15:3:30:0:31:0:28:5 years:29:1 year:35:557117:36:0_733_0_0_0_0_574_716_0_0_352_78_729_0_42_0_833_68_0_347_0_38_240_205_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0:37:3:38:1:39:2:46:7729:47:13773:40:0:27:AwMABAYDBw==#eb541c03f8355c0709f8007a1d9a595ae5bedc5d#291568b26b08d70a198fca10a87c736a2823be0c

	auto stuff = GameToolbox::splitByDelim(backendResponse, ':');

	std::map<std::string, std::string> levelResponse;

	for(size_t i = 0; i < stuff.size(); i+=2)
	{
		//GameToolbox::log("key {} value {}", stuff[i], stuff[i+1]);
		levelResponse.insert({stuff[i], stuff[i+1]});
	}

	if(levelResponse.contains("1") && !levelResponse["1"].empty()) level->_LevelID = std::stoi(levelResponse["1"]);
	if(levelResponse.contains("2") && !levelResponse["2"].empty()) level->_LevelName = levelResponse["2"];
	if(levelResponse.contains("3") && !levelResponse["3"].empty()) level->_Description = levelResponse["3"];
	if(levelResponse.contains("4") && !levelResponse["4"].empty()) level->_LevelString = levelResponse["4"];
	if(levelResponse.contains("5") && !levelResponse["5"].empty()) level->_Version = std::stoi(levelResponse["5"]);
	if(levelResponse.contains("6") && !levelResponse["6"].empty()) level->_PlayerID = std::stoi(levelResponse["6"]);
	if(levelResponse.contains("8") && !levelResponse["8"].empty()) level->_DifficultyDenominator = std::stoi(levelResponse["8"]);
	if(levelResponse.contains("9") && !levelResponse["9"].empty()) level->_DifficultyNumerator = std::stoi(levelResponse["9"]);
	if(levelResponse.contains("10") && !levelResponse["10"].empty()) level->_Downloads = std::stoi(levelResponse["10"]);
	if(levelResponse.contains("11") && !levelResponse["11"].empty()) level->_SetCompletes = std::stoi(levelResponse["11"]);
	if(levelResponse.contains("12") && !levelResponse["12"].empty()) level->_SongID = std::stoi(levelResponse["12"]);
	if(levelResponse.contains("13") && !levelResponse["13"].empty()) level->_GameVersion = std::stoi(levelResponse["13"]);
	if(levelResponse.contains("14") && !levelResponse["14"].empty()) level->_Likes = std::stoi(levelResponse["14"]);
	if(levelResponse.contains("15") && !levelResponse["15"].empty()) level->_Length = std::stoi(levelResponse["15"]);
	if(levelResponse.contains("16") && !levelResponse["16"].empty()) level->_Dislikes = std::stoi(levelResponse["16"]);
	if(levelResponse.contains("17") && !levelResponse["17"].empty()) level->_Demon = std::stoi(levelResponse["17"]);
	if(levelResponse.contains("18") && !levelResponse["18"].empty()) level->_Stars = std::stoi(levelResponse["18"]);
	if(levelResponse.contains("19") && !levelResponse["19"].empty()) level->_FeatureScore = std::stoi(levelResponse["19"]);
	if(levelResponse.contains("25") && !levelResponse["25"].empty()) level->_Auto = std::stoi(levelResponse["25"]);
	if(levelResponse.contains("26") && !levelResponse["26"].empty()) level->_RecordString = levelResponse["26"];
	if(levelResponse.contains("28") && !levelResponse["28"].empty()) level->_UploadDate = levelResponse["28"];
	if(levelResponse.contains("29") && !levelResponse["29"].empty()) level->_UpdateDate = levelResponse["29"];
	if(levelResponse.contains("30") && !levelResponse["30"].empty()) level->_CopiedID = std::stoi(levelResponse["30"]);
	if(levelResponse.contains("35") && !levelResponse["35"].empty()) level->_SongID = std::stoi(levelResponse["35"]);
	if(levelResponse.contains("37") && !levelResponse["37"].empty()) level->_Coins = std::stoi(levelResponse["37"]);
	if(levelResponse.contains("39") && !levelResponse["39"].empty()) level->_StarsRequested = std::stoi(levelResponse["39"]);
	if(levelResponse.contains("40") && !levelResponse["40"].empty()) level->_LDM = std::stoi(levelResponse["40"]);
	if(levelResponse.contains("41") && !levelResponse["41"].empty()) level->_DailyNumber = std::stoi(levelResponse["41"]);
	if(levelResponse.contains("42") && !levelResponse["42"].empty()) level->_Epic = std::stoi(levelResponse["42"]);
	if(levelResponse.contains("43") && !levelResponse["43"].empty()) level->_DemonDifficulty = std::stoi(levelResponse["43"]);
	if(levelResponse.contains("44") && !levelResponse["44"].empty()) level->_Gauntlet = std::stoi(levelResponse["44"]);
	if(levelResponse.contains("45") && !levelResponse["45"].empty()) level->_Objects = std::stoi(levelResponse["45"]);
	if(levelResponse.contains("46") && !levelResponse["46"].empty()) level->_EditorTime = std::stoi(levelResponse["46"]);
	if(levelResponse.contains("47") && !levelResponse["47"].empty()) level->_EditorTimeTotal = std::stoi(levelResponse["47"]);

	return level;
}
GJGameLevel::GJGameLevel(std::string levelName, int levelID)
{
	this->_LevelName = levelName;
	this->_LevelID = levelID;
	this->_MusicID = levelID - 1;
}
GJGameLevel* GJGameLevel::create() 
{
	return createWithMinimumData("", "", 0);
}
GJGameLevel* GJGameLevel::createWithMinimumData(std::string levelName, std::string levelCreator, int levelID) 
{
	GJGameLevel* level = new GJGameLevel(levelName, levelID);

	if (!level)
		return nullptr;

	level->_LevelCreator = levelCreator;

	return level;
}

std::string GJGameLevel::decompressLvlStr(std::string compressedLvlStr)
{
	if (compressedLvlStr.empty())
		return "";
	
	std::replace(compressedLvlStr.begin(), compressedLvlStr.end(), '_', '/');
	std::replace(compressedLvlStr.begin(), compressedLvlStr.end(), '-', '+');
	
	std::string decoded = base64_decode(compressedLvlStr);
	
	unsigned char* data = (unsigned char*)decoded.data();
	unsigned char* a = nullptr;
	ssize_t deflatedLen = ax::ZipUtils::inflateMemory(data, decoded.length(), &a);
		
	return std::string(reinterpret_cast<char*>(a));
}

std::string GJGameLevel::getDifficultySprite(GJGameLevel* level) {
    // if (level->_Auto) {
    //     return "difficulty_auto_btn_001.png";
    // }

	int diff = 0;

    if (level->_DifficultyNumerator != 0)
        diff = level->_DifficultyNumerator / 10;

    if (level->_Demon) {
        switch (level->_DemonDifficulty) {
        case 3:
            return "difficulty_07_btn2_001.png";
        case 4:
            return "difficulty_08_btn2_001.png";
        case 5:
            return "difficulty_09_btn2_001.png";
        case 6:
            return "difficulty_10_btn2_001.png";
        default:
        case 0:
            return "difficulty_06_btn_001.png";
        }
    }

    switch (diff) {
    case 1:
        return "difficulty_01_btn_001.png";
    case 2:
        return "difficulty_02_btn_001.png";
    case 3:
        return "difficulty_03_btn_001.png";
    case 4:
        return "difficulty_04_btn_001.png";
    case 5:
        return "difficulty_05_btn_001.png";
    case 6:
        return "difficulty_06_btn_001.png";
    default:
        return "difficulty_00_btn_001.png";
    }
}