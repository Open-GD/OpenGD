#include "GJGameLevel.h"
#include "GameToolbox.h"

#include "external/base64.h"

#include <ZipUtils.h>


//the only thing we actually want as normal string is the class members
static inline std::string _toString(std::string_view s) {
	return {s.begin(), s.end()};
}

GJGameLevel* GJGameLevel::createWithResponse(std::string_view backendResponse)
{
	GJGameLevel* level = new GJGameLevel();
	if (!level) return nullptr;

	if(!backendResponse.compare("-1"))
	{
		delete level;
		return nullptr;
	}
	//																														  -
	// 1:6508283:2:ReTraY:3:VGhhbmtzIGZvciBwbGF5aW5nIEdlb21ldHJ5IERhc2g=:4:{levelString}:5:3:6:4993756:8:10:9:10:10:39431612:12:0:13:21:14:4125578:17::43:3:25::18:2:19:7730:42:0:45:20000:15:3:30:0:31:0:28:5 years:29:1 year:35:557117:36:0_733_0_0_0_0_574_716_0_0_352_78_729_0_42_0_833_68_0_347_0_38_240_205_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0:37:3:38:1:39:2:46:7729:47:13773:40:0:27:AwMABAYDBw==#eb541c03f8355c0709f8007a1d9a595ae5bedc5d#291568b26b08d70a198fca10a87c736a2823be0c

	auto stuff = GameToolbox::splitByDelimStringView(backendResponse, ':');

	std::map<std::string_view, std::string_view> levelResponse;

	for (size_t i = 0; i < stuff.size(); i += 2)
	{
		if (!stuff[i + 1].empty())
			levelResponse.insert({stuff[i], stuff[i + 1]});
	}
	
	if (levelResponse.contains("1")) level->_levelID = GameToolbox::stoi(levelResponse["1"]);
	if (levelResponse.contains("2")) level->_levelName = _toString(levelResponse["2"]);
	if (levelResponse.contains("3")) level->_description = _toString(levelResponse["3"]);
	if (levelResponse.contains("4")) level->_levelString = _toString(levelResponse["4"]);
	if (levelResponse.contains("5")) level->_version = GameToolbox::stoi(levelResponse["5"]);
	if (levelResponse.contains("6")) level->_playerID = GameToolbox::stoi(levelResponse["6"]);
	if (levelResponse.contains("8")) level->_difficultyDenominator = GameToolbox::stoi(levelResponse["8"]);
	if (levelResponse.contains("9")) level->_difficultyNumerator = GameToolbox::stoi(levelResponse["9"]);
	if (levelResponse.contains("10")) level->_downloads = GameToolbox::stoi(levelResponse["10"]);
	if (levelResponse.contains("11")) level->_setCompletes = GameToolbox::stoi(levelResponse["11"]);
	if (levelResponse.contains("12")) level->_officialSongID = GameToolbox::stoi(levelResponse["12"]);
	if (levelResponse.contains("13")) level->_gameVersion = GameToolbox::stoi(levelResponse["13"]);
	if (levelResponse.contains("14")) level->_likes = GameToolbox::stoi(levelResponse["14"]);
	if (levelResponse.contains("15")) level->_length = GameToolbox::stoi(levelResponse["15"]);
	if (levelResponse.contains("16")) level->_dislikes = GameToolbox::stoi(levelResponse["16"]);
	if (levelResponse.contains("17")) level->_demon = GameToolbox::stoi(levelResponse["17"]);
	if (levelResponse.contains("18")) level->_stars = GameToolbox::stoi(levelResponse["18"]);
	if (levelResponse.contains("19")) level->_featureScore = GameToolbox::stoi(levelResponse["19"]);
	if (levelResponse.contains("25")) level->_auto = GameToolbox::stoi(levelResponse["25"]);
	if (levelResponse.contains("26")) level->_recordString = _toString(levelResponse["26"]);
	if (levelResponse.contains("28")) level->_uploadDate = _toString(levelResponse["28"]);
	if (levelResponse.contains("29")) level->_updateDate = _toString(levelResponse["29"]);
	if (levelResponse.contains("30")) level->_copiedID = GameToolbox::stoi(levelResponse["30"]);
	if (levelResponse.contains("35")) level->_songID = GameToolbox::stoi(levelResponse["35"]);
	if (levelResponse.contains("37")) level->_coins = GameToolbox::stoi(levelResponse["37"]);
	if (levelResponse.contains("38")) level->_verifiedCoins = GameToolbox::stoi(levelResponse["38"]);
	if (levelResponse.contains("39")) level->_starsRequested = GameToolbox::stoi(levelResponse["39"]);
	if (levelResponse.contains("40")) level->_LDM = GameToolbox::stoi(levelResponse["40"]);
	if (levelResponse.contains("41")) level->_dailyNumber = GameToolbox::stoi(levelResponse["41"]);
	if (levelResponse.contains("42")) level->_epic = GameToolbox::stoi(levelResponse["42"]);
	if (levelResponse.contains("43")) level->_demonDifficulty = GameToolbox::stoi(levelResponse["43"]);
	if (levelResponse.contains("44")) level->_gauntlet = GameToolbox::stoi(levelResponse["44"]);
	if (levelResponse.contains("45")) level->_objects = GameToolbox::stoi(levelResponse["45"]);
	if (levelResponse.contains("46")) level->_editorTime = GameToolbox::stoi(levelResponse["46"]);
	if (levelResponse.contains("47")) level->_editorTimeTotal = GameToolbox::stoi(levelResponse["47"]);
	
	return level;
}

//GJGameLevel::GJGameLevel(std::string levelName, int levelID)
//{
//	this->_levelName = levelName;
//	this->_levelID = levelID;
//	this->_musicID = levelID - 1;
//}

GJGameLevel* GJGameLevel::create() { return createWithMinimumData("", "", 0); }

GJGameLevel* GJGameLevel::createWithMinimumData(std::string levelName, std::string levelCreator, int levelID)
{
	GJGameLevel* level = new GJGameLevel;
	if (!level) return nullptr;

	level->_levelName = levelName;
	level->_levelID = levelID;
	level->_levelCreator = levelCreator;

	return level;
}

std::string GJGameLevel::decompressLvlStr(std::string compressedLvlStr)
{
	if (compressedLvlStr.empty()) return "";

	std::replace(compressedLvlStr.begin(), compressedLvlStr.end(), '_', '/');
	std::replace(compressedLvlStr.begin(), compressedLvlStr.end(), '-', '+');

	std::string decoded = base64_decode(compressedLvlStr);

	unsigned char* data = (unsigned char*)decoded.data();
	unsigned char* a = nullptr;
	ssize_t deflatedLen = ax::ZipUtils::inflateMemory(data, decoded.length(), &a);

	return std::string(reinterpret_cast<char*>(a));
}

std::string GJGameLevel::getDifficultySprite(GJGameLevel* level, DifficultyType type)
{
	if (level->_auto)
	{
		return "difficulty_auto_btn_001.png";
	}

	int diff = 0;

	if (level->_difficultyNumerator != 0) diff = level->_difficultyNumerator / 10;

	if (level->_demon)
	{
		switch (level->_demonDifficulty) {
		case 3:
			return type == kLevelInfoLayer ? "difficulty_07_btn2_001.png" : "difficulty_07_btn_001.png";
		case 4:
			return type == kLevelInfoLayer ? "difficulty_08_btn2_001.png" : "difficulty_08_btn_001.png";
		case 5:
			return type == kLevelInfoLayer ? "difficulty_09_btn2_001.png" : "difficulty_09_btn_001.png";
		case 6:
			return type == kLevelInfoLayer ? "difficulty_10_btn2_001.png" : "difficulty_10_btn_001.png";
		default:
		case 0:
			return type == kLevelInfoLayer ? "difficulty_06_btn2_001.png" : "difficulty_06_btn_001.png";
		}
	}

	switch (diff)
	{
	case 1:
		return type == kMainLevels ? "diffIcon_01_btn_001.png" : "difficulty_01_btn_001.png";
	case 2:
		return type == kMainLevels ? "diffIcon_02_btn_001.png" : "difficulty_02_btn_001.png";
	case 3:
		return type == kMainLevels ? "diffIcon_03_btn_001.png" : "difficulty_03_btn_001.png";
	case 4:
		return type == kMainLevels ? "diffIcon_04_btn_001.png" : "difficulty_04_btn_001.png";
	case 5:
		return type == kMainLevels ? "diffIcon_05_btn_001.png" : "difficulty_05_btn_001.png";
	case 6:
		return type == kMainLevels ? "diffIcon_06_btn_001.png" : "difficulty_06_btn_001.png";
	default:
		return type == kMainLevels ? "diffIcon_01_btn_001.png" : "difficulty_00_btn_001.png";
	}
}