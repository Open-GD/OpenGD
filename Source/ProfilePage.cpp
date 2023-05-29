#include "ProfilePage.h"

#include <MenuItemSpriteExtra.h>
#include "GameToolbox.h"
#include <ui/CocosGUI.h>
#include "ButtonSprite.h"
#include "2d/CCMenu.h"
#include "ccUTF8.h"
#include "base/CCDirector.h"
#include "LoadingCircle.h"

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
	floorLine->setOpacity(100);
	floorLine->setScaleX(0.8f);
	floorLine->setPosition({ winSize.width / 2, bg->getPositionY() + 100.0f});

	this->_mainLayer->addChild(floorLine);

	// TODO: GJCommentListLayer position: -170.0f,-22.5f;

	auto closeBtnSprite = Sprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	auto closeBtn = MenuItemSpriteExtra::create(closeBtnSprite, [&](Node*) { this->close(); });

	auto menu = Menu::create();
	menu->setPosition((winSize.width / 2 - 220.0f) + 10.0f, ((winSize.y / 2) + 145.0f) - 10.0f );
	menu->addChild(closeBtn);

	auto errorMsg = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), "Something went wrong...");
	errorMsg->setScale(0.6f);
	errorMsg->setPosition({ winSize.width / 2, winSize.height / 2 - 53.0f});

	this->_mainLayer->addChild(errorMsg);

	if (true) // complete if statement here: if registered player id is not equal to ProfilePage player id?
	{
		const char* heartSprite = "gj_heartOn_001.png";
		
		if (true) // complete if statement here: if not following, FUN_004a4950 is the function that checks if the player is followed or not
		{
			heartSprite = "gj_heartOff_001.png";
		}
		auto heartObject = MenuItemSpriteExtra::create(heartSprite, [&](Node*) { /*Follow Function*/ });
		heartObject->setScaleMultiplier(1.1f);
		ax::Vec2 heartPosition = { (winSize.width / 2) - 198.0f, (winSize.height / 2) - 123.0f };
		heartObject->setPosition(menu->convertToNodeSpace(heartPosition));
		
		if (true) // complete if statement here: if not following
		{
			auto followTxtSprite = Sprite::createWithSpriteFrameName("GJ_followTxt_001.png");
			followTxtSprite->setPosition(heartPosition.operator+({48.0f,-2.0f}));
			
			this->_mainLayer->addChild(followTxtSprite);
		}

		menu->addChild(heartObject);
		
	}

	auto loadingcircle = LoadingCircle::create();
	loadingcircle->setPosition({ winSize.width / 2, winSize.height / 2 - 53.0f});
	this->_mainLayer->addChild(loadingcircle);

	auto refreshBtnSprite = Sprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
	auto refreshBtn = MenuItemSpriteExtra::create(refreshBtnSprite, [&](Node*) { /*Refresh Function*/ });

	refreshBtn->setPosition(menu->convertToNodeSpace({(winSize.width / 2 - 220.0f) + 10.0f, (winSize.height / 2 - 145.0f) + 10.0f + 1.0f}));
	refreshBtn->setScaleMultiplier(1.5f);

	menu->addChild(refreshBtn);

	this->_mainLayer->addChild(menu);

	return true;
}
