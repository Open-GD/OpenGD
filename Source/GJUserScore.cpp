/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#include "GJUserScore.h"

#include "external/base64.h"
#include "GameToolbox/conv.h"

static inline std::string _toString(std::string_view s)
{
	return {s.begin(), s.end()};
}

GJUserScore* GJUserScore::createWithResponse(std::string_view backendResponse)
{
	GJUserScore* score = new GJUserScore();
	if (!score) return nullptr;

	if(!backendResponse.compare("-1"))
	{
		delete score;
		return nullptr;
	}

	auto stuff = GameToolbox::splitByDelimStringView(backendResponse, ':');

	std::map<std::string_view, std::string_view> scoreResponse;

	for (size_t i = 0; i < stuff.size(); i += 2)
	{
		if (!stuff[i + 1].empty())
			scoreResponse.insert({stuff[i], stuff[i + 1]});
	}
	
	if (scoreResponse.contains("1")) score->_userName = _toString(scoreResponse["1"]);
	if (scoreResponse.contains("2")) score->_userID = GameToolbox::stoi(scoreResponse["2"]);
	if (scoreResponse.contains("3")) score->_stars = GameToolbox::stoi(scoreResponse["3"]);
	if (scoreResponse.contains("4")) score->_demons = GameToolbox::stoi(scoreResponse["4"]);
	if (scoreResponse.contains("6")) score->_ranking = GameToolbox::stoi(scoreResponse["6"]);
	if (scoreResponse.contains("7")) score->_accountHighlight = GameToolbox::stoi(scoreResponse["7"]);
	if (scoreResponse.contains("8")) score->_creatorpoints = GameToolbox::stoi(scoreResponse["8"]);
	if (scoreResponse.contains("9")) score->_iconID = GameToolbox::stoi(scoreResponse["9"]);
	if (scoreResponse.contains("10")) score->_playerColor = GameToolbox::stoi(scoreResponse["10"]);
	if (scoreResponse.contains("11")) score->_playerColor2 = GameToolbox::stoi(scoreResponse["11"]);
	if (scoreResponse.contains("13")) score->_secretCoins = GameToolbox::stoi(scoreResponse["13"]);
	if (scoreResponse.contains("14")) score->_iconType = GameToolbox::stoi(scoreResponse["14"]);
	if (scoreResponse.contains("15")) score->_special = GameToolbox::stoi(scoreResponse["15"]);
	if (scoreResponse.contains("16")) score->_accountID = GameToolbox::stoi(scoreResponse["16"]);
	if (scoreResponse.contains("17")) score->_usercoins = GameToolbox::stoi(scoreResponse["17"]);
	if (scoreResponse.contains("18")) score->_messageState = GameToolbox::stoi(scoreResponse["18"]);
	if (scoreResponse.contains("19")) score->_friendsState = GameToolbox::stoi(scoreResponse["19"]);
	if (scoreResponse.contains("20")) score->_youTube = _toString(scoreResponse["20"]);
	if (scoreResponse.contains("21")) score->_accIcon = GameToolbox::stoi(scoreResponse["21"]);
	if (scoreResponse.contains("22")) score->_accShip = GameToolbox::stoi(scoreResponse["22"]);
	if (scoreResponse.contains("23")) score->_accBall = GameToolbox::stoi(scoreResponse["23"]);
	if (scoreResponse.contains("24")) score->_accBird = GameToolbox::stoi(scoreResponse["24"]);
	if (scoreResponse.contains("25")) score->_accDart = GameToolbox::stoi(scoreResponse["25"]);
	if (scoreResponse.contains("26")) score->_accRobot = GameToolbox::stoi(scoreResponse["26"]);
	if (scoreResponse.contains("27")) score->_accStreak = GameToolbox::stoi(scoreResponse["27"]);
	if (scoreResponse.contains("28")) score->_accGlow = GameToolbox::stoi(scoreResponse["28"]);
	if (scoreResponse.contains("29")) score->_isRegistered = GameToolbox::stoi(scoreResponse["29"]);
	if (scoreResponse.contains("30")) score->_globalRank = GameToolbox::stoi(scoreResponse["30"]);
	if (scoreResponse.contains("31")) score->_friendState = GameToolbox::stoi(scoreResponse["31"]);
	if (scoreResponse.contains("38")) score->_messages = GameToolbox::stoi(scoreResponse["38"]);
	if (scoreResponse.contains("39")) score->_friendRequests = GameToolbox::stoi(scoreResponse["39"]);
	if (scoreResponse.contains("40")) score->_newFriends = GameToolbox::stoi(scoreResponse["40"]);
	if (scoreResponse.contains("41")) score->_newFriendRequest = GameToolbox::stoi(scoreResponse["41"]);
	if (scoreResponse.contains("42")) score->_age = _toString(scoreResponse["42"]);
	if (scoreResponse.contains("43")) score->_accSpider = GameToolbox::stoi(scoreResponse["43"]);
	if (scoreResponse.contains("44")) score->_twitter = _toString(scoreResponse["44"]);
	if (scoreResponse.contains("45")) score->_twitch = _toString(scoreResponse["45"]);
	if (scoreResponse.contains("46")) score->_diamonds = GameToolbox::stoi(scoreResponse["46"]);
	if (scoreResponse.contains("48")) score->_accExplosion = GameToolbox::stoi(scoreResponse["48"]);
	if (scoreResponse.contains("49")) score->_modlevel = GameToolbox::stoi(scoreResponse["49"]);
	if (scoreResponse.contains("50")) score->_commentHistoryState = GameToolbox::stoi(scoreResponse["50"]);
		
	return score;
}

GJUserScore* GJUserScore::create() 
{
	GJUserScore* score = new GJUserScore;
	return score;
}
