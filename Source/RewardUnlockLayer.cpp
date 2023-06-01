#include "RewardUnlockLayer.h"

#include <MenuItemSpriteExtra.h>
#include "GameToolbox.h"
#include <ui/CocosGUI.h>
#include "ButtonSprite.h"
#include "2d/CCMenu.h"
#include "ccUTF8.h"
#include "base/CCDirector.h"
#include "GameManager.h"
#include "GJUserScore.h"
#include "2d/CCActionInterval.h"
#include <2d/CCActionEase.h>
#include "2d/CCActionInstant.h"
#include "AudioEngine.h"
#include "external/base64.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"

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
	_chestObj->runAction(Sequence::create(DelayTime::create(0.36f), CallFunc::create([=]() {AudioEngine::play2d("chestLand.ogg", false, 0.5f);}), 0));
	
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
	
	_chestObj->runAction(Sequence::create(shake0, shake1, shake2, shake3, shake4, shake5, shake6, shake7, shake8, 0));
	
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

void RewardUnlockLayer::playRewardEffect(getGJRewards* rewards)
{
	_chestObj->switchState(4, false);
	
  	_rewardBtn->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), FadeIn::create(1.0)));
}

void RewardUnlockLayer::onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response, int chestID)
{
	if (auto str = GameToolbox::getResponse(response))
	{
		std::string_view strResp {*str};

		getGJRewards* rewards;

		auto decodedResponse = GameToolbox::xorCipher(base64_decode(fmt::format("{}", strResp.substr(5))), "59182");
		auto data = GameToolbox::splitByDelim(decodedResponse, ':');
		
		playRewardEffect(rewards);
		return;
	}
	_errorMsg->setVisible(true);
	_closeBtn->setVisible(true);
}

void RewardUnlockLayer::sendHttpRequest(int chestID)
{
	std::string postData = fmt::format("secret=Wmfd2893gb7&udid={}&chk={}&rewardType=0", "HELLOWORLDROBTOP", "5yQrSBA4DAQAH"); // todo: udid
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