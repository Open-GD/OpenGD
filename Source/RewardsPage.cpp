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

#include "RewardsPage.h"

#include <MenuItemSpriteExtra.h>

#include <ui/CocosGUI.h>
#include "ButtonSprite.h"
#include "2d/Menu.h"
#include "UTF8.h"
#include "base/Director.h"
#include "GameManager.h"
#include "GJUserScore.h"
#include "2d/ActionInterval.h"
#include "RewardUnlockLayer.h"
#include "fmt/format.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "external/base64.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/conv.h"
#include "GameToolbox/network.h"
#include "GameToolbox/log.h"

USING_NS_AX;

RewardsPage* RewardsPage::create()
{
	RewardsPage* pRet = new RewardsPage();

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool RewardsPage::init()
{
	if (!PopupLayer::init()) return false;
	const auto& winSize = Director::getInstance()->getWinSize();

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square02.png"));
	bg->setContentSize({ 340.0, 220.0 });
	bg->setPosition(winSize / 2);
	this->_mainLayer->addChild(bg);

	auto rewardsLabel = Sprite::createWithSpriteFrameName("rewardsLabel_001.png");
	rewardsLabel->setPosition(winSize.width / 2, (winSize.height / 2 + 110.0) - 32.0);
	this->_mainLayer->addChild(rewardsLabel);

	for (int i = 0; i < 4; i++)
	{
		auto rewardsCorners = Sprite::createWithSpriteFrameName("rewardCorner_001.png");
		this->_mainLayer->addChild(rewardsCorners);

		float xPosition = 145.0f;
		float yPosition = 85.0f;
		switch (i)
		{
			case 0:
				yPosition = -85.0f;
				xPosition = -145.0f;
				break;
			case 1:
				xPosition = -145.0f;
				rewardsCorners->setFlippedY(true);
				break;
			case 2:
				rewardsCorners->setFlippedX(true);
				rewardsCorners->setFlippedY(true);
				break;
			case 3:
				yPosition = -85.0;
				rewardsCorners->setFlippedX(true);
				break;
			default:
				yPosition = 85.0;
		}
		rewardsCorners->setPosition(winSize.width / 2 + xPosition, winSize.height / 2 + yPosition);
	}

	auto closeBtnSprite = Sprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	auto closeBtn = MenuItemSpriteExtra::create(closeBtnSprite, [&](Node*) { this->close(); });
	closeBtn->setScaleMultiplier(1.6f);
	auto menu = Menu::create();
	closeBtn->setPosition(menu->convertToNodeSpace({(winSize.width / 2 - 170.0f) + 10.0f, (winSize.height / 2 + 110.0f) - 10.0f}));
	menu->addChild(closeBtn);

	auto freeStuffSprite = Sprite::createWithSpriteFrameName("GJ_freeStuffBtn_001.png");
	auto freeStuffBtn = MenuItemSpriteExtra::create(freeStuffSprite, [&](Node*) { /*call le function here*/ });
	freeStuffBtn->setScaleMultiplier(1.6f);
	freeStuffBtn->setPosition(menu->convertToNodeSpace({(winSize.width / 2 + 170.0f) - 10.0f, (winSize.height / 2 + 110.0f) - 10.0f}));
	menu->addChild(freeStuffBtn);

	Vec2 position = {winSize.width / 2, winSize.height / 2 - 24.0f};

	_dailyChestTimer1 = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "00:00:00");
	_dailyChestTimer1->setPosition(position.operator+({-70.0f, -50.0f}));
	_dailyChestTimer1->setScale(0.5f);
	_dailyChestTimer1->setVisible(false);
	this->_mainLayer->addChild(_dailyChestTimer1);

	_dailyChestTimer2 = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "00:00:00");
	_dailyChestTimer2->setPosition(position.operator+({70.0f, -50.0f}));
	_dailyChestTimer2->setScale(0.5f);
	_dailyChestTimer2->setVisible(false);
	this->_mainLayer->addChild(_dailyChestTimer2);

	_chestSprite1 = Sprite::createWithSpriteFrameName("chest_01_01_001.png");
	auto chestObject1 = MenuItemSpriteExtra::create(_chestSprite1, [&](Node*) {onChestClicked(1);});
	chestObject1->setPosition(menu->convertToNodeSpace(position.operator+({-70.0f, 14.0f - 10.05f})));
	chestObject1->setScaleMultiplier(1.08f);
	menu->addChild(chestObject1);

	_chestSprite2 = Sprite::createWithSpriteFrameName("chest_02_01_001.png");
	auto chestObject2 = MenuItemSpriteExtra::create(_chestSprite2, [&](Node*) {onChestClicked(2);});
	chestObject2->setScaleMultiplier(1.08f);
	chestObject2->setPosition(menu->convertToNodeSpace(position.operator+({70.0f, 14.0f})));

	menu->addChild(chestObject2);

	this->_mainLayer->addChild(menu);

	sendHttpRequest();

	return true;
}

void RewardsPage::onChestClicked(int chestID)
{
	if (_dailyChestTimer1->getString() == "00:00:00") return;

	Label* timer;
	RewardUnlockLayer* unlockLayer;

	if (chestID == 1)
	{
		if (_dailyChestTimer1->getString() != "Open")
		{
			timer = _dailyChestTimer1;
			// unlockLayer = RewardUnlockLayer::create(chestID); // temp
			// unlockLayer->show(kNone);
		}
		else
		{
			unlockLayer = RewardUnlockLayer::create(chestID);
			unlockLayer->show(kNone);
			return;
		}
	}
	else
	{
		if (_dailyChestTimer2->getString() != "Open")
		{
			timer = _dailyChestTimer2;
			// unlockLayer = RewardUnlockLayer::create(chestID); // temp
			// unlockLayer->show(kNone);
		}
		else
		{
			unlockLayer = RewardUnlockLayer::create(chestID);
			unlockLayer->show(kNone);
			return;
		}
	}

	this->stopAllActions();
	timer->setColor(ax::Color3B(255, 75, 0));
	auto ax = TintTo::create(1.0,0xff,0xff,0xff);
	timer->runAction(ax);
}

std::string formatSeconds(int seconds)
{
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int remainingSeconds = seconds % 60;

    std::string formattedTime;

    if (hours > 0)
    {
        formattedTime += fmt::format("{}h ", hours);
    }

    if (minutes > 0 || (hours == 0 && minutes == 0))
    {
        formattedTime += fmt::format("{}min", minutes);
    }

    if (hours == 0 && minutes == 0 && remainingSeconds > 0)
    {
        formattedTime = fmt::format("{}sec", remainingSeconds);
    }

    return formattedTime;
}

void RewardsPage::onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response)
{
	if (auto str = GameToolbox::getResponse(response))
	{
		std::string_view strResp {*str};

		auto decodedResponse = GameToolbox::xorCipher(base64_decode(fmt::format("{}", strResp.substr(5))), "59182");
		auto data = GameToolbox::splitByDelim(decodedResponse, ':');
		
		if (GameToolbox::stoi(data[5]) > 1)
		{
			_dailyChestTimer1->setString(formatSeconds(GameToolbox::stoi(data[5])));
		}
		else
		{
			_chestSprite1->setSpriteFrame("chest_01_02_001.png");
			_dailyChestTimer1->setString("Open");
		}
		
		if (GameToolbox::stoi(data[8]) > 1)
		{
			_dailyChestTimer2->setString(formatSeconds(GameToolbox::stoi(data[8])));
		}
		else
		{
			_chestSprite2->setSpriteFrame("chest_02_02_001.png");
			_dailyChestTimer2->setString("Open");
		}
		
		GameToolbox::log("RESPONSE: {}", strResp);
		GameToolbox::log("RESPONSE: {}", decodedResponse);

		_dailyChestTimer1->setVisible(true);
		_dailyChestTimer2->setVisible(true);
	}
}

void RewardsPage::sendHttpRequest()
{
	std::string postData = fmt::format("secret=Wmfd2893gb7&udid={}&chk={}&rewardType=0", "JUSTANORMALUDID", "5yQrSBA4DAQAH");
	GameToolbox::log("postData: {}", postData);
		
	auto _request = new ax::network::HttpRequest();
	_request->setUrl("http://www.boomlings.com/database/getGJRewards.php");
	_request->setRequestType(ax::network::HttpRequest::Type::POST);
	_request->setHeaders(std::vector<std::string>{"user-agent: "});
	_request->setRequestData(postData.c_str(), postData.length());
	_request->setResponseCallback(AX_CALLBACK_2(RewardsPage::onHttpRequestCompleted, this));
	_request->setTag("valid");
	ax::network::HttpClient::getInstance()->send(_request);
	_request->release();
}