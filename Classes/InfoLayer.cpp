#include "InfoLayer.h"

#include <core/ui/UIScale9Sprite.h>
#include "MenuItemSpriteExtra.h"
#include "TextButton.h"
#include <ui/CocosGUI.h>
#include <ui/UITextField.h>

USING_NS_AX;

InfoLayer* InfoLayer::create(GJGameLevel* level)
{
	InfoLayer* pRet = new InfoLayer();

	if (pRet->init(level))
	{
		pRet->autorelease();
		return pRet;
	}
	
	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool InfoLayer::init(GJGameLevel* level)
{
	if (!PopupLayer::init()) return false;
	auto winSize = Director::getInstance()->getWinSize();

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square01.png"));
	bg->setContentSize({ 420.0, 260.0 });

	bg->setPosition(winSize / 2);

	this->m_pMainLayer->addChild(bg);

	auto closeBtnMenu = Menu::create();
	auto closeBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_closeBtn_001.png"), [&](Node*) {this->close();});
	closeBtnMenu->addChild(closeBtn);
	closeBtnMenu->setPosition({ 0, 0 });

	closeBtn->setPosition({ (winSize.width / 2) - 200.0f, (winSize.height / 2.0f) + 120.f});

	this->m_pMainLayer->addChild(closeBtnMenu);


	auto levelName = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), level->_LevelName);
	levelName->setPosition({ winSize.width / 2, (winSize.height / 2) + 105.f });

	if (levelName->getContentSize().width > 300.0f)
	{
		levelName->setScale(300.0f / levelName->getContentSize().width);
	}

	this->m_pMainLayer->addChild(levelName);

	auto levelCreator = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), StringUtils::format("By %s", level->_LevelCreator));
	levelCreator->setPosition({ winSize.width / 2, levelName->getPositionY() - 30.f });

	if (levelCreator->getContentSize().width > 300.0f)
	{
		levelCreator->setScale(300.0f / levelCreator->getContentSize().width);
	}
	if (levelCreator->getScale() > 0.8)
	{
		levelCreator->setScale(0.8);
	}

	this->m_pMainLayer->addChild(levelCreator);


	auto desc = level->_Description.empty() ? "(No description provided)" : level->_Description;
	
	auto descField = ax::ui::UICCTextField::createWithBMFont(GameToolbox::getTextureString("chatFont.fnt"), desc, TextHAlignment::CENTER, 380.f);
	descField->setPosition({ winSize.width / 2, (winSize.height / 2) + 10.f /*+ 20.f */});
	this->m_pMainLayer->addChild(descField, 4);

	auto moreBtnMenu = Menu::create();

	auto moreBtn = TextButton::create("More by this user", GameToolbox::getTextureString("bigFont.fnt"), 220.f, 30.f, [](TextButton*) {});
	moreBtnMenu->addChild(moreBtn);
	moreBtnMenu->setPosition({ winSize.width / 2, (winSize.height / 2) - 70.f });

	this->m_pMainLayer->addChild(moreBtnMenu);
	
	// Level Info (ID, Version)
	auto versionLabel = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), StringUtils::format("Version: %i", level->_Version));
	versionLabel->setPosition({ (winSize.width / 2) - 50.f, (winSize.height / 2) - 110.0f });
	versionLabel->setScale(0.6);
	this->m_pMainLayer->addChild(versionLabel);

	auto idLabel = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), StringUtils::format("ID: %i", level->_LevelID));
	idLabel->setPosition({ (winSize.width / 2) + 50.f, (winSize.height / 2) - 110.0f });
	idLabel->setScale(0.6);
	this->m_pMainLayer->addChild(idLabel);

	return true;
}
