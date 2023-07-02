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

#include "AlertLayer.h"
#include "ButtonSprite.h"
#include "MenuItemSpriteExtra.h"

#include "2d/Menu.h"
#include "2d/Label.h"
#include "ui/UIScale9Sprite.h"
#include "base/Director.h"

#include <string>
#include "GameToolbox/getTextureString.h"

USING_NS_AX;

bool AlertLayer::init(std::string_view title, std::string_view desc, std::string_view btn1Str, std::string_view btn2Str, float width, std::function<void(Node*)> btn1Callback, std::function<void(Node*)> btn2Callback)
{
	if (!PopupLayer::init()) 
		return false;

	const auto& winSize = Director::getInstance()->getWinSize();

	auto descLabel = Label::createWithBMFont(GameToolbox::getTextureString("chatFont.fnt"), desc, TextHAlignment::CENTER);
	width = descLabel->getContentSize().width + 32;
	// descLabel->setAnchorPoint({0.5, 1});
	descLabel->setDimensions(width, 0);
	descLabel->setPosition(Point(winSize / 2));
	this->_mainLayer->addChild(descLabel);
	auto descHeight = std::max(descLabel->getContentSize().height, 150.f);

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square01_001.png"));
	bg->setStretchEnabled(true);
	bg->setContentSize({width, descHeight});
	bg->setPosition(winSize / 2);
	this->_mainLayer->addChild(bg, -1);

	std::string goldFontStr = GameToolbox::getTextureString("goldFont.fnt");
	
	auto titleLabel = Label::createWithBMFont(goldFontStr, title);
	titleLabel->setAnchorPoint({0.5, 1.0});
	titleLabel->setPosition({winSize.width / 2, (winSize.height - descHeight) / 2 + descHeight - 15});
	titleLabel->setScale(0.9f);
	this->_mainLayer->addChild(titleLabel);

	auto menu = Menu::create();
	this->_mainLayer->addChild(menu);
	menu->setPositionY((winSize.height - descHeight) / 2 + 60);
	
	auto _btnCallback = btn1Callback ? btn1Callback : [this](Node*) {this->close(); };
	
	_btn1 = MenuItemSpriteExtra::create(ButtonSprite::create(btn1Str, 0x60, 0, 1), _btnCallback);
	menu->addChild(_btn1);

	if (!btn2Str.empty())
	{
		auto _btn2Callback = btn2Callback ? btn2Callback : [this](Node*) {this->close(); };
		_btn2 = MenuItemSpriteExtra::create(ButtonSprite::create(btn2Str, 0x60, 0, 1), _btn2Callback);
		menu->addChild(_btn2);

		menu->alignItemsHorizontallyWithPadding(MIN((width - (_btn1->getContentSize().width + _btn2->getContentSize().width)) / 2, 30));
	}
	menu->setPositionY(descHeight - 32);

	return true;
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc, std::string_view btn1, std::string_view btn2, float width, std::function<void(Node*)> btn1Callback, std::function<void(Node*)> btn2Callback)
{
	auto pRet = new(std::nothrow) AlertLayer();

	if (pRet && pRet->init(title, desc, btn1, btn2, width, btn1Callback, btn2Callback))
	{
		pRet->autorelease();
		return pRet;
	} 
	else 
	{
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc, std::string_view btn1, std::string_view btn2, std::function<void(Node*)> btn1Callback, std::function<void(Node*)> btn2Callback) {
	return AlertLayer::create(title, desc, btn1, btn2, 500, btn1Callback, btn2Callback);
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc, std::string_view btn1, float width, std::function<void(Node*)> btn1Callback) {
	return AlertLayer::create(title, desc, btn1, "", width, btn1Callback, nullptr);
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc, std::string_view btn1, std::function<void(Node*)> btn1Callback) {
	return AlertLayer::create(title, desc, btn1, "", 500, btn1Callback, nullptr);
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc, float width) {
	return AlertLayer::create(title, desc, "OK", "", width, nullptr, nullptr);
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc) {
	return AlertLayer::create(title, desc, "OK", "", 500, nullptr, nullptr);
}

void AlertLayer::setBtn1Callback(std::function<void(Node*)> btn1Callback)
{
	_btn1->setCallback(btn1Callback);
}

void AlertLayer::setBtn2Callback(std::function<void(Node*)> btn2Callback)
{
	_btn2->setCallback(btn2Callback);
}