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

#include "LevelCell.h"
#include "MenuItemSpriteExtra.h"
#include "ui/UIScale9Sprite.h"
#include "LevelInfoLayer.h"
#include "ButtonSprite.h"
#include "GJGameLevel.h"
#include "2d/Label.h"
#include "2d/Menu.h"
#include "fmt/core.h"
#include "LevelTools.h"
#include "2d/Transition.h"
#include "base/Director.h"
#include "ProfilePage.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/nodes.h"
#include "GameToolbox/conv.h"

USING_NS_AX;

LevelCell* LevelCell::create(GJGameLevel* level)
{
	LevelCell* ret = new LevelCell();

	if (ret && ret->init(level))
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		delete ret;
		ret = nullptr;
		return nullptr;
	}
}

void LevelCell::updateBGColor(int num)
{
	_bgColor = ax::LayerColor::create();

	if (num % 2 == 0)
		_bgColor->initWithColor({ 161, 88, 44, 255 });
	else
		_bgColor->initWithColor({ 191, 114, 62, 255 });

	_bgColor->setContentSize(this->getContentSize());

	this->addChild(_bgColor, -1);
}

bool LevelCell::init(GJGameLevel* level) {
	_layer = Layer::create();
	addChild(_layer);

	Vec2 size = { 356, 90 };
	setContentSize(size);

	Vec2 difficultySpritePos = { 26, size.height / 2 };
	std::string bigFontStr = GameToolbox::getTextureString("bigFont.fnt");
	bool isDailyOrWeekly = false;


	float coinYPos;

	if (level->_stars && level->_coins)
	{
		difficultySpritePos += {0, 14};
		coinYPos = -45.5;
	}
	else if (level->_coins || level->_stars)
	{
		difficultySpritePos += {0, 5};
		coinYPos = -31.5;
	}

	if (isDailyOrWeekly)
	{
		difficultySpritePos += {0, 10};
		coinYPos = -60;
	}


	// Difficulty sprite
	if (level->_epic == 1)
	{
		Sprite* epicSprite = Sprite::createWithSpriteFrameName("GJ_epicCoin_001.png");
		epicSprite->setPosition(difficultySpritePos);
		_layer->addChild(epicSprite, 2);
	}
	else if (level->_featureScore > 0)
	{
		Sprite* featureScore = Sprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
		featureScore->setPosition(difficultySpritePos);
		_layer->addChild(featureScore, 2);
	}

	Sprite* diffSprite = Sprite::createWithSpriteFrameName(GJGameLevel::getDifficultySprite(level));
	diffSprite->setPosition(difficultySpritePos);
	_layer->addChild(diffSprite, 2);


	// Stars
	if (level->_stars > 0) {
		Sprite* starSprite = Sprite::createWithSpriteFrameName("star_small01_001.png");
		starSprite->setPosition(difficultySpritePos + ax::Vec2(8, -30));
		_layer->addChild(starSprite);

		Label* starLabel = Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_stars));
		starLabel->setScale(0.4f);
		starLabel->setPosition(starSprite->getPosition() + ax::Vec2(-8, 0));
		starLabel->setAnchorPoint({ 1, 0.5 });
		_layer->addChild(starLabel);
	}


	// Diamonds (for daily and weekly)
	if (isDailyOrWeekly)
	{
		Sprite* diamondIcon = Sprite::createWithSpriteFrameName("diamond_small01_001.png");
		_layer->addChild(diamondIcon);

		auto awardedAmount = 12;
		auto totalAmount = 12;

		auto diamondLabel = Label::createWithBMFont(bigFontStr, fmt::format("{}/{}", awardedAmount, totalAmount));
		diamondLabel->setAnchorPoint({ 1, 0.5 });

		diamondLabel->setScale(0.35);

		diamondIcon->setPosition(diffSprite->getPosition() + Vec2(((diamondLabel->getContentSize().width * diamondLabel->getScale()) / 2) + 2, -44.5));
		diamondLabel->setPosition(diamondIcon->getPosition() + Vec2(-8, 0));

		_layer->addChild(diamondLabel);

		if (awardedAmount == totalAmount)
		{
			diamondLabel->setColor({ 100, 255, 255 });
		}
	}


	// Coins
	float coinXPos;

	for (size_t i = 0; i < level->_coins; i++)
	{
		if (i == 1)
		{
			if (level->_coins == 2)
				coinXPos = 5.0;
			else
				coinXPos = 0;
		}
		else if (i == 2)
		{
			coinXPos = 10;
		}
		else
		{
			if (i == 1)
				coinXPos = 0;
			else if (level->_coins == 2)
			{
				coinXPos = -5;
			}
			else if (level->_coins <= 2)
				coinXPos = 0;
			else
				coinXPos = -10;
		}

		Sprite* coin = Sprite::createWithSpriteFrameName("usercoin_small01_001.png");
		coin->setPosition(difficultySpritePos + Vec2(coinXPos, coinYPos));
		_layer->addChild(coin);

		auto collected = false;

		if (level->_verifiedCoins && !collected)
		{
			coin->setColor({ 165, 165, 165 });
		}
		else
		{
			if (collected)
				coin->setColor({ 255, 175, 75 });
			else
				coin->setColor({ 165, 113, 48 });
		}
	}


	// Level info: Level name
	Label* levelNameLabel = Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_levelName));
	GameToolbox::limitLabelWidth(levelNameLabel, 190.f, 0.8f, 0);

	levelNameLabel->setPosition({ 50, (size.height / 2) + 30 });
	levelNameLabel->setAnchorPoint({ 0, 0.5 });

	_layer->addChild(levelNameLabel);

	// Level info: Level song
	Label* songNameLabel = nullptr;

	if (level->_songID == 0)
	{
		songNameLabel = Label::createWithBMFont(bigFontStr, fmt::format("{}", LevelTools::getAudioTitle(level->_officialSongID)));
		songNameLabel->setColor(ax::Color3B(39, 206, 250));
	}
	else
	{
		songNameLabel = Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_songName));
		songNameLabel->setColor(ax::Color3B(255, 132, 211));
	}
	GameToolbox::limitLabelWidth(songNameLabel, 200.f, 0.45f, 0);

	songNameLabel->setPosition({ levelNameLabel->getPositionX() + 2, (size.height / 2) - 12});
	songNameLabel->setAnchorPoint({ 0, 0.5 });
	_layer->addChild(songNameLabel);


	// Level stats
	auto padding = 25;

	if (level->_stars)
		padding = 18;

	// Level stats: Length
	Sprite* lenSprite = Sprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
	lenSprite->setPosition({ levelNameLabel->getPositionX() + 9, 14 });
	lenSprite->setScale(0.6f);
	_layer->addChild(lenSprite, 2);

	Label* lenLabel = Label::createWithBMFont(bigFontStr, GameToolbox::levelLengthString(level->_length));
	GameToolbox::limitLabelWidth(lenLabel, 50.f, 0.4f, 0);

	lenLabel->setPosition({ lenSprite->getPositionX() + 10, 14 });
	lenLabel->setAnchorPoint({ 0, 0.5 });
	_layer->addChild(lenLabel);

	// Level stats: Downloads
	Sprite* dwnSprite = Sprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
	dwnSprite->setPosition({
		(lenLabel->getPosition().x + (lenLabel->getContentSize().width * lenLabel->getScale())) + padding, 14
	});
	dwnSprite->setScale(0.6f);
	_layer->addChild(dwnSprite, 2);

	Label* dwnLabel = Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_downloads));
	GameToolbox::limitLabelWidth(dwnLabel, 45.f, 0.4f, 0);

	dwnLabel->setPosition({ dwnSprite->getPositionX() + 10, 14 });
	dwnLabel->setAnchorPoint({ 0, 0.5 });
	_layer->addChild(dwnLabel);

	// Level stats: Likes
	Sprite* likeSprite;

	if (level->_likes < 0 || level->_likes < level->_dislikes)
		likeSprite = Sprite::createWithSpriteFrameName("GJ_dislikesIcon_001.png");
	else
		likeSprite = Sprite::createWithSpriteFrameName("GJ_likesIcon_001.png");

	likeSprite->setPosition({
		(dwnLabel->getPosition().x + (dwnLabel->getContentSize().width * dwnLabel->getScale())) + padding, 14
		});
	likeSprite->setScale(0.6f);
	_layer->addChild(likeSprite, 2);


	Label* likeLabel = Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_likes));
	GameToolbox::limitLabelWidth(likeLabel, 45.f, 0.4f, 0);

	likeLabel->setPosition({ likeSprite->getPositionX() + 10, 14 });
	likeLabel->setAnchorPoint({ 0, 0.5 });
	_layer->addChild(likeLabel);

	// Level stats: Orbs
	if (level->_stars > 1)
	{
		Sprite* manaSprite = Sprite::createWithSpriteFrameName("currencyOrbIcon_001.png");
		manaSprite->setPosition({
			(likeLabel->getPosition().x + (likeLabel->getContentSize().width * likeLabel->getScale())) + padding, 14
		});
		manaSprite->setScale(0.6f);
		_layer->addChild(manaSprite, 2);

		int awardedAmount = 0;
		int totalAmount = 0;

		Label* manaLabel;

		if (awardedAmount != totalAmount)
		{
			manaLabel = Label::createWithBMFont(bigFontStr, fmt::format("{}/{}", awardedAmount, totalAmount));
		}
		else
		{
			manaLabel = Label::createWithBMFont(bigFontStr, fmt::format("{}", totalAmount));
			manaLabel->setColor({ 100, 255, 255 });
		}

		GameToolbox::limitLabelWidth(manaLabel, 45.f, 0.4f, 0);

		manaLabel->setPosition({ manaSprite->getPositionX() + 10, 14 });
		manaLabel->setAnchorPoint({ 0, 0.5 });

		_layer->addChild(manaLabel);
	}


	// Menu: Creator Name, View/Play Button
	Sprite* buttonSprite;
	Menu* menu = Menu::create();

	if (isDailyOrWeekly)
	{
		buttonSprite = Sprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
		buttonSprite->setScale(0.7);
		MenuItemSpriteExtra* button = MenuItemSpriteExtra::create(buttonSprite, [](Node*) {});
		menu->addChild(button);
	}
	else
	{
		buttonSprite = ButtonSprite::create("View", 0x32, 0, 0.6, false, GameToolbox::getTextureString("bigFont.fnt"), GameToolbox::getTextureString("GJ_button_01.png"), 30);
		MenuItemSpriteExtra* button = MenuItemSpriteExtra::create(buttonSprite, [&, level](Node* btn)
		{
			ax::Director::getInstance()->pushScene((ax::TransitionFade::create(0.5f, LevelInfoLayer::scene(level))));
		});
		menu->addChild(button);
	}

	Vec2 menuPosition = {
		(this->getContentSize().width - ((buttonSprite->getContentSize().width) / 2.f) - 9.f) + ((isDailyOrWeekly) ? 18 : 0),
		this->getContentSize().height / 2
	};

	menu->setPosition(menuPosition);
	_layer->addChild(menu);

	Label* creatorNameLabel = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), fmt::format("By {}", level->_levelCreator));
	GameToolbox::limitLabelWidth(creatorNameLabel, 140.f, 0.7f, 0);

	if (level->_levelCreator == "-")
		creatorNameLabel->setColor(ax::Color3B(90, 255, 255)); // thanks gd colon

	auto creatorLabelMenuItem = MenuItemSpriteExtra::create(creatorNameLabel, [level](Node*) { ProfilePage::create(level->_playerID, false)->show(); });
	creatorLabelMenuItem->setPosition(
		menu->convertToNodeSpace(levelNameLabel->getPosition() + Point(((creatorNameLabel->getContentSize().width / 2) * creatorNameLabel->getScale()) + 2, -22))
	);
	menu->addChild(creatorLabelMenuItem);


	// Collab & High object icons
	auto levelBadgePosition = creatorNameLabel->getContentSize().width * creatorNameLabel->getScale() + 65;

	if (level->_copiedID > 0)
	{
		Sprite* collabSprite = Sprite::createWithSpriteFrameName("collaborationIcon_001.png");
		collabSprite->setPosition(levelBadgePosition, 52);
		_layer->addChild(collabSprite);
	}

	if (level->_objects > 40000)
	{
		Sprite* highObjectSprite = Sprite::createWithSpriteFrameName("highObjectIcon_001.png");
		highObjectSprite->setPosition(levelBadgePosition + (level->_copiedID > 0 ? 18 : 0), 52);
		_layer->addChild(highObjectSprite);
	}


	// Percentage / Checkmark
	bool isLevelCompleted = (level->_normalPercent >= 100);

	if (isLevelCompleted)
	{
		Sprite* checkIcon = Sprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
		checkIcon->setAnchorPoint({ 0, 0.5 });

		_layer->addChild(checkIcon);
		checkIcon->setScale(0.8);
		checkIcon->setPosition(levelNameLabel->getPosition() + Vec2((levelNameLabel->getContentSize().width * levelNameLabel->getScale()) + 10, -1));
	}
	else if (level->_normalPercent != 0)
	{
		Label* percentLabel = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), fmt::format("{}%", (int)level->_normalPercent));
		percentLabel->setScale(0.6);
		percentLabel->setAnchorPoint({ 0, 0.5 });
		percentLabel->setPosition(levelNameLabel->getPosition() + Vec2((levelNameLabel->getContentSize().width * levelNameLabel->getScale()) + 10, -1));
		_layer->addChild(percentLabel);
	}


	// Outlines for the cell
	Color4B color = Color4B(0, 0, 0, 0x4B);

	DrawNode* topLine = DrawNode::create();
	topLine->drawLine(Vec2::ZERO, Vec2(size.width, 0), color);
	_layer->addChild(topLine, _layer->getLocalZOrder() + 1);

	DrawNode* bottomLine = DrawNode::create();
	bottomLine->drawLine(Vec2::ZERO, Vec2(size.width, 0), color);
	bottomLine->setPosition(Vec2(0, size.height));
	_layer->addChild(bottomLine, _layer->getLocalZOrder() + 1);


	return true;
}