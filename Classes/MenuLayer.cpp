#include "MenuLayer.h"
/*
#include "GarageLayer.h"
*/

#include "MenuGameLayer.h"
#include "CreatorLayer.h"
#include "AlertLayer.h"
#include "AudioEngine.h"
#include "MenuItemSpriteExtra.h"
#include "GameToolbox.h"
#include "PlayLayer.h"
#include "LoadingCircle.h"
#include "SimpleProgressBar.h"
#include "LevelSelectLayer.h"
#include "DropDownLayer.h"
#include "GJMoreGamesLayer.h"

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

bool MenuLayer::init()
{
	if (!Layer::init()) return false;

	if (music)
	{
		AudioEngine::play2d("menuLoop.mp3", true, 0.2f);
		music = false;
	}
	addChild(MenuGameLayer::create(), -1);

	float offsetScale = 1.13F;
	auto winSize = Director::getInstance()->getWinSize();

	auto log_oSpr = Sprite::createWithSpriteFrameName("GJ_logo_001.png");
	log_oSpr->setStretchEnabled(false);
	//log_oSpr->setPosition({ winSize.width / 2, winSize.height - 100 });
	log_oSpr->setPosition({ winSize.width / 2.f, winSize.height - 50 });
	this->addChild(log_oSpr);
	auto playBtn = MenuItemSpriteExtra::create("GJ_playBtn_001.png", [&](Node* btn) {
		/* AudioEngine::stopAll();
		AudioEngine::play2d("playSound_01.ogg", false, 0.1f);
		auto scene = PlayLayer::scene(GJGameLevel::createWithMinimumData("My awesome level", "MikaKC", 5));
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene)); */
		auto scene = LevelSelectLayer::scene();
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});
	playBtn->getChildren().at(0)->setAnchorPoint({0.5, 0.5});
	playBtn->setPosition({ 0, 0 });
	//static_cast<ax::Sprite*>(playBtn->getSprite())->setStretchEnabled(false);

	auto garageBtn = MenuItemSpriteExtra::create("GJ_garageBtn_001.png", [&](Node* btn) {
		//Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GarageLayer::scene()));
	});

	garageBtn->setPosition({-110, 0});

	garageBtn->getChildren().at(0)->setAnchorPoint({0.5, 0.5});

	auto creatorBtn = MenuItemSpriteExtra::create("GJ_creatorBtn_001.png", [&](Node* btn) {
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, CreatorLayer::scene()));
	});

	creatorBtn->setPosition({110, 0});

	creatorBtn->getChildren().at(0)->setAnchorPoint({0.5, 0.5});

	auto mainButtonMenu = Menu::create(garageBtn, playBtn, creatorBtn, nullptr);

	mainButtonMenu->setPosition({ winSize.width / 2.f , winSize.height / 2.f + 10 });
	addChild(mainButtonMenu);

	auto robBtn = MenuItemSpriteExtra::create(
		"robtoplogo_small.png", [&](Node* btn) { Application::getInstance()->openURL("http://www.robtopgames.com"); });
	robBtn->setScale(1.f);

	auto otherMenu = Menu::create();
	otherMenu->setPosition({58, 24});
	otherMenu->addChild(robBtn);
	addChild(otherMenu);

	auto achievementsBtn = MenuItemSpriteExtra::create("GJ_achBtn_001.png", [&](Node* btn) {
		//AlertLayer::create("coming soon", "this feature has not been added yet!")->show();
		auto dropdownlayer = DropDownLayer::create("Achievements");
		dropdownlayer->showLayer();
	});
	achievementsBtn->setScale(1.f);
	//static_cast<ax::Sprite*>(achievementsBtn->getSprite())->setStretchEnabled(false);

	auto optionsBtn = MenuItemSpriteExtra::create("GJ_optionsBtn_001.png", [&](Node* btn) {
		auto dropdownlayer = DropDownLayer::create("Options");
		dropdownlayer->showLayer();
	});

	static_cast<ax::Sprite*>(optionsBtn->getSprite())->setStretchEnabled(false);

	auto statsBtn = MenuItemSpriteExtra::create("GJ_statsBtn_001.png", [&](Node* btn) {
		auto alert = AlertLayer::create("WIP!", "This feature is not yet supported!", "Close", "Click me!", NULL, NULL);
		alert->setBtn2Callback([=](TextButton*) {
			alert->close();
			AlertLayer::create(
				"Woah hello ;)", "apparently you can do this now. its pretty cool!", "Close", "My mood rn", NULL,
				[=](TextButton*) { Application::getInstance()->openURL("https://www.youtube.com/watch?v=XSsRrlM3tNg"); })
				->show();
		});
		alert->show();
	});
	//static_cast<ax::Sprite*>(statsBtn->getSprite())->setStretchEnabled(false);

	auto bottomMenu = Menu::create(achievementsBtn, optionsBtn, statsBtn, nullptr);

	bottomMenu->setPosition({ winSize.width / 2.f, 45 });
	//bottomMenu->setPositionY(100);
	bottomMenu->alignItemsHorizontallyWithPadding(5);

	this->addChild(bottomMenu);

	auto moreGamesBtn = MenuItemSpriteExtra::create("GJ_moreGamesBtn_001.png", [&](Node* btn) {
		auto moregames = GJMoreGamesLayer::create();
		addChild(moregames);
	});

	auto moreMenu = Menu::create();
	moreMenu->setPosition({ winSize.width - 45, 45 });
	moreMenu->addChild(moreGamesBtn);
	addChild(moreMenu);

	moreGamesBtn->setContentSize({74.25f, 63});
	//moreGamesBtn->getSprite()->setStretchEnabled(false);
	moreGamesBtn->getSprite()->setPosition({37.125f, 31.5f});
	moreGamesBtn->getSprite()->setScale(1.f);

	// auto lctest = LoadingCircle::create();
	// lctest->setVisible(false);
	// lctest->setPosition({winSize.width / 2, winSize.height / 2});
	// this->addChild(lctest, 1024);

	/*auto pbtest = SimpleProgressBar::create();
	pbtest->setPercentage(10.f);
	pbtest->setPosition({winSize.width / 2, winSize.height / 2});
	pbtest->setVisible(false);
	this->addChild(pbtest, 1024);
	*/

	return true;
}