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
#include "CreatorLayer.h"

#include <iostream>
#include "LevelSearchLayer.h"
#include "MenuItemSpriteExtra.h"
#include <ui/CocosGUI.h>
#include "LevelBrowserLayer.h"
#include "2d/Menu.h"
#include "GJSearchObject.h"
#include "TextInputNode.h"
#include "2d/Transition.h"
#include "EventListenerKeyboard.h"
#include "EventDispatcher.h"
#include "UTF8.h"
#include "base/Director.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/nodes.h"
#include "GameToolbox/conv.h"
#include <fmt/format.h>


USING_NS_AX;

static const Color3B SELECTED_COLOR = { 255, 255, 255 };
static const Color3B UNSELECTED_COLOR = { 125, 125, 125 };

Menu* LevelSearchLayer::_difficultyMenu = nullptr;
Menu* LevelSearchLayer::_lengthFilter = nullptr;

// these are supposed to be stored in GameManager, but we dont have that yet.
std::vector<int> LevelSearchLayer::_selectedDifficulties = {};
std::vector<int> LevelSearchLayer::_selectedTimes = {};


Sprite* searchButton(std::string texture, std::string text, float scale, std::string icon)
{
	auto sprite = Sprite::createWithSpriteFrameName(texture);
	auto menu = Menu::create();

	const auto& content_size = sprite->getContentSize();
	auto label = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), text);
	label->setScale(scale);
	menu->addChild(label);

	if (!icon.empty())
	{
		auto icon_sprite = Sprite::createWithSpriteFrameName(icon);
		icon_sprite->setScale(1.1f);
		menu->addChild(icon_sprite);
	}

	menu->setPosition(content_size / 2);
	menu->alignItemsHorizontally();

	sprite->addChild(menu);

	return sprite;
}

Scene* LevelSearchLayer::scene()
{
	auto scene = Scene::create();
	scene->addChild(LevelSearchLayer::create());
	return scene;
}

bool LevelSearchLayer::init()
{
	if (!Layer::init()) return false;

	auto winSize = Director::getInstance()->getWinSize();

	GameToolbox::createBG(this);
	GameToolbox::createCorners(this, false, false, true, true);

	auto backBtnMenu = Menu::create();
	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [](Node*) {
		// Director::getInstance()->replaceScene(TransitionFade::create(.5, CreatorLayer::scene()));
		GameToolbox::popSceneWithTransition(0.5f);
	});

	backBtnMenu->addChild(backBtn);
	backBtnMenu->setPosition({ 24.0, winSize.height - 23.0f });
	this->addChild(backBtnMenu);


	// Search bar, search button
	Vec2 searchPos{ winSize.width / 2, winSize.height / 2 + 130.0f };

	auto searchBg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square02b_001.png"));
	searchBg->setOpacity(255);
	searchBg->setContentSize({ 365.0, 40.0 });
	searchBg->setPosition(searchPos);
	searchBg->setScale(1.0f);
	searchBg->setColor({ 0, 56, 141 });
	this->addChild(searchBg);

	searchBg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square02b_small.png"));
	searchBg->setOpacity(255);
	searchBg->setContentSize({ 210.0f, 30.0f });
	searchBg->setPosition({ searchPos.x - 73.0f, searchPos.y });
	searchBg->setScale(1.0f);
	searchBg->setColor({ 0, 39, 98 });
	this->addChild(searchBg);

	_searchObject = GJSearchObject::create();
	addChild(_searchObject);

	auto menuSearch = Menu::create();
	auto searchBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn02_001.png", "Search", 0.6, ""), AX_CALLBACK_1(LevelSearchLayer::onSearch, this));

	searchBtn->setPosition(menuSearch->convertToNodeSpace({ searchPos.x + 84.0f, searchPos.y }));
	menuSearch->addChild(searchBtn);

	auto searchProfileBtn = MenuItemSpriteExtra::create("GJ_longBtn05_001.png", AX_CALLBACK_1(LevelSearchLayer::onSearchProfile, this));
	searchProfileBtn->setPosition(menuSearch->convertToNodeSpace({ searchPos.x + 156.0f, searchPos.y }));
	menuSearch->addChild(searchProfileBtn);
	this->addChild(menuSearch);

	_searchField = TextInputNode::create(194.0f, 50.0f, GameToolbox::getTextureString("bigFont.fnt").c_str(), "Enter a level, user or id", 0x18);
	_searchField->setPosition({ searchPos.x - 174.0f, searchPos.y });
	_searchField->setAnchorPoint({ 0.0, 0.5 });

	auto sfTextField = _searchField->getTextField();
	sfTextField->setMaxLength(20);
	sfTextField->setMaxLengthEnabled(true);

	auto sfDisplayLabel = _searchField->getDisplayedLabel();
	sfDisplayLabel->setPositionX(0);
	sfDisplayLabel->setAnchorPoint({ 0, 0.5 });

	this->addChild(_searchField);


	// Quick Search
	Vec2 quickSearchPos{ winSize.width / 2, winSize.height / 2 + 28.0f };

	auto quickSearchLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Quick Search", TextHAlignment::CENTER);
	quickSearchLabel->setPosition({ quickSearchPos.x, quickSearchPos.y + 69.5f });
	quickSearchLabel->setScale(0.5f);
	this->addChild(quickSearchLabel);

	auto quickSearchBg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square02b_001.png"));
	quickSearchBg->setOpacity(255);
	quickSearchBg->setContentSize({ 365.0, 115.0 });
	quickSearchBg->setPosition(quickSearchPos);
	quickSearchBg->setScale(1.0f);
	quickSearchBg->setColor({ 0, 46, 117 });

	this->addChild(quickSearchBg);



	auto mostDownloadedBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn03_001.png", "Most Downloaded", 0.45, "GJ_sDownloadIcon_001.png"), [&](Node*) {
		_searchObject->_screenID = SearchType::kGJSearchTypeDownloaded;
	ax::Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelBrowserLayer::scene(_searchObject)));
		});

	auto mostLikedBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn03_001.png", "Most Liked", 0.6, "GJ_sLikeIcon_001.png"), [&](Node*) {
		_searchObject->_screenID = SearchType::kGJSearchTypeLiked;
	ax::Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelBrowserLayer::scene(_searchObject)));
		});

	auto trendingBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn04_001.png", "Trending", 0.5, "GJ_sTrendingIcon_001.png"), [&](Node*) {
		_searchObject->_screenID = SearchType::kGJSearchTypeTrending;
	ax::Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelBrowserLayer::scene(_searchObject)));
		});

	auto recentBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn04_001.png", "Recent", 0.5, "GJ_sRecentIcon_001.png"), [&](Node*) {
		_searchObject->_screenID = SearchType::kGJSearchTypeRecent;
	ax::Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelBrowserLayer::scene(_searchObject)));
		});

	auto magicBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn04_001.png", "Magic", 0.5, "GJ_sMagicIcon_001.png"), [&](Node*) {
		_searchObject->_screenID = SearchType::kGJSearchTypeMagic;
	ax::Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelBrowserLayer::scene(_searchObject)));
		});

	auto awardedBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn04_001.png", "Awarded", 0.5, "GJ_sStarsIcon_001.png"), [&](Node*) {
		_searchObject->_screenID = SearchType::kGJSearchTypeAwarded;
	ax::Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelBrowserLayer::scene(_searchObject)));
		});

	auto followedBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn04_001.png", "Followed", 0.5, "GJ_sFollowedIcon_001.png"), [&](Node*) {
		_searchObject->_screenID = SearchType::kGJSearchTypeFollowed;
	ax::Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelBrowserLayer::scene(_searchObject)));
		});

	auto friendsBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn04_001.png", "Friends", 0.5, "GJ_sFriendsIcon_001.png"), [&](Node*) {
		_searchObject->_screenID = SearchType::kGJSearchTypeFriends;
	ax::Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelBrowserLayer::scene(_searchObject)));
		});


	auto quickSearchMenu = Menu::create(mostDownloadedBtn, mostLikedBtn, trendingBtn, recentBtn, magicBtn, awardedBtn, followedBtn, friendsBtn, nullptr);
	quickSearchMenu->setPosition(quickSearchPos);

	mostDownloadedBtn->setPosition({ -90.5f, 36.0f });
	mostLikedBtn->setPosition({ 90.5f, 36.0f });

	trendingBtn->setPosition({ -119.0f, 0.0f });
	recentBtn->setPosition({ 0.0f, 0.0f });
	magicBtn->setPosition({ 119.0f, 0.0f });

	awardedBtn->setPosition({ -119.9f, -36.0f });
	followedBtn->setPosition({ 0.0f, -36.0f });
	friendsBtn->setPosition({ 119.0f, -36.0f });

	this->addChild(quickSearchMenu);

	Vec2 filtersPos{ winSize.width / 2, winSize.height / 2 - 77.0f };

	// Difficulty filters
	auto filterLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Filters", TextHAlignment::CENTER);
	filterLabel->setPosition({ filtersPos.x, filtersPos.y + 37.0f });
	filterLabel->setScale(0.5f);
	this->addChild(filterLabel);

	auto filterBg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square02b_001.png"));
	filterBg->setOpacity(255);
	filterBg->setContentSize({ 365.0, 50.0 });
	filterBg->setPosition(filtersPos);
	filterBg->setScale(1.0f);
	filterBg->setColor({ 0, 39, 98 });
	this->addChild(filterBg);

	_difficultyMenu = Menu::create();
	_difficultyMenu->setPosition(filtersPos);

	for (unsigned int i = 0; i < 8; i++)
	{
		auto texture_name = StringUtils::format("difficulty_%02d_btn_001.png", i);
		if (i == 7)
			texture_name = "difficulty_auto_btn_001.png";

		auto diff_sprite = Sprite::createWithSpriteFrameName(texture_name);

		auto diff_spriteextra = MenuItemSpriteExtra::create(diff_sprite, AX_CALLBACK_1(LevelSearchLayer::toggleDifficulty, this));
		diff_spriteextra->setColor(UNSELECTED_COLOR);
		diff_spriteextra->setTag(i);
		diff_spriteextra->setScale(0.8f);

		_difficultyMenu->addChild(diff_spriteextra);
	}

	_difficultyMenu->alignItemsHorizontallyWithPadding(11);

	_extraDemonsBtn = MenuItemSpriteExtra::create("GJ_plus2Btn_001.png", [this](Node*) {
		//DemonFilterSelectLayer
		});
	_extraDemonsBtn->setVisible(false);
	_extraDemonsBtn->setPosition(_difficultyMenu->convertToNodeSpace({ filtersPos.x + 202.5f, filtersPos.y }));
	_difficultyMenu->addChild(_extraDemonsBtn);

	this->addChild(_difficultyMenu);

	for (size_t tag : _selectedDifficulties) {
		_difficultyMenu->getChildByTag(tag)->setColor(SELECTED_COLOR);
	}


	// Length filter
	Vec2 lengthFilterPos{ winSize.width / 2, (winSize.height / 2) - 128.0f };

	auto lengthFilterBg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square02b_001.png"));
	lengthFilterBg->setOpacity(255);
	lengthFilterBg->setContentSize({ 365.0, 35.0 });
	lengthFilterBg->setPosition(lengthFilterPos);
	lengthFilterBg->setScale(1.0f);
	lengthFilterBg->setColor({ 0, 31, 79 });

	this->addChild(lengthFilterBg);

	_lengthFilter = Menu::create();

	auto timeIcon = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_timeIcon_001.png"), [&](Node*) {});
	timeIcon->setEnabled(false);

	_lengthFilter->addChild(timeIcon);
	for (int i = 0; i < 5; ++i)
	{
		const char* length = GameToolbox::levelLengthString(i);
		auto label = MenuItemLabel::create(Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), length, TextHAlignment::CENTER), AX_CALLBACK_1(LevelSearchLayer::toggleTime, this));
		label->setColor(UNSELECTED_COLOR);
		label->setScale(0.5f);
		label->setTag(i);
		_lengthFilter->addChild(label);

	}

	_starIcon = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_starsIcon_001.png"), [&](Node* btn)
		{
			_starIcon->setColor(_starIcon->getColor() == UNSELECTED_COLOR ? SELECTED_COLOR : UNSELECTED_COLOR);
		});
	_starIcon->setColor(UNSELECTED_COLOR);
	_lengthFilter->addChild(_starIcon);

	_lengthFilter->setPosition(lengthFilterPos);
	_lengthFilter->alignItemsHorizontallyWithPadding(15);

	this->addChild(_lengthFilter);

	for (size_t tag : _selectedTimes)
	{
		_lengthFilter->getChildByTag(tag)->setColor(SELECTED_COLOR);
	}

	auto listener = ax::EventListenerKeyboard::create();
	listener->onKeyPressed = [=](ax::EventKeyboard::KeyCode key, ax::Event*) {
		switch (key) {
		case ax::EventKeyboard::KeyCode::KEY_ESCAPE:
			GameToolbox::popSceneWithTransition(0.5f);
			break;
		default:
			break;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void LevelSearchLayer::onSearch(Node* btn)
{
	_searchObject->_searchQuery = _searchField->getTextField()->getString();
	_searchObject->_difficulty = "";
	_searchObject->_length = "";
	_searchObject->_screenID = SearchType::kGJSearchTypeSearch;

	for (int i = 0; i < _selectedDifficulties.size(); i++)
		_searchObject->_difficulty += fmt::format("{}{}", _selectedDifficulties[i], i == _selectedDifficulties.size() - 1 ? "" : ",");

	for (int i = 0; i < _selectedTimes.size(); i++)
		_searchObject->_length += fmt::format("{}{}", _selectedTimes[i], i == _selectedTimes.size() - 1 ? "" : ",");

	ax::Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelBrowserLayer::scene(_searchObject)));
}

void LevelSearchLayer::onSearchProfile(Node* btn)
{

}



void LevelSearchLayer::toggleTime(Ref* btn)
{
	auto label = dynamic_cast<MenuItemLabel*>(btn);

	int tag = label->getTag();
	bool selected = std::find(_selectedTimes.begin(), _selectedTimes.end(), tag) != _selectedTimes.end();
	if (selected)
	{
		std::erase(_selectedTimes, tag);
		label->setColor(UNSELECTED_COLOR);
	}
	else
	{
		label->setColor(SELECTED_COLOR);
		_selectedTimes.push_back(tag);
	}
}

void LevelSearchLayer::toggleDifficulty(Ref* ref)
{
	auto btn = dynamic_cast<ax::Node*>(ref);

	int tag = btn->getTag();

	bool naSelected = false;
	bool autoSelected = false;
	bool demonSelected = false;
	int deselectPos = 0;

	for (int i = 0; i < _selectedDifficulties.size(); ++i)
	{
		switch (_selectedDifficulties[i])
		{
			case 0:
				naSelected = true;
				deselectPos = i;
				break;
			case 6:
				demonSelected = true;
				deselectPos = i;
				break;
			case 7:
				autoSelected = true;
				deselectPos = i;
				break;
		}
	}

	bool deleselectNa = naSelected && tag != 0;
	bool deselectDemon = demonSelected && tag != 6;
	bool deselectAuto = autoSelected && tag != 7;

	if (deleselectNa || deselectDemon || deselectAuto)
	{
		_extraDemonsBtn->setVisible(false);
		_selectedDifficulties.erase(_selectedDifficulties.begin() + deselectPos);
		_difficultyMenu->getChildByTag(0)->setColor(UNSELECTED_COLOR);
		_difficultyMenu->getChildByTag(6)->setColor(UNSELECTED_COLOR);
		_difficultyMenu->getChildByTag(7)->setColor(UNSELECTED_COLOR);
	}

	auto selected = std::find(_selectedDifficulties.begin(), _selectedDifficulties.end(), tag) != _selectedDifficulties.end();

	if (selected)
	{
		if(tag == 6)
		_extraDemonsBtn->setVisible(false);

		std::erase(_selectedDifficulties, tag);
		btn->setColor(UNSELECTED_COLOR);
	}
	else
	{
		if (tag == 0 || tag == 6 || tag == 7)
		{
			_extraDemonsBtn->setVisible(tag == 6);
			for (int selectedTags : _selectedDifficulties) {
				_difficultyMenu->getChildByTag(selectedTags)->setColor(UNSELECTED_COLOR);
			}
			_selectedDifficulties.clear();
		}

		btn->setColor(SELECTED_COLOR);
		_selectedDifficulties.push_back(tag);
	}
}
