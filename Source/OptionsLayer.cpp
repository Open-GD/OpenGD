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

#include "OptionsLayer.h"
#include "ButtonSprite.h"
#include "GameManager.h"
#include "MenuItemSpriteExtra.h"
#include "2d/Menu.h"
#include "base/Director.h"
#include "DropDownLayer.h"
#include "AccountLoginLayer.h"
#include "TutorialLayer.h"

USING_NS_AX;

OptionsLayer* OptionsLayer::create()
{
	auto pRet = new(std::nothrow) OptionsLayer();

	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool OptionsLayer::init()
{
	
	auto scrollLayer = Layer::create();

	auto accountBtn = MenuItemSpriteExtra::create(ButtonSprite::create("Account"), [](Node*) {
		AccountLoginLayer::create()->show();
	});
	auto howToPlayBtn = MenuItemSpriteExtra::create(ButtonSprite::create("How to Play"), [this](Node*) {
		TutorialLayer::create()->show();
	});
	
	
	auto optionsBtn = MenuItemSpriteExtra::create(ButtonSprite::create("Options"), [](Node*) {
	//	GameManager::getInstance()->setQualityHigh();
	});


	auto rateBtn = MenuItemSpriteExtra::create(ButtonSprite::create("Rate"), [](Node*) {
	//	GameManager::getInstance()->setQualityHigh();
	});
	
	auto songsBtn = MenuItemSpriteExtra::create(ButtonSprite::create("Songs"), [](Node*) {
	//	GameManager::getInstance()->setQualityHigh();
	});

	auto helpBtn = MenuItemSpriteExtra::create(ButtonSprite::create("Help"), [](Node*) {
	//	GameManager::getInstance()->setQualityHigh();
	});

	auto menu = Menu::create(accountBtn, howToPlayBtn, optionsBtn, rateBtn, songsBtn, helpBtn, nullptr);

	scrollLayer->addChild(menu);
	const auto& winSize = Director::getInstance()->getWinSize();
	//positions not accurate to original
	menu->setPosition({-10.0f, 80.0f});

	accountBtn->setPosition({-79.0f, 0.0f});
	howToPlayBtn->setPosition({79.0f, 0.0f});
	optionsBtn->setPosition({-89.0f, -80.0f});
	rateBtn->setPosition({-105.0f, -40.0f});
	songsBtn->setPosition({0.0f, -40.0f});
	helpBtn->setPosition({105.0f, -40.0f});
	

	auto dropdownlayer = DropDownLayer::create(scrollLayer, "Settings");
	dropdownlayer->showLayer();
	return true;
}
