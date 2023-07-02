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

#include "AccountLoginLayer.h"

#include "MenuItemSpriteExtra.h"
#include "GameManager.h"
#include "ButtonSprite.h"
#include "2d/Menu.h"
#include "2d/Label.h"
#include "UTF8.h"
#include "base/Director.h"
#include "ui/UIScale9Sprite.h"
#include "LoadingCircle.h"
//#include "GJUserScore.h"
//#include "network/HttpResponse.h"
//#include "network/HttpClient.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/nodes.h"

USING_NS_AX;

AccountLoginLayer* AccountLoginLayer::create()
{
	AccountLoginLayer* pRet = new AccountLoginLayer();

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool AccountLoginLayer::init()
{
	if(!PopupLayer::init()) {
		return false;
	}
	
	auto dir = Director::getInstance();
	const auto& winSize = dir->getWinSize();

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square01.png"));
	bg->setContentSize({ 400.0f, 300.0f });
	ax::Vec2 winSizeHalf { winSize / 2 };
	bg->setPosition(winSizeHalf);
	_mainLayer->addChild(bg);

	std::string bigFontStr = GameToolbox::getTextureString("bigFont.fnt");
	auto loginLabel = Label::createWithBMFont(bigFontStr, "Login");
	loginLabel->setScale(0.6f);
	loginLabel->setPosition({ winSizeHalf.x, (winSizeHalf.y + 150) - 16.0f });
	_mainLayer->addChild(loginLabel);
	return true;

	auto cancelBtn = MenuItemSpriteExtra::create(ButtonSprite::create("Cancel"), [](Node*) {
	
	});
	
	auto loginBtn = MenuItemSpriteExtra::create(ButtonSprite::create("Login"), [](Node*) {
	
	});

	Menu* menu = Menu::create(cancelBtn, loginBtn, nullptr);
	float y = (winSizeHalf.y - 150.0f) + 22.0f;
    ax::Vec2 pos  {winSizeHalf.x, y + _mainLayer->getPositionY()};
    GameToolbox::alignItemsHorizontally(menu->getChildren(), 15.0f, menu->convertToNodeSpace(pos));

    _mainLayer->addChild(menu);
    
}
