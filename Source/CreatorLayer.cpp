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

#include "CreatorLayer.h"
#include <AudioEngine.h>
#include "LevelPage.h"
#include "GJGameLevel.h"
#include "LevelBrowserLayer.h"
#include "LevelEditorLayer.h"
#include "LevelInfoLayer.h"
#include "LevelSearchLayer.h"
#include "MenuItemSpriteExtra.h"
#include "MenuLayer.h"
#include "PlayLayer.h"
#include <network/HttpClient.h>
#include "2d/Menu.h"
#include "2d/Transition.h"
#include "EventListenerKeyboard.h"
#include "base/Director.h"
#include "EventDispatcher.h"
#include "GJSearchObject.h"
#include "SecretLayer2.h"

#include "GameToolbox/log.h"
#include "GameToolbox/nodes.h"
#include "GameToolbox/conv.h"
#include "platform/FileUtils.h"
#include <filesystem>


USING_NS_AX;

using namespace ax::network;

Scene* CreatorLayer::scene() {
	return CreatorLayer::create();
}

CreatorLayer* CreatorLayer::create()
{
	CreatorLayer* ret = new CreatorLayer();
	if (ret->init())
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

bool CreatorLayer::init()
{
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();
	const auto& winSize = director->getWinSize();

	GameToolbox::createBG(this);
	GameToolbox::createAllCorners(this);

	auto menu = Menu::create();

	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_03_001.png", [](Node*) {
		Director::getInstance()->replaceScene(TransitionFade::create(.5, MenuLayer::scene()));
	});
	backBtn->setPosition(menu->convertToNodeSpace({ 24.0, winSize.height - 23.0f }));
	menu->addChild(backBtn);

	Sprite* secretLock = Sprite::createWithSpriteFrameName("GJ_lock_open_001.png");
	MenuItemSpriteExtra* secretLockBtn = MenuItemSpriteExtra::create(secretLock, [](Node*) {
		Director::getInstance()->replaceScene(TransitionFade::create(.5, SecretLayer2::scene()));
	});
	menu->addChild(secretLockBtn);
	secretLockBtn->setPosition(menu->convertToNodeSpace({winSize.width - 23.0f, winSize.height - 24.0f})); // Wrong cuz the function doesnt want to decompile for some reason
	
	auto buttonTexture = [](int n) -> const char*
	{
		switch (n) 
		{
			default: return "GJ_createBtn_001.png"; 
			case 1: return "GJ_savedBtn_001.png";
			case 2: return "GJ_highscoreBtn_001.png";
			case 3: return "GJ_challengeBtn_001.png";
			case 4: return "GJ_dailyBtn_001.png";
			case 5: return "GJ_weeklyBtn_001.png";
			case 6: return "GJ_gauntletsBtn_001.png";
			case 7: return "GJ_featuredBtn_001.png";
			case 8: return "GJ_fameBtn_001.png";
			case 9: return "GJ_mapPacksBtn_001.png";
			case 10: return "GJ_searchBtn_001.png";
		}
	};
	
	auto onButtonsCallback = [this](Node* btn) -> void
	{
		GameToolbox::log("tag: {}", btn->getTag());
		switch (btn->getTag())
		{
			case 0:
			{
				#ifdef _WIN32
				auto fu = ax::FileUtils::getInstance();
				std::filesystem::path appdata(fu->getWritablePath());
				appdata = appdata.append("../GeometryDash/CCLocalLevels.dat");
				GameToolbox::log("gd save file: {}", appdata.string());
				
				if(!std::filesystem::exists(appdata)) return;
				
				std::string data = fu->getStringFromFile(appdata.string());
				if(data.empty()) return;
				data = GJGameLevel::decompressLvlStr(GameToolbox::xorFunction(data, 11));
				//GameToolbox::log("{}", data);
				
				//best xml parser ever made
				size_t pos = data.find("opengd");
				if(pos == std::string::npos) return;
				
				size_t startPos = data.find("H4sIAAAAAAAA", pos);
				if(startPos == std::string::npos) return;
				
				size_t endPos = data.find("</s>");
				if(endPos == std::string::npos) return;
				
				auto level = GJGameLevel::createWithMinimumData("OpenGD", "creator", 33);
				level->_levelString = data.substr(startPos, endPos - startPos);
				
				Director::getInstance()->pushScene(ax::TransitionFade::create(0.5f, PlayLayer::scene(level)));

				#endif
				return;
			}
			case 7: return  Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelBrowserLayer::scene(GJSearchObject::create(kGJSearchTypeFeatured))));
			case 8: return  Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelBrowserLayer::scene(GJSearchObject::create(kGJSearchTypeHallOfFame))));
			case 10: return Director::getInstance()->pushScene(TransitionFade::create(0.5f, LevelSearchLayer::scene()));
		}
	};
	
	for (int i = 0; i < 11; i++)
	{
		auto spr = Sprite::createWithSpriteFrameName(buttonTexture(i));
		spr->setScale(.85f);
		auto btn = MenuItemSpriteExtra::create(spr, onButtonsCallback);
		btn->setTag(i);
		Vec2 pos;
		if (i < 3)
		{
			// First row with 3 elements
			pos = { (i - 1) * 100.0f, 97.0f };
		}
		else
		{
			// Rows below with 4 elements each
			int row = (i - 3) / 4;
			int col = (i - 3) % 4;
			pos = { col * 100.0f, -row * 97.0f };
			pos.x -= 150.0f;
		}
		btn->setPosition(pos);
		menu->addChild(btn);
	}

	
	addChild(menu);
	
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed  = AX_CALLBACK_2(CreatorLayer::onKeyPressed, this);

	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

//TODO: add keybinds for other stuff
void CreatorLayer::onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event) {
	switch (keyCode) 
	{
	case EventKeyboard::KeyCode::KEY_BACK:
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
		break;
	default:
		break;
	}
}
