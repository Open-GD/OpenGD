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

#include "MenuLayer.h"

#include "GarageLayer.h"
#include "MenuGameLayer.h"
#include "CreatorLayer.h"
#include "AlertLayer.h"
#include "AudioEngine.h"
#include "MenuItemSpriteExtra.h"

#include "PlayLayer.h"
#include "LoadingCircle.h"
#include "SimpleProgressBar.h"
#include "LevelSelectLayer.h"
#include "DropDownLayer.h"
#include "MoreGamesLayer.h"
#include "GameManager.h"
#include "OptionsLayer.h"
#include "EndLevelLayer.h"
#include "2d/Transition.h"
#include "2d/Menu.h"
#include "base/Director.h"
#include "ios/Application-ios.h"
#include "2d/Label.h"
#include "EventDispatcher.h"
#include "EventListenerKeyboard.h"
#include "ProfilePage.h"
#include "RewardsPage.h"
#include "GameToolbox/log.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/keyboard.h"

/*
#include "ColoursPalette.h"
#include "ListLayer.h"
#include "GJMoreGamesLayer.h"
*/

USING_NS_AX;

bool MenuLayer::music = true;

Scene* MenuLayer::scene()
{
	auto scene = Scene::create();
	scene->addChild(MenuLayer::create());
	return scene;
}

MenuLayer* MenuLayer::create() {
	MenuLayer* pRet = new MenuLayer();
	if (pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool MenuLayer::init()
{
	auto gm = GameManager::getInstance();

	// gm->set<std::string>("key1", "value");
	// gm->set<bool>("key2", true);
	// gm->set<int>("key3", 523423);
	
	// std::string key = gm->get<std::string>("key1");
	// bool otherKey = gm->get<bool>("key2");
	// int myKey = gm->get<int>("key3");
	
	// GameToolbox::log("key1: {}, key2: {}, key3: {}", key, otherKey, myKey);
	
	
	if (!Layer::init()) return false;

	if (music)
	{
		AudioEngine::play2d("menuLoop.mp3", true, 0.2f);
		music = false;
	}

	_mgl = MenuGameLayer::create();
	addChild(_mgl, -1);

	float offsetScale = 1.13F;
	const auto& winSize = Director::getInstance()->getWinSize();

	auto log_oSpr = Sprite::createWithSpriteFrameName("GJ_logo_001.png");
	log_oSpr->setStretchEnabled(false);
	//log_oSpr->setPosition({ winSize.width / 2, winSize.height - 100 });
	log_oSpr->setPosition({ winSize.width / 2.f, winSize.height - 50 });
	this->addChild(log_oSpr);
	auto playBtn = MenuItemSpriteExtra::create("GJ_playBtn_001.png", [&](Node* btn) {
		auto scene = LevelSelectLayer::scene(0);
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});
	playBtn->getChildren().at(0)->setAnchorPoint({0.5, 0.5});
	playBtn->setPosition({ 0, 0 });

	auto garageBtn = MenuItemSpriteExtra::create("GJ_garageBtn_001.png", [=](Node* btn) {
		gm->_openedGarage = true;
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, GarageLayer::scene()));
	});

	garageBtn->setPosition({-110, 0});
	garageBtn->getChildren().at(0)->setAnchorPoint({0.5, 0.5});

	auto creatorBtn = MenuItemSpriteExtra::create("GJ_creatorBtn_001.png", [=](Node* btn) {
		gm->_openedCreator = true;
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, CreatorLayer::scene()));
	});

	creatorBtn->setPosition({110, 0});

	creatorBtn->getChildren().at(0)->setAnchorPoint({0.5, 0.5});

	auto mainButtonMenu = Menu::create(garageBtn, playBtn, creatorBtn, nullptr);

	mainButtonMenu->setPosition({ winSize.width / 2.f , winSize.height / 2.f + 10 });
	addChild(mainButtonMenu);
	
	if(!gm->_openedGarage)
	{
		auto spr = Sprite::createWithSpriteFrameName("GJ_chrSel_001.png");
		spr->setPosition({-150.0f, -50.0f});
		mainButtonMenu->addChild(spr);
	}
	GameToolbox::log("gm->_openedCreator) {}", gm->_openedCreator);
	if(!gm->_openedCreator)
	{
		auto spr = Sprite::createWithSpriteFrameName("GJ_lvlEdit_001.png");
		spr->setPosition({150.0f, -50.0f});
		mainButtonMenu->addChild(spr);
	}


	auto rbBtn = MenuItemSpriteExtra::create("robtoplogo_small.png", [&](Node* btn) { Application::getInstance()->openURL("http://www.robtopgames.com"); });
	auto fbBtn = MenuItemSpriteExtra::create("gj_fbIcon_001.png", [&](Node* btn) { Application::getInstance()->openURL("http://www.robtopgames.com"); });
	auto twBtn = MenuItemSpriteExtra::create("gj_twIcon_001.png", [&](Node* btn) { Application::getInstance()->openURL("http://www.robtopgames.com"); });
	auto ytBtn = MenuItemSpriteExtra::create("gj_ytIcon_001.png", [&](Node* btn) { Application::getInstance()->openURL("http://www.robtopgames.com"); });
	
	rbBtn->setScale(0.8f);
	fbBtn->setScale(0.8f);
	twBtn->setScale(0.8f);
	ytBtn->setScale(0.8f);

	auto socialMenu = Menu::create(rbBtn, fbBtn, twBtn, ytBtn, nullptr);
	socialMenu->setPosition({50, 24});
	addChild(socialMenu);
	
	Vec2 fbPos {socialMenu->convertToNodeSpace({22, 55})};
	
	fbBtn->setPosition(fbPos);
	twBtn->setPosition({fbPos.x + 30.0f, fbPos.y});
	ytBtn->setPosition({fbPos.x + 60.0f, fbPos.y});
	


	auto achievementsBtn = MenuItemSpriteExtra::create("GJ_achBtn_001.png", [&](Node* btn) {
		//AlertLayer::create("coming soon", "this feature has not been added yet!")->show();
		//auto dropdownlayer = DropDownLayer::create(nullptr, "Achievements");
		//dropdownlayer->showLayer();
	});
	achievementsBtn->setScale(1.f);
	//static_cast<ax::Sprite*>(achievementsBtn->getSprite())->setStretchEnabled(false);

	auto optionsBtn = MenuItemSpriteExtra::create("GJ_optionsBtn_001.png", [&](Node* btn) {
		addChild(OptionsLayer::create());
	});
	
	auto statsBtn = MenuItemSpriteExtra::create("GJ_statsBtn_001.png", [&](Node* btn) {
		auto alert = AlertLayer::create("WIP!", "This feature is not yet supported!", "Close", "Click me!", NULL, NULL);
		alert->setBtn2Callback([=](Node*) {
			alert->close();
			AlertLayer::create(
				"Woah hello ;)", "apparently you can do this now. its pretty cool!", "Close", "My mood rn", NULL,
				[=](Node*) { Application::getInstance()->openURL("https://www.youtube.com/watch?v=XSsRrlM3tNg"); })
				->show();
		});
		alert->show();
	});
	
	auto ngButton = MenuItemSpriteExtra::create("GJ_ngBtn_001.png", [&](Node* btn) {});
	ngButton->setScale(0.93);
	//static_cast<ax::Sprite*>(statsBtn->getSprite())->setStretchEnabled(false);

	auto bottomMenu = Menu::create(achievementsBtn, optionsBtn, statsBtn, ngButton, nullptr);

	bottomMenu->setPosition({ winSize.width / 2.0f, 45 });
	//bottomMenu->setPositionY(100);
	bottomMenu->alignItemsHorizontallyWithPadding(5);

	this->addChild(bottomMenu);

	auto moreGamesBtn = MenuItemSpriteExtra::create("GJ_moreGamesBtn_001.png", [&](Node* btn) {
		this->addChild(MoreGamesLayer::create());
	});
	moreGamesBtn->setScale(.9f); //no setScale in 2.1 but oversized for some reason in opengd
	
	auto moreMenu = Menu::create(moreGamesBtn, nullptr);
	moreMenu->setPosition({ winSize.width - 43, 45 });
	addChild(moreMenu);
	
	Vec2 profilePos {mainButtonMenu->convertToNodeSpace({45.0f, 105.0f})};
	
	_profileBtn = MenuItemSpriteExtra::create("GJ_profileButton_001.png", [](Node*){ProfilePage::create(71, true)->show();});
	_profileBtn->setPosition(profilePos);
	_profileBtn->setScale(0.92f); //no setScale in 2.1 but oversized for some reason in opengd
	mainButtonMenu->addChild(_profileBtn);
	
	_profileLabel = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), "Player");
	_profileLabel->setPosition({profilePos.x + 2.0f, profilePos.y + 36.0f});
	_profileLabel->setScale(.8); //no setScale in 2.1 but oversized for some reason in opengd
	mainButtonMenu->addChild(_profileLabel);
	
	//TODO: add option to game manager when profile is done
	if(true)
	{
		auto spr = Sprite::createWithSpriteFrameName("GJ_viewProfileTxt_001.png");
		spr->setPosition({profilePos.x + 76.0f, profilePos.y - 1.0f});
		addChild(spr);
	}
	
	auto dailyRewardBtn = MenuItemSpriteExtra::create("GJ_dailyRewardBtn_001.png", [](Node*){RewardsPage::create()->show();});
	dailyRewardBtn->setPosition(bottomMenu->convertToNodeSpace({winSize.width - 40.0f, winSize.height / 2 + 20.0f}));
	bottomMenu->addChild(dailyRewardBtn);

	GameToolbox::onKeyDown(true, this, [&](EventKeyboard::KeyCode code, Event*)
	{
		if (code == EventKeyboard::KeyCode::KEY_SPACE)
		{
			auto scene = LevelSelectLayer::scene(0);
			Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
		} else if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			auto alert = AlertLayer::create("Quit Game", "Are you sure you want to Quit?", "Cancel", "Yes", nullptr, nullptr);
			alert->setBtn2Callback([](Node*)
			{
				GameManager::getInstance()->save();
				Director::getInstance()->end();
			});
			alert->show();
		}
	});


	return true;
}