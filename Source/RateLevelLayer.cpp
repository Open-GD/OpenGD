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

#include "RateLevelLayer.h"

#include <MenuItemSpriteExtra.h>

#include <ui/CocosGUI.h>
#include "ButtonSprite.h"
#include "2d/Menu.h"
#include "UTF8.h"
#include "base/Director.h"
#include "GameToolbox/getTextureString.h"

USING_NS_AX;

const Color3B SELECTED_COLOR = { 255, 255, 255 };
const Color3B UNSELECTED_COLOR = { 125, 125, 125 };

unsigned int RateLevelLayer::m_dSelectedDiff = 0;
MenuItemSpriteExtra* RateLevelLayer::m_pSubmitButton = nullptr;

RateLevelLayer* RateLevelLayer::create(int levelID)
{
	RateLevelLayer* pRet = new RateLevelLayer();

	if (pRet && pRet->init(levelID))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

void RateLevelLayer::selectRating(Node* btn)
{
	auto tag = btn->getTag();

	if (tag == m_dSelectedDiff)
		return;
	
	btn->setColor(SELECTED_COLOR);

	if (auto oldSelected = btn->getParent()->getChildByTag(m_dSelectedDiff))
		oldSelected->setColor(UNSELECTED_COLOR);
	else
	{
		static_cast<ButtonSprite*>(m_pSubmitButton->getSprite())->setColor(SELECTED_COLOR);
	}

	m_dSelectedDiff = tag;
}

bool RateLevelLayer::init(int)
{
	m_dSelectedDiff = 0;

	if (!PopupLayer::init()) return false;
	const auto& winSize = Director::getInstance()->getWinSize();

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square01.png"));
	bg->setContentSize({ 360, 180 });
	bg->setPosition(winSize / 2);

	this->_mainLayer->addChild(bg);

	auto label = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Rate Difficulty");
	label->setPosition({ winSize.width / 2, (winSize.height / 2) + 70.0f });

	this->_mainLayer->addChild(label);

	auto menu = Menu::create();

	for (unsigned int i = 1; i < 6; i++)
	{
		auto textureName = StringUtils::format("difficulty_%02d_btn_001.png", i);
		auto diffSprite = Sprite::createWithSpriteFrameName(textureName);

		auto diffSpriteExtra = MenuItemSpriteExtra::create(diffSprite, RateLevelLayer::selectRating);
		diffSpriteExtra->setColor(UNSELECTED_COLOR);
		diffSpriteExtra->setTag(i);
		diffSpriteExtra->setScale(1.2f);

		menu->addChild(diffSpriteExtra);
	}

	menu->alignItemsHorizontallyWithPadding(38);
	menu->setPosition(winSize / 2);
	this->_mainLayer->addChild(menu);

	auto btnMenu = Menu::create();
	btnMenu->setPosition({ (winSize.width / 2), (winSize.height / 2) - 65 });
	this->_mainLayer->addChild(btnMenu);

	auto cancelBtn = MenuItemSpriteExtra::create(ButtonSprite::create("Cancel"), [&](Node*) {
		this->close();
	});
	btnMenu->addChild(cancelBtn);


	auto submitBtn = MenuItemSpriteExtra::create(ButtonSprite::create("Submit"), [&](Node*) {});
	static_cast<ButtonSprite*>(submitBtn->getSprite())->setColor(UNSELECTED_COLOR);

	btnMenu->addChild(submitBtn);

	m_pSubmitButton = submitBtn;

	btnMenu->alignItemsHorizontallyWithPadding(15);

	return true;
}
