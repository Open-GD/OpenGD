#include "ProfilePage.h"

#include <MenuItemSpriteExtra.h>
#include "GameToolbox.h"
#include <ui/CocosGUI.h>
#include "ButtonSprite.h"
#include "2d/CCMenu.h"
#include "ccUTF8.h"
#include "base/CCDirector.h"

USING_NS_AX;

ProfilePage* ProfilePage::create(int accountID, bool idk)
{
	ProfilePage* pRet = new ProfilePage();

	if (pRet && pRet->init(accountID, idk))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool ProfilePage::init(int accountID, bool idk)
{
	if (!PopupLayer::init()) return false;
	const auto& winSize = Director::getInstance()->getWinSize();

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square01.png"));
	bg->setContentSize({ 440.0, 290.0 });
	bg->setPosition(winSize / 2);

	this->_mainLayer->addChild(bg);

	auto playerName = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Hello World");
	playerName->setPosition({ winSize.width / 2, ((winSize.height / 2) + 145.0f) - 20.0f});

	this->_mainLayer->addChild(playerName);

	auto floorLine = Sprite::createWithSpriteFrameName("floorLine_001.png");
	floorLine->setPosition({ winSize.width / 2, bg->getPositionY() + 100.0f});
	floorLine->setScaleX(0.8f);
	floorLine->setOpacity(100);

	this->_mainLayer->addChild(floorLine);

	auto closeBtnSprite = Sprite::createWithSpriteFrameName("GJ_closeBtn_001.png");

	auto menu = Menu::create();
	
	//auto closeBtn = MenuItemSpriteExtra::create(closeBtnSprite, [&](Node*) { this->close(); });
	// TODO: add position
	//menu->addChild(closeBtn);

	this->_mainLayer->addChild(menu);

	return true;
}
