#include "CreatorLayer.h"
#include <AudioEngine.h>
#include "LevelPage.h"
#include "GJGameLevel.h"
#include "GameToolbox.h"
#include "LevelBrowserLayer.h"
#include "LevelEditorLayer.h"
#include "LevelInfoLayer.h"
#include "LevelSearchLayer.h"
#include "MenuItemSpriteExtra.h"
#include "MenuLayer.h"
#include "PlayLayer.h"
#include <network/HttpClient.h>
#include "2d/CCMenu.h"
#include "2d/CCTransition.h"
#include "CCEventListenerKeyboard.h"
#include "CCDirector.h"
#include "CCEventDispatcher.h"
#include "GJSearchObject.h"

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
			// case 0:
			// {
				// auto scene = LevelEditorLayer::scene(GJGameLevel::createWithMinimumData("Unnamed 0", "partur", 5));
				// return Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
			// }
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
    }
}
