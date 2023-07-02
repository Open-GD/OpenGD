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

#include "RewardUnlockLayer.h"

#include <MenuItemSpriteExtra.h>

#include <ui/CocosGUI.h>
#include "ButtonSprite.h"
#include "2d/Menu.h"
#include "UTF8.h"
#include "base/Director.h"
#include "GameManager.h"
#include "GJUserScore.h"
#include "2d/ActionInterval.h"
#include <2d/ActionEase.h>
#include "2d/ActionInstant.h"
#include "AudioEngine.h"
#include "external/base64.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "CurrencyRewardLayer.h"
#include "fmt/format.h"
#include "GameToolbox/nodes.h"

#include "GameToolbox/network.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/log.h"
#include "GameToolbox/conv.h"

USING_NS_AX;

RewardUnlockLayer* RewardUnlockLayer::create(int chestID)
{
	RewardUnlockLayer* pRet = new RewardUnlockLayer();

	if (pRet && pRet->init(chestID))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool RewardUnlockLayer::init(int chestID)
{
	if (!PopupLayer::init()) return false;
	const auto& winSize = Director::getInstance()->getWinSize();

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square02.png"));
	bg->setContentSize({ 340.0, 220.0 });
	bg->setPosition(winSize / 2);
	bg->setOpacity(0);
	bg->runAction(FadeIn::create(0.2));
	this->_mainLayer->addChild(bg);

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
		rewardsCorners->setOpacity(0);
		rewardsCorners->runAction(FadeIn::create(0.2));
		rewardsCorners->setPosition(winSize.width / 2 + xPosition, winSize.height / 2 + yPosition);
	}

	float offset;

	if (chestID == 1)
	{
      offset = 0.8f;
    }
    else
	{
      offset = 1.0f;
    }

	Vec2 position = {winSize.width / 2, winSize.height / 2 - 20.0f};

	auto chestShadow = Sprite::createWithSpriteFrameName("chest_shadow_001.png");
	chestShadow->setPosition(position.operator+=({0.0f, offset * -48.0f}));
	chestShadow->setScale(2);
	chestShadow->setOpacity(0);

	this->_mainLayer->addChild(chestShadow);

	chestShadow->runAction(FadeTo::create(0.4, 90));
	chestShadow->runAction(EaseBounceOut::create(ScaleTo::create(1.0, (offset * 0.95f))));

	_chestObj = GJChestSprite::create(chestID);
	_chestObj->setPosition(position.operator+({0.0, winSize.height}));
	this->_mainLayer->addChild(_chestObj);

	_chestObj->runAction(EaseBounceOut::create(MoveTo::create(1.0, position.operator+({0.0, _chestObj->getChildren().front()->getContentSize().x / 2.5f})))); // these coords were hardcoded bc i couldnt figure them out
	_chestObj->runAction(Sequence::create(DelayTime::create(0.36f), CallFunc::create([=]() {AudioEngine::play2d("chestLand.ogg", false, 0.5f);}), nullptr));
	
	auto shake0 = DelayTime::create(1.2f);
	auto shake1 = MoveBy::create(0.05f, {-4.0f,0.0f});
	auto shake2 = MoveBy::create(0.05f, {4.0f,0.0f});
	auto shake3 = MoveBy::create(0.05f, {-4.0f,0.0f});
	auto shake4 = MoveBy::create(0.05f, {4.0f,0.0f});
	auto shake5 = MoveBy::create(0.05f, {-4.0f,0.0f});
	auto shake6 = MoveBy::create(0.05f, {4.0f,0.0f});
	auto shake7 = DelayTime::create(0.2f);
	auto shake8 = CallFunc::create([=]() { 
		AudioEngine::play2d("chestOpen01.ogg", false, 0.5f);
		_chestObj->switchState(3, false);
		sendHttpRequest(chestID);
		});
	
	_chestObj->runAction(Sequence::create(shake0, shake1, shake2, shake3, shake4, shake5, shake6, shake7, shake8, nullptr));
	
	auto closeBtnSprite = Sprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");
	_closeBtn = MenuItemSpriteExtra::create(closeBtnSprite, [&](Node*) { this->close(); });
	auto rewardBtnSprite = Sprite::createWithSpriteFrameName("GJ_rewardBtn_001.png");
	_rewardBtn = MenuItemSpriteExtra::create(rewardBtnSprite, [&](Node*) { this->close(); });

	_rewardBtn->setOpacity(0);
	_closeBtn->setVisible(false);

	auto menu = Menu::create();
	menu->addChild(_closeBtn);
	menu->addChild(_rewardBtn);
	this->addChild(menu);

	_closeBtn->setPosition(menu->convertToNodeSpace({(winSize.width / 2 - 170.0f) + 10.0f, winSize.height / 2 + 110.0f - 10.0f}));
	_rewardBtn->setPosition(menu->convertToNodeSpace({winSize.width / 2, winSize.height / 2 - 110.0f}));

	_errorMsg = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), "Something went wrong...");
	_errorMsg->setScale(0.6f);
	_errorMsg->setPosition({ winSize.width / 2, winSize.height / 2 - 90.0f});
	_errorMsg->setVisible(false);

	this->addChild(_errorMsg);

	return true;
}

void RewardUnlockLayer::playRewardEffect(GetGJRewards* rewards)
{
	std::map<int, int> array;
	if (rewards->orbs > 0) array[1] = rewards->orbs;
    if (rewards->diamonds > 0) array[3] = rewards->diamonds;
	if (rewards->key) array[9] = rewards->key;
	const auto& winSize = Director::getInstance()->getWinSize();

    _chestObj->switchState(4, false);

    _rewardBtn->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), FadeIn::create(1.0)));

	Vec2 position = {winSize.width / 2, winSize.height / 2 + 68.0f};
	Vec2 position1;
	Vec2 position2;
	Vec2 position3;
	Vec2 position4;
	Vec2 position5;

	switch (array.size())
	{
		case 1:
			position1 = position;
			break;
		case 2:
			position1 = position;
			position1.x -= 45;
			position2 = {position1.x + 90.f, position1.y};
			break;
		case 3:
			position1 = position;
			position1 = {position.x - 75.f, position.y - 10.f};
			position2 = {position.x, position.y + 10.f};
			position3 = {position.x + 75.f, position.y - 10.f};
			break;
	}

    int index = 1;
    for (const auto& entry : array)
    {
        int currencyID = entry.first;
        int currencyCount = entry.second;

        if (currencyCount != 0)
        {
            float delay = static_cast<float>(index) * 0.2f + 0.5f;
            switch (index)
			{
				case 1:
					showEarnedCurrency(currencyID, currencyCount, delay, position1);
					break;
				case 2:
					showEarnedCurrency(currencyID, currencyCount, delay, position2);
					break;
				case 3:
					showEarnedCurrency(currencyID, currencyCount, delay, position3);
					break;
			}
        }
        index++;
    }
	this->addChild(CurrencyRewardLayer::create(rewards->orbs, rewards->stars, rewards->diamonds));
}

void RewardUnlockLayer::showEarnedCurrency(int currencyID, int currencyCount, float delay, Vec2 position)
{
	std::string_view currencySpriteName;

	float scale = 0;
	float yoffset = 0;
	float xoffset = 0;

	switch (currencyID)
	{
		case 1:
      		currencySpriteName = "currencyOrbIcon_001.png";
			scale = 1.2f;
			break;
		case 3:
			currencySpriteName = "GJ_bigDiamond_001.png";
			scale = 0.65f;
			break;
		case 4:
			currencySpriteName = "fireShardBig_001.png";
			break;
		case 5:
			currencySpriteName = "iceShardBig_001.png";
			break;
		case 6:
			currencySpriteName = "poisonShardBig_001.png";
			break;
		case 7:
			currencySpriteName = "shadowShardBig_001.png";
			break;
		case 8:
			currencySpriteName = "lavaShardBig_001.png";
			break;
		case 9:
			currencySpriteName = "GJ_bigKey_001.png";
			scale = 0.9f;
			xoffset = 7.0f;
			yoffset = -0.5f;
			break;
	}

	const auto& winSize = Director::getInstance()->getWinSize();

	Sprite* currencySprite = Sprite::createWithSpriteFrameName(currencySpriteName);
	
    Label* label = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), fmt::format("+{}", currencyCount));

	Node* currencyNode = Node::create();

	this->addChild(currencyNode);

	currencyNode->addChild(label);
	currencyNode->addChild(currencySprite);
	currencyNode->setPosition(position);
	currencyNode->setScale(2.f);
	
	label->setOpacity(0);
	GameToolbox::limitLabelWidth(label, 40.0f, 0.7f, 0.0f);
	label->setPositionX(label->getPositionX()-label->getContentSize().x * 0.4f); // came up with this, not 100% accurate but it does the job
	currencySprite->setOpacity(0);
	currencySprite->setPositionX(15.f + xoffset);
	currencySprite->setPositionY(yoffset);
	currencySprite->setScale(scale);

    currencyNode->runAction(Sequence::create(DelayTime::create(delay), EaseBounceOut::create(ScaleTo::create(0.3f, 1.0f)), nullptr));
    currencySprite->runAction(Sequence::create(DelayTime::create(delay), FadeIn::create(0.3f), nullptr));
	label->runAction(Sequence::create(DelayTime::create(delay), FadeIn::create(0.3f), nullptr));
}

void RewardUnlockLayer::onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response, int chestID)
{
	if (auto str = GameToolbox::getResponse(response))
	{
		std::string_view strResp {*str};

		GetGJRewards* rewards = GetGJRewards::create();

		auto decodedResponse = GameToolbox::xorCipher(base64_decode(fmt::format("{}", strResp.substr(5))), "59182");
		auto data = GameToolbox::splitByDelim(decodedResponse, ':');
		
		GameToolbox::log("{}", (chestID == 1) ? data[6] : data[9]);
		auto chestData = GameToolbox::splitByDelim((chestID == 1) ? data[6] : data[9], ',');
		int orbs = GameToolbox::stoi(chestData[0]);
		int diamonds = GameToolbox::stoi(chestData[1]);
		int key = GameToolbox::stoi(chestData[3]);
		if (orbs > 0) rewards->orbs = orbs;
		if (diamonds > 0) rewards->diamonds = diamonds;
		if (key > 0) rewards->key = true;
		//add key and shards
		
		playRewardEffect(rewards);
		return;
	}
	_errorMsg->setVisible(true);
	_closeBtn->setVisible(true);
}

void RewardUnlockLayer::sendHttpRequest(int chestID)
{
	std::string postData = fmt::format("secret=Wmfd2893gb7&udid={}&chk={}&rewardType={}", "JUSTANORMALUDID", "5yQrSBA4DAQAH", chestID); // todo: udid
	GameToolbox::log("postData: {}", postData);
		
	auto _request = new ax::network::HttpRequest();
	_request->setUrl("http://www.boomlings.com/database/getGJRewards.php");
	_request->setRequestType(ax::network::HttpRequest::Type::POST);
	_request->setHeaders(std::vector<std::string>{"user-agent: "});
	_request->setRequestData(postData.c_str(), postData.length());
	_request->setResponseCallback(AX_CALLBACK_2(RewardUnlockLayer::onHttpRequestCompleted, this, chestID));
	_request->setTag("valid");
	ax::network::HttpClient::getInstance()->send(_request);
	_request->release();
}