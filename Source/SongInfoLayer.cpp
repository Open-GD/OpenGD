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

#include "SongInfoLayer.h"
#include <ui/UIScale9Sprite.h>

#include "LevelTools.h"
#include "MenuItemSpriteExtra.h"
#include "ButtonSprite.h"
#include "2d/Label.h"
#include "Application.h"
#include "2d/Menu.h"
#include "base/Director.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/nodes.h"
#include "fmt/format.h"

USING_NS_AX;

SongInfoLayer* SongInfoLayer::create(int soundtrackID)
{
	auto artistID = LevelTools::getArtistForAudio(soundtrackID);

	return SongInfoLayer::create(
		LevelTools::getAudioTitle(soundtrackID), LevelTools::getNameForArtist(artistID),
		LevelTools::getURLForAudio(soundtrackID), LevelTools::getNgURLForArtist(artistID),
		LevelTools::getYtURLForArtist(artistID), LevelTools::getFbURLForArtist(artistID)
	);
}

SongInfoLayer* SongInfoLayer::create(std::string_view songName, std::string_view artistName, std::string_view urlLink,
	std::string_view ngArtistLink, std::string_view ytArtistLink, std::string_view fbArtistLink)
{
	auto pRet = new SongInfoLayer();
	
	if (pRet && pRet->init(songName, artistName, urlLink, ngArtistLink, ytArtistLink, fbArtistLink))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool SongInfoLayer::init(std::string_view songName, std::string_view artistName, std::string_view urlLink,
	std::string_view ngArtistLink, std::string_view ytArtistLink, std::string_view fbArtistLink)
{
	if (!PopupLayer::init())
		return false;
	
	const auto& winSize = Director::getInstance()->getWinSize();

	_downloadLink = urlLink;
	_ngArtistLink = ngArtistLink;
	_ytArtistLink = ytArtistLink;
	_fbArtistLink = fbArtistLink;

	auto bg = ax::ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square01.png"), {0, 0, 80, 80});
	bg->setContentSize({ 420.0, 260.0 });
	bg->setPosition(winSize / 2);

	this->_mainLayer->addChild(bg);


	auto songLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), songName);
	songLabel->setPosition({ winSize.width / 2, (winSize.height / 2) + 90 });

	if (songLabel->getContentSize().width > 300.0f)
		songLabel->setScale(300 / songLabel->getContentSize().width);

	this->_mainLayer->addChild(songLabel);

	auto artistLabel = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), fmt::format("By {}", artistName));
	artistLabel->setPosition({ winSize.width / 2, (songLabel->getPosition().height + 4) - 30});

	if (artistLabel->getContentSize().width > 300.0f)
		artistLabel->setScale(300 / artistLabel->getContentSize().width);

	if (artistLabel->getScale() >= 0.8)
		artistLabel->setScale(0.8);

	this->_mainLayer->addChild(artistLabel);


	auto closeBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_closeBtn_001.png"), [&](Node*)
	{
		this->close();
	});
	auto closeBtnMenu = Menu::create();
	closeBtnMenu->addChild(closeBtn);

	closeBtnMenu->setPosition({ (winSize.width / 2) - 210 + 10, (winSize.height / 2) + 130 - 10 });
	
	this->_mainLayer->addChild(closeBtnMenu);

	auto dls = MenuItemSpriteExtra::create(ButtonSprite::create("Download Soundtrack", 0xDC, 0, 0.6, false, GameToolbox::getTextureString("bigFont.fnt"), GameToolbox::getTextureString("GJ_button_01.png"), 30), [&](Node*)
	{
		Application::getInstance()->openURL(_downloadLink);
	});
	dls->setPosition(closeBtnMenu->convertToNodeSpace(artistLabel->getPosition()) + Vec2(0, -50));
	closeBtnMenu->addChild(dls);

	auto moreByLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), fmt::format("More By {}", artistName));
	moreByLabel->setPosition(artistLabel->getPosition() + Vec2(0, -110));
	moreByLabel->setScale(0.5);
	this->_mainLayer->addChild(moreByLabel);

	ax::Vector<Node*> socialButtons;

	if (!ngArtistLink.empty())
	{
		auto ngButton = MenuItemSpriteExtra::create(ButtonSprite::create("Newgrounds", 0x50, 0, 0.6, false, GameToolbox::getTextureString("bigFont.fnt"), GameToolbox::getTextureString("GJ_button_01.png"), 30), [&](Node*)// TextButton::create("Newgrounds", GameToolbox::getTextureString("bigFont.fnt"), 96.000, 30, [&](TextButton*)
		{
			Application::getInstance()->openURL(_ngArtistLink);
		});
		socialButtons.pushBack(ngButton);
		closeBtnMenu->addChild(ngButton);
	}

	if (!ytArtistLink.empty())
	{
		auto ytButton = MenuItemSpriteExtra::create(ButtonSprite::create("YouTube", 0x50, 0, 0.6, false, GameToolbox::getTextureString("bigFont.fnt"), GameToolbox::getTextureString("GJ_button_01.png"), 30), [&](Node*)
		{
			Application::getInstance()->openURL(_ytArtistLink);
		});
		socialButtons.pushBack(ytButton);
		closeBtnMenu->addChild(ytButton);
	}

	if (!fbArtistLink.empty())
	{
		auto fbButton = MenuItemSpriteExtra::create(ButtonSprite::create("Facebook", 0x50, 0, 0.6, false, GameToolbox::getTextureString("bigFont.fnt"), GameToolbox::getTextureString("GJ_button_01.png"), 30), [&](Node*)
		{
			Application::getInstance()->openURL(_fbArtistLink);
		});
		socialButtons.pushBack(fbButton);
		closeBtnMenu->addChild(fbButton);
	}

	GameToolbox::alignItemsHorizontally(socialButtons, 20, closeBtnMenu->convertToNodeSpace({ winSize.width / 2, ((winSize.height / 2) - 130.0f) + 40.0f }));

	return true;
}
