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

	level->_LevelID = std::stoi(stuff[1]);
	level->_LevelName = stuff[3];
	level->_Version = std::stoi(stuff[9]);
	level->_PlayerID = std::stoi(stuff[11]);
	//level->_Stars = std::stoi(stuff[17]);
	level->_MusicID = std::stoi(stuff[19]);
	
	level->_LevelString = stuff[7];

	/*while (parsed != 0)
	{
		int key = std::stoi(std::string(parsed));
		switch (key)
		{
			PARSE_KEY(1, level->_LevelID, PKINT);
			PARSE_KEY(5, level->_Version, PKINT);
			PARSE_KEY(6, level->_PlayerID, PKINT);
			PARSE_KEY(8, level->_DifficultyDenominator, PKINT);
			PARSE_KEY(9, level->_DifficultyNumerator, PKINT);
			PARSE_KEY(10, level->_Downloads, PKINT);
			PARSE_KEY(11, level->_SetCompletes, PKINT);
			PARSE_KEY(12, level->_MusicID, PKINT);
			PARSE_KEY(13, level->_GameVersion, PKINT);
			PARSE_KEY(14, level->_Likes, PKINT);
			PARSE_KEY(15, level->_Length, PKINT);
			PARSE_KEY(16, level->_Dislikes, PKINT);
			PARSE_KEY(18, level->_Stars, PKINT);
			PARSE_KEY(19, level->_FeatureScore, PKINT);
			PARSE_KEY(30, level->_SongID, PKINT);
			PARSE_KEY(37, level->_Coins, PKINT);
			PARSE_KEY(39, level->_StarsRequested, PKINT);
			PARSE_KEY(41, level->_DailyNumber, PKINT);
			PARSE_KEY(42, level->_Epic, PKINT);
			PARSE_KEY(43, level->_DemonDifficulty, PKINT);
			PARSE_KEY(45, level->_Objects, PKINT);
			PARSE_KEY(46, level->_EditorTime, PKINT);
			PARSE_KEY(47, level->_EditorTimeTotal, PKINT);
			PARSE_KEY(17, level->_Demon, PKBOOL);
			PARSE_KEY(25, level->_Auto, PKBOOL);
			PARSE_KEY(31, level->_2P, PKBOOL);
			PARSE_KEY(38, level->_VerifiedCoins, PKBOOL);
			PARSE_KEY(40, level->_LDM, PKBOOL);
			PARSE_KEY(44, level->_Gauntlet, PKBOOL);
			PARSE_KEY(2, level->_LevelName, PKSTRING);
			PARSE_KEY(4, level->_LevelString, PKSTRING);
			PARSE_KEY(26, level->_RecordString, PKSTRING);
			PARSE_KEY(27, level->_XORPassword, PKSTRING);
			PARSE_KEY(28, level->_UploadDate, PKSTRING);
			PARSE_KEY(29, level->_UpdateDate, PKSTRING);
			PARSE_KEY(36, level->_ExtraString, PKSTRING);
			PARSE_KEY(48, level->_Settings, PKSTRING);
			PARSE_KEY(3, level->_Description, PKBASE64);
		}
		parsed = strtok(NULL, ":");
	}*/
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