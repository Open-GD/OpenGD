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

#include "SongCell.h"

#include "ButtonSprite.h"
#include "LevelTools.h"
#include "ui/UIScale9Sprite.h"
#include "MenuItemSpriteExtra.h"
#include "SongInfoLayer.h"
#include "SongObject.h"
#include "2d/Label.h"
#include "2d/Menu.h"
#include "base/Director.h"
#include "GameToolbox/getTextureString.h"
#include "fmt/format.h"

USING_NS_AX;

SongCell* SongCell::create(SongObject* songInfo)
{
	auto pRet = new SongCell();

	if (pRet && pRet->init(songInfo))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

void SongCell::updateBGColor(int num)
{
	_bgColor = ax::LayerColor::create();
	if (num % 2 == 0) _bgColor->initWithColor({ 161, 88, 44, 255 });
	else _bgColor->initWithColor({ 191, 114, 62, 255 });
	_bgColor->setContentSize(this->getContentSize());
	_bgColor->setPosition(this->getPosition());
	this->addChild(_bgColor, -1);
}

bool SongCell::init(SongObject* songInfo)
{
	this->setContentSize({ 356, 60 });

	_layer = ax::Layer::create();
	_layer->setContentSize(this->getContentSize());
	this->addChild(_layer);

	const auto& contentSize = this->getContentSize();
	auto sID = songInfo->_songID;


	// Song info
	auto songName = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), fmt::format("{}", LevelTools::getAudioTitle(sID)));
	songName->setAnchorPoint({ 0.0, 0.5 });
	songName->setScale(0.7);
	songName->setPosition({ 10, (contentSize.height / 2) + 10 });

	if (songName->getContentSize().width > 240)
		songName->setScale(240 / songName->getContentSize().width);
	
	if (songName->getScale() >= 0.7)
		songName->setScale(0.7);

	auto artistName = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), fmt::format("By {}", LevelTools::getNameForArtist(LevelTools::getArtistForAudio(sID))));
	artistName->setAnchorPoint({ 0.0, 0.5 });
	artistName->setScale(0.7);
	artistName->setPosition(songName->getPosition() + Vec2(2, -21));

	if (artistName->getContentSize().width > 140)
		artistName->setScale(140 / artistName->getContentSize().width);

	if (artistName->getScale() >= 0.7)
		artistName->setScale(0.7);

	_layer->addChild(artistName);
	_layer->addChild(songName);


	// View button
	auto buttonSprite = ButtonSprite::create("View", 0x32, 0, 0.6, false, GameToolbox::getTextureString("bigFont.fnt"), GameToolbox::getTextureString("GJ_button_01.png"), 30);

	auto button = MenuItemSpriteExtra::create(buttonSprite, [sID](Node* btn)
	{
		SongInfoLayer::create(sID)->show();
	});

	auto viewBtnMenu = Menu::create();
	viewBtnMenu->setPosition({ (this->getContentSize().width - ((buttonSprite->getContentSize().width) / 2.f) - 9.f), this->getContentSize().height / 2 });
	viewBtnMenu->addChild(button);
	_layer->addChild(viewBtnMenu);


	// Outlines for the cell
	auto color = Color4B(0, 0, 0, 50);

	auto topLine = DrawNode::create();
	topLine->drawRect(Vec2(0, 0), Vec2(contentSize.width, 0), color);
	_layer->addChild(topLine, _layer->getLocalZOrder() + 1);

	auto bottomLine = DrawNode::create();
	bottomLine->drawRect(Vec2::ZERO, Vec2(contentSize.width, 0), color);
	bottomLine->setPosition(Vec2(0, contentSize.height));
	_layer->addChild(bottomLine, _layer->getLocalZOrder() + 1);


	return true;
}
