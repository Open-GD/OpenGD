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

#include "InfoLayer.h"

#include <core/ui/UIScale9Sprite.h>
#include "MenuItemSpriteExtra.h"
#include <ui/CocosGUI.h>
#include <ui/UITextField.h>
#include "external/base64.h"
#include "ButtonSprite.h"
#include "Director.h"
#include "2d/Menu.h"
#include "GJGameLevel.h"
#include "UTF8.h"
#include "GameToolbox/getTextureString.h"
#include <fmt/format.h>
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
	const auto& winSize = Director::getInstance()->getWinSize();

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square01.png"));
	bg->setContentSize({ 420.0, 260.0 });

	bg->setPosition(winSize / 2);

	this->_mainLayer->addChild(bg);

	auto closeBtnMenu = Menu::create();
	auto closeBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_closeBtn_001.png"), [&](Node*) {this->close();});
	closeBtnMenu->addChild(closeBtn);
	closeBtnMenu->setPosition({ 0, 0 });

	closeBtn->setPosition({ (winSize.width / 2) - 200.0f, (winSize.height / 2.0f) + 120.f});

	this->_mainLayer->addChild(closeBtnMenu);


	auto levelName = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), level->_levelName);
	levelName->setPosition({ winSize.width / 2, (winSize.height / 2) + 105.f });

	if (levelName->getContentSize().width > 300.0f)
	{
		levelName->setScale(300.0f / levelName->getContentSize().width);
	}

	this->_mainLayer->addChild(levelName);

	auto levelCreator = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), fmt::format("By {}", level->_levelCreator));
	levelCreator->setPosition({ winSize.width / 2, levelName->getPositionY() - 30.f });

	if (levelCreator->getContentSize().width > 300.0f)
	{
		levelCreator->setScale(300.0f / levelCreator->getContentSize().width);
	}
	if (levelCreator->getScale() > 0.8)
	{
		levelCreator->setScale(0.8);
	}

	this->_mainLayer->addChild(levelCreator);


	auto desc = level->_description.empty() ? "(No description provided)" : base64_decode(level->_description);
	
	auto descField = ax::ui::UICCTextField::createWithBMFont(GameToolbox::getTextureString("chatFont.fnt"), desc, TextHAlignment::CENTER, 380.f);
	descField->setPosition({ winSize.width / 2, (winSize.height / 2) + 10.f /*+ 20.f */});
	this->_mainLayer->addChild(descField, 4);

	auto moreBtnMenu = Menu::create();

	auto moreBtn = MenuItemSpriteExtra::create(ButtonSprite::create("More by this user", 0xDC, 0, 0.6, false, GameToolbox::getTextureString("bigFont.fnt"), GameToolbox::getTextureString("GJ_button_01.png"), 30), [](Node*) {});
	moreBtnMenu->addChild(moreBtn);
	moreBtnMenu->setPosition({ winSize.width / 2, (winSize.height / 2) - 70.f });

	this->_mainLayer->addChild(moreBtnMenu);
	
	// Level Info (ID, Version)
	auto versionLabel = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), StringUtils::format("Version: %i", level->_version));
	versionLabel->setPosition({ (winSize.width / 2) - 50.f, (winSize.height / 2) - 110.0f });
	versionLabel->setScale(0.6);
	this->_mainLayer->addChild(versionLabel);

	auto idLabel = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), StringUtils::format("ID: %i", level->_levelID));
	idLabel->setPosition({ (winSize.width / 2) + 50.f, (winSize.height / 2) - 110.0f });
	idLabel->setScale(0.6);
	this->_mainLayer->addChild(idLabel);

	return true;
}
