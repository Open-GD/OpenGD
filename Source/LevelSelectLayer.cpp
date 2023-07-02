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

#include "LevelSelectLayer.h"
#include "GJGameLevel.h"
#include "MenuItemSpriteExtra.h"
#include "AudioEngine.h"
#include "PlayLayer.h"

#include "MenuLayer.h"
#include "LevelPage.h"
#include "SongsLayer.h"
//#include "Checkbox.h"
#include "GroundLayer.h"
#include "BoomScrollLayer.h"
#include "2d/Menu.h"
#include "2d/Label.h"
#include "EventListenerKeyboard.h"
#include "2d/Transition.h"
#include "EventDispatcher.h"
#include "base/Director.h"
#include "GameToolbox/log.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/nodes.h"

USING_NS_AX;


Scene* LevelSelectLayer::scene(int page)
{
	auto scene = Scene::create();
	scene->addChild(LevelSelectLayer::create(page));
	return scene;
}

LevelSelectLayer* LevelSelectLayer::create(int page) {
	LevelSelectLayer* pRet = new LevelSelectLayer();
	if (pRet->init(page)) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool LevelSelectLayer::init(int page)
{

	if(!Layer::init()) return false;

	auto director = Director::getInstance();
	const auto& winSize = director->getWinSize();

	_background = Sprite::create("GJ_gradientBG.png");
	_background->setAnchorPoint({0.0f, 0.0f});
	addChild(_background, -2);

	_background->setScaleX((winSize.width + 10.0f) / _background->getTextureRect().size.width);
	_background->setScaleY((winSize.height + 10.0f) / _background->getTextureRect().size.height);
	_background->setPosition({-5.0f, -5.0f});
	_background->setColor({0x28, 0x7D, 0xFF});
	_ground = GroundLayer::create(1);
	_ground->setPositionY(-25.f);
	addChild(_ground, -1);

	GameToolbox::createCorners(this, false, false, true, true);
	
	constexpr auto levelData = std::to_array<std::tuple<const char*, const char*, int>>({
		{ "Stereo Madness", "Robtop", 1 },
		{ "Back on Track", "Robtop", 2 },
		{ "Polargeist", "Robtop", 3 },
		{ "Dry Out", "Robtop", 4 },
		{ "Base After Base", "Robtop", 5 },
		{ "Cant Let Go", "Robtop", 6 },
		{ "Jumper", "Robtop", 7 },
		{ "Time Machine", "Robtop", 8 },
		{ "Cycles", "Robtop", 9 },
		{ "xStep", "Robtop", 10 },
		{ "Clutterfunk", "Robtop", 11 },
		{ "Theory Of Everything", "Robtop", 12 },
		{ "Electroman Adventures", "Robtop", 13 },
		{ "Clubstep", "Robtop", 14 },
		{ "Electrodynamix", "Robtop", 15 },
		{ "Hexagon Force", "Robtop", 16 },
		{ "Blast Processing", "Robtop", 17 },
		{ "Theory Of Everything 2", "Robtop", 18 },
		{ "Geometrical Dominator", "Robtop", 19 },
		{ "Deadlocked", "Robtop", 20 },
		{ "Fingerdash", "Robtop", 21 },
		{ "Performance Test", "OpenGD", 22 }
	});
	
	//TODO: add getters on level page because they are actually owning the stuff

	std::vector<Layer*> layers;
	layers.reserve(levelData.size());
	
	for (const auto& [name, creator, id] : levelData)
	{
		auto level = GJGameLevel::createWithMinimumData(name, creator, id);
		layers.push_back(LevelPage::create(level));
	}
	_levelPages = layers;
	_bsl = BoomScrollLayer::create(layers, page);
	addChild(_bsl);

	auto btnMenu = Menu::create();
	addChild(btnMenu, 5);

	//bool controller = PlatformToolbox::isControllerConnected();
	bool controller = false;

	auto left =
		Sprite::createWithSpriteFrameName(controller ? "controllerBtn_DPad_Left_001.png" : "navArrowBtn_001.png");
	if (!controller) left->setFlippedX(true);

	MenuItemSpriteExtra* leftBtn = MenuItemSpriteExtra::create(left, [this](Node*) {
		_bsl->changePageLeft();
	});
	btnMenu->addChild(leftBtn);

	leftBtn->setPosition(btnMenu->convertToNodeSpace({ 25.0f, winSize.height / 2 }));

	auto right = Sprite::createWithSpriteFrameName(controller ? "controllerBtn_DPad_Right_001.png" : "navArrowBtn_001.png");

	MenuItemSpriteExtra* rightBtn = MenuItemSpriteExtra::create(right, [this](Node*) {
		_bsl->changePageRight();
	});
	btnMenu->addChild(rightBtn);

	rightBtn->setPosition(btnMenu->convertToNodeSpace({ winSize.width - 25.0f, winSize.height / 2 }));

	auto back = Sprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
		MenuItemSpriteExtra* backBtn =
			MenuItemSpriteExtra::create(back, []( Node const* btn) { 
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
	});
	
	//auto bglCheckbox = Checkbox::create("BaseGameLayer", [this](Node* btn, bool on)
	//{
	//	GameToolbox::log("on: {}", on);
	//	if (auto currentLevelPage = dynamic_cast<LevelPage*>(_bsl->_layers.at(_bsl->_currentPage)))
	//	{
	//		currentLevelPage->_openBGL = on;
	//	}
	//});
	Menu* backMenu = Menu::create();

	addChild(backMenu, 1);
	backMenu->addChild(backBtn);
	//backMenu->addChild(bglCheckbox);
	backMenu->setPosition({25.0f, winSize.height - 22.0f });
	//bglCheckbox->setPosition({backMenu->convertToNodeSpace({winSize.width / 2 + 15.0f, winSize.height - 40.0f})});

	auto infoMenu = Menu::create();
	addChild(infoMenu);

	Sprite* info = Sprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
	MenuItemSpriteExtra* infoBtn = MenuItemSpriteExtra::create(info, [](Node* btn) {

	});
	infoMenu->addChild(infoBtn, 1);

	infoMenu->setPosition({ winSize.width - 20.0f, winSize.height - 20.0f });


	auto dlLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Download the soundtracks");
	dlLabel->setScale(0.5);

	auto dlMenuItem = MenuItemSpriteExtra::create(dlLabel, [](Node*) {
		SongsLayer::create()->showLayer(true, false);
	});

	auto dlLabelMenu = Menu::create();
	dlLabelMenu->addChild(dlMenuItem);
	addChild(dlLabelMenu);
	dlLabelMenu->setPosition({ winSize.width / 2, 35 });


	auto listener = EventListenerKeyboard::create();

	// int currentlevel = 0;

	listener->onKeyPressed = [this](EventKeyboard::KeyCode code, Event const*)
	{
		using enum ax::EventKeyboard::KeyCode;
		if (code == KEY_ESCAPE)
		{
			auto scene = MenuLayer::scene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
			// GameToolbox::popSceneWithTransition(0.5f);
		}
		else if (code == KEY_LEFT_ARROW) {
			_bsl->changePageLeft();
		} else if (code == KEY_RIGHT_ARROW) {
			_bsl->changePageRight();
		} else if (code == KEY_SPACE)
		{
			if (auto currentLevelPage = dynamic_cast<LevelPage*>(_bsl->_layers.at(_bsl->_currentPage)))
			{
				currentLevelPage->onPlay(nullptr);
			}
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//if (controller) GameToolbox::addBackButton(this, backBtn);
	return true;
}

void LevelSelectLayer::onExit()
{
	if (LevelPage::replacingScene)
		return;

	for (const auto& page : _levelPages)
	{
		if (LevelPage* levelPage = dynamic_cast<LevelPage*>(page))
		{
			if (GJGameLevel* level = levelPage->_level; level) {
				delete level;
			}
		}
	}
	Layer::onExit();
}