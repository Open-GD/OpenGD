#include "RewardsLayer.h"

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
#include "RewardUnlockLayer.h"

USING_NS_AX;

RewardsLayer* RewardsLayer::create()
{
	RewardsLayer* pRet = new RewardsLayer();

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool RewardsLayer::init()
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
	this->_mainLayer->addChild(_dailyChestTimer1);

	_dailyChestTimer2 = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "00:00:00");
	_dailyChestTimer2->setPosition(position.operator+({70.0f, -50.0f}));
	_dailyChestTimer2->setScale(0.5f);
	this->_mainLayer->addChild(_dailyChestTimer2);

	auto chestSprite1 = Sprite::createWithSpriteFrameName("chest_01_01_001.png");
	auto chestObject1 = MenuItemSpriteExtra::create(chestSprite1, [&](Node*) {onChestClicked(1);});
	chestObject1->setPosition(menu->convertToNodeSpace(position.operator+({-70.0f, 14.0f - 10.05f})));
	menu->addChild(chestObject1);

	auto chestSprite2 = Sprite::createWithSpriteFrameName("chest_02_01_001.png");
	auto chestObject2 = MenuItemSpriteExtra::create(chestSprite2, [&](Node*) {onChestClicked(2);});
	chestObject2->setPosition(menu->convertToNodeSpace(position.operator+({70.0f, 14.0f})));
	menu->addChild(chestObject2);

	this->_mainLayer->addChild(menu);
	return true;
}

void RewardsLayer::onChestClicked(int chestID)
{
	Label* timer;
	if (chestID == 1)
	{
		timer = _dailyChestTimer1;
	}
	else
	{
		timer = _dailyChestTimer2;
		RewardUnlockLayer::create(chestID)->show(kNone);
		return;
	}
	this->stopAllActions();
	timer->setColor(ax::Color3B(255, 75, 0));
	auto ax = TintTo::create(1.0,0xff,0xff,0xff);
	timer->runAction(ax);
}