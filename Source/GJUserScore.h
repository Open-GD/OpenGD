#pragma once
#include <string>
#include <string_view>

class GJUserScore;

class GJUserScore { 
public:
    std::string _userName;
    int _userID = 0;
    int _stars = 0;
    int _demons = 0;
    int _ranking = 0;
    int _accountHighlight = 0;
    int _creatorpoints = 0;
    int _iconID = 0;
    int _playerColor = 0;
    int _playerColor2 = 0;
    int _secretCoins = 0;
    int _iconType = 0;
    int _special = 0;
    int _accountID = 0;
    int _usercoins = 0;
    int _messageState = 0;
    int _friendsState = 0;
    std::string _youTube;
    int _accIcon = 0;
    int _accShip = 0;
    int _accBall = 0;
    int _accBird = 0;
    int _accDart = 0;
    int _accRobot = 0;
    int _accStreak = 0;
    int _accGlow = 0;
    int _isRegistered = 0;
    int _globalRank = 0;
    int _friendState = 0;
    int _messages = 0;
    int _friendRequests = 0;
    int _newFriends = 0;
    int _newFriendRequest = 0;
    std::string _age;
    int _accSpider = 0;
    std::string _twitter;
    std::string _twitch;
    int _diamonds = 0;
    int _accExplosion = 0;
    int _modlevel = 0;
    int _commentHistoryState = 0;

	static GJUserScore *createWithResponse(std::string_view backendResponse);
	static GJUserScore *create();
};
