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

#include "GameToolbox/enums.h"

#include "ProfilePage.h"

#include <MenuItemSpriteExtra.h>

#include "GameManager.h"
#include "ui/UIScale9Sprite.h"
#include "ButtonSprite.h"
#include "2d/Menu.h"
#include "UTF8.h"
#include "base/Director.h"
#include "LoadingCircle.h"
#include "SimplePlayer.h"
#include "GameManager.h"
#include "GJUserScore.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "GameToolbox/log.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/network.h"
#include "GameToolbox/conv.h"
#include "GameToolbox/nodes.h"
#include "2d/Label.h"

USING_NS_AX;

ProfilePage* ProfilePage::create(int accountID, bool mainMenuProfile)
{
	ProfilePage* pRet = new ProfilePage();

	if (pRet && pRet->init(accountID, mainMenuProfile))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool ProfilePage::init(int accountID, bool mainMenuProfile)
{
	GameToolbox::log("{}", accountID);
	if (!PopupLayer::init()) return false;
	const auto& winSize = Director::getInstance()->getWinSize();
	_accountID = accountID;

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square01.png"));
	bg->setContentSize({ 440.0, 290.0 });
	bg->setPosition(winSize / 2);

	_mainLayer->addChild(bg);

	auto playerName = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Hello World");
	ax::Vec2 playerNamePos { winSize.width / 2, ((winSize.height / 2) + 145.0f) - 20.0f};
	playerName->setPosition(playerNamePos);
	GameToolbox::limitLabelWidth(playerName, 185.0f, 0.9f, 0.0f);

	_mainLayer->addChild(playerName);

	auto floorLine = Sprite::createWithSpriteFrameName("floorLine_001.png");
	floorLine->setOpacity(100);
	floorLine->setScaleX(0.8f);
	playerNamePos.y -= 21.0f;
	floorLine->setPosition(playerNamePos);

	_mainLayer->addChild(floorLine);

	// TODO: GJCommentListLayer position: -170.0f,-22.5f;

	auto closeBtnSprite = Sprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	auto closeBtn = MenuItemSpriteExtra::create(closeBtnSprite, [&](Node*) { this->close(); });

	auto menu = Menu::create();
	menu->setPosition((winSize.width / 2 - 220.0f) + 10.0f, ((winSize.y / 2) + 145.0f) - 10.0f );
	menu->addChild(closeBtn);

	_errorMsg = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), "Something went wrong...");
	_errorMsg->setScale(0.6f);
	_errorMsg->setPosition({ winSize.width / 2, winSize.height / 2 - 53.0f});
	_errorMsg->setVisible(false);

	_mainLayer->addChild(_errorMsg);

	auto gm = GameManager::getInstance();
	if (!mainMenuProfile)
	{
		bool isFollowingUser = gm->isFollowingUser(accountID);
		const char* heartSprite = isFollowingUser ? "gj_heartOn_001.png" : "gj_heartOff_001.png";
		auto heartObject = MenuItemSpriteExtra::create(heartSprite, [this](Node* btn)
		{
			bool isFollowingUser = static_cast<bool>(btn->getTag());
			//if(isFollowingUser) {
			//	GameManager::getInstance()->followUser(this->_accountID);
			//}
			//else {
			//	GameManager::getInstance()->unfollowUser(this->_accountID);
			//}
			
			const char* heartSprite = isFollowingUser ? "gj_heartOff_001.png" : "gj_heartOn_001.png";
			if(auto btn2 = dynamic_cast<MenuItemSpriteExtra*>(btn); btn) {
				btn2->setSpriteFrame(heartSprite);
			}
			isFollowingUser = !isFollowingUser;
			btn->setTag(static_cast<int>(isFollowingUser));
		});
		heartObject->setTag(static_cast<int>(isFollowingUser));
		ax::Vec2 heartPosition = { (winSize.width / 2) - 198.0f, (winSize.height / 2) - 123.0f };
		heartObject->setPosition(menu->convertToNodeSpace(heartPosition));
		
		if (!isFollowingUser)
		{
			auto followTxtSprite = Sprite::createWithSpriteFrameName("GJ_followTxt_001.png");
			followTxtSprite->setPosition(heartPosition.operator+({48.0f,-2.0f}));
			
			_mainLayer->addChild(followTxtSprite);
		}

		menu->addChild(heartObject);
		
	}

	_loadingcircle = LoadingCircle::create();
	_loadingcircle->setPosition({ winSize.width / 2, winSize.height / 2 - 53.0f});
	_mainLayer->addChild(_loadingcircle);
	
	_mainLayer->addChild(menu);

	if(mainMenuProfile)
	{
		auto refreshBtnSprite = Sprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
		auto refreshBtn = MenuItemSpriteExtra::create(refreshBtnSprite, [&](Node*) { /*Refresh Function*/ });
		refreshBtn->setPosition(menu->convertToNodeSpace({(winSize.width / 2 - 220.0f) + 10.0f, (winSize.height / 2 - 145.0f) + 10.0f + 1.0f}));
		//refreshBtn->setScaleMultiplier(1.5f);
		menu->addChild(refreshBtn);
	}

	std::string postData = fmt::format("secret=Wmfd2893gb7&targetAccountID={}", accountID);
	GameToolbox::log("postData: {}", postData);
	
	auto _request = new ax::network::HttpRequest();
	_request->setUrl("http://www.boomlings.com/database/getGJUserInfo20.php");
	_request->setRequestType(ax::network::HttpRequest::Type::POST);
	_request->setHeaders(std::vector<std::string>{"user-agent: "});
	_request->setRequestData(postData.c_str(), postData.length());
	_request->setResponseCallback(AX_CALLBACK_2(ProfilePage::onHttpRequestCompleted, this));
	_request->setTag("valid");
	ax::network::HttpClient::getInstance()->send(_request);
	_request->release();



	return true;
}



void ProfilePage::loadPageFromUserInfo(GJUserScore* score) // replace with 'GJUserScore* score'
{
	const auto& winSize = Director::getInstance()->getWinSize();

	//GameToolbox::limitLabelWidth(playerName, 185.0f, 0.9f, 0.0f);

	auto playerStatsNode = Node::create();
	std::string_view statSpriteName;

	bool hasCP = score->_creatorpoints > 0;
	int statValue;
	ax::Vec2 initialPos = ax::Vec2::ZERO;
	
	float v16 = 0.0f;
	
	for (int i = 0; i < (hasCP ? 6 : 5); i++)
	{
		switch (i)
		{
			case 1:
				statSpriteName = "GJ_diamondsIcon_001.png";
				statValue = score->_diamonds;
				break;
			case 2:
				statSpriteName = "GJ_coinsIcon_001.png";
				statValue = score->_secretCoins;
				break;
			case 3:
				statSpriteName = "GJ_coinsIcon2_001.png";
				statValue = score->_usercoins;
				break;
			case 4:
				statSpriteName = "GJ_demonIcon_001.png";
				statValue = score->_demons;
				break;
			case 5:
				statSpriteName = "GJ_hammerIcon_001.png";
				statValue = score->_creatorpoints;
				break;
			default:
				statSpriteName = "GJ_starsIcon_001.png";
				statValue = score->_stars;
				break;
		}
		int widthLimit = hasCP ? 50 : 60;
		std::string bigFontStr = GameToolbox::getTextureString("bigFont.fnt");
		auto statLabel = Label::createWithBMFont(bigFontStr, std::to_string(statValue));
		GameToolbox::limitLabelWidth(statLabel, widthLimit, 0.6, 0.0);
		statLabel->setAnchorPoint({0.0f, 0.5f});
		statLabel->setScale(0.6f);
		statLabel->setPosition(initialPos);
		playerStatsNode->addChild(statLabel);

		auto statSprite = Sprite::createWithSpriteFrameName(statSpriteName);
		playerStatsNode->addChild(statSprite);

		auto labelPos = statLabel->getPosition();
		auto scaledContentSize = statLabel->getContentSize();
		scaledContentSize.x *= statLabel->getScaleX();
		scaledContentSize.x += 14.0f;
		ax::Vec2 startSpritePos {labelPos.x + scaledContentSize.x, 0.0f};
		statSprite->setPosition(startSpritePos);
		statSprite->setScale(1);

		auto statSpritePos = statSprite->getPosition();
		statSpritePos.x += 20.0f;
		initialPos = statSpritePos;

		float labelContentSizeX = statLabel->getContentSize().x;
		float v27 = v16 + ((labelContentSizeX * statLabel->getScale()) + 14.0);
		float v28 = i == 1 ? 20.0f : 10.0f;
		v16 = v27 + v28;
	}

	//no idea but i guess its the length
	size_t youtubeLen = score->_youTube.length(); 
	size_t twitterLen = score->_twitter.length();
	float offset = 340.0f;
	bool twitterIsNotEmpty = twitterLen != 0;
	int v35 = youtubeLen ? twitterIsNotEmpty + 1 : twitterIsNotEmpty;

	size_t twitchLen = score->_twitch.length();
	if(twitchLen) {
		v35++;
	}
	if(v35 <= 1) {
		offset = 420.0f;
	}
	if(v16 > offset)
	{
		float v37 = offset / v16;
		v16 = offset;
		playerStatsNode->setScale(v37);
	}
	
	playerStatsNode->setPosition((winSize.x / 2) - (v16 / 2), winSize.y / 2 + 85);
	_mainLayer->addChild(playerStatsNode);

	IconType gamemode;
	int iconID;

	for (int i = 0; i < 7; i++)
	{
		switch(i)
		{
			case 0:
				gamemode = IconType::kIconTypeCube;
				iconID = score->_accIcon;
				break;
			case 1:
				gamemode = IconType::kIconTypeShip;
				iconID = score->_accShip;
				break;
			case 2:
				gamemode = IconType::kIconTypeBall;
				iconID = score->_accBall;
				break;
			case 3:
				gamemode = IconType::kIconTypeUfo;
				iconID = score->_accBird;
				break;
			case 4:
				gamemode = IconType::kIconTypeWave;
				iconID = score->_accDart;
				break;
			case 5:
				gamemode = IconType::kIconTypeRobot;
				iconID = score->_accRobot;
				break;
			case 6:
				gamemode = IconType::kIconTypeSpider;
				iconID = score->_accSpider;
				break;
		}

		SimplePlayer* iconSprite = SimplePlayer::create(iconID);
		iconSprite->updateGamemode(iconID, gamemode);
		ax::Vec2 iconPos = {i * 48.0f - 144.0f, 0.0f};
		iconPos += {winSize.width / 2, winSize.height / 2 + 40.0f};
		iconSprite->setPosition(iconPos);
		iconSprite->setMainColor(GameToolbox::colorForIdx(score->_playerColor));
		iconSprite->setSecondaryColor(GameToolbox::colorForIdx(score->_playerColor2));
		if (score->_accGlow) {
			iconSprite->setGlow(true);
			iconSprite->setGlowColor(GameToolbox::colorForIdx(score->_playerColor2));
		}
		_mainLayer->addChild(iconSprite);
		_loadingcircle->setVisible(false);
	}
}



void ProfilePage::onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response)
{
	if (auto str = GameToolbox::getResponse(response))
	{
		std::string_view strResp {*str};
		
		GameToolbox::log("RESPONSE: {}", strResp);
		auto score = GJUserScore::createWithResponse(strResp);
		loadPageFromUserInfo(score);
		delete score;

	}
	else {
		_loadingcircle->setVisible(false);
		_errorMsg->setVisible(true);
	}
}
