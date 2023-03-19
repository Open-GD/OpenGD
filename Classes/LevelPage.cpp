#include "LevelPage.h"
#include "MenuItemSpriteExtra.h"
#include "core/ui/UIScale9Sprite.h"
#include "PlayLayer.h"
#include <AudioEngine.h>
bool LevelPage::replacingScene = false;

ax::Scene* LevelPage::scene(GJGameLevel* level)
{
	auto scene = ax::Scene::create();
	auto winSize = ax::Director::getInstance()->getWinSize();
	ax::Sprite* background;

	background = ax::Sprite::create("GJ_gradientBG.png");
	background->setAnchorPoint({0.0f, 0.0f});
	scene->addChild(background, -2);

	background->setScaleX((winSize.width + 10.0f) / background->getTextureRect().size.width);
	background->setScaleY((winSize.height + 10.0f) / background->getTextureRect().size.height);
	background->setPosition({-5.0f, -5.0f});
	background->setColor({0x28, 0x7D, 0xFF});

	scene->addChild(LevelPage::create(level));
	return scene;
}


bool LevelPage::init(GJGameLevel* level)
{
	if (!Layer::init()) return false;

	auto winSize = ax::Director::getInstance()->getWinSize();

	auto normalBar = ax::Sprite::create("GJ_progressBar_001-hd.png");
	normalBar->setPosition({ winSize.width / 2, winSize.height / 2.f - 30 });
	normalBar->setColor({0, 0, 0});
	normalBar->setOpacity(125);
	addChild(normalBar, 3);

	auto normalProgress = ax::Sprite::create("GJ_progressBar_001-hd.png");
	normalProgress->setPosition({1.36f, 10});
	normalProgress->setColor({0, 255, 0});
	normalProgress->setOpacity(255);
	normalProgress->setAnchorPoint({0, 0.5});
	normalProgress->setContentSize({340 / (level->_normalPercent / 100.f), 20});
	normalProgress->setScale(0.992f);
	normalProgress->setScaleX(0.992f);
	normalProgress->setScaleY(0.86f);
	normalBar->addChild(normalProgress);

	auto practiceBar = ax::Sprite::create("GJ_progressBar_001-hd.png");
	practiceBar->setPosition({ winSize.width / 2, winSize.height / 2.f - 80 });
	practiceBar->setColor({0, 0, 0});
	practiceBar->setOpacity(125);
	addChild(practiceBar, 3);

	auto practiceProgress = ax::Sprite::create("GJ_progressBar_001-hd.png");
	practiceProgress->setPosition({1.36f, 10});
	practiceProgress->setColor({0, 255, 0});
	practiceProgress->setOpacity(255);
	practiceProgress->setAnchorPoint({0, 0.5});
	practiceProgress->setContentSize({340 / (level->_normalPercent / 100.f), 20});
	practiceProgress->setScale(0.992f);
	practiceProgress->setScaleX(0.992f);
	practiceProgress->setScaleY(0.86f);
	practiceBar->addChild(practiceProgress);

	auto normalText = ax::Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Normal Mode");
	normalText->setPosition({ winSize.width / 2, winSize.height / 2.f - 10 });
	// normalText->enableShadow(ax::Color4B::BLACK, {0.2, -0.2});
	normalText->setScale(0.55f);
	addChild(normalText, 4);

	auto practiceText = ax::Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Practice Mode");
	practiceText->setPosition({ winSize.width / 2, winSize.height / 2.f - 60 });
	// practiceText->enableShadow(ax::Color4B::BLACK, {0.2, -0.2});
	practiceText->setScale(0.55f);
	addChild(practiceText, 4);

	auto normalPerc = ax::Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "");
	normalPerc->setPosition({ winSize.width / 2, winSize.height / 2.f - 30 });
	normalPerc->enableShadow(ax::Color4B::BLACK, {0.2, -0.2});
	normalPerc->setString(std::to_string((int)level->_normalPercent) + "%");
	normalPerc->setScale(0.55f);
	addChild(normalPerc, 4);

	auto practicePerc = ax::Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "");
	practicePerc->setPosition({ winSize.width / 2, winSize.height / 2.f - 80 });
	practicePerc->enableShadow(ax::Color4B::BLACK, {0.2, -0.2});
	practicePerc->setString(std::to_string((int)level->_practicePercent) + "%");
	practicePerc->setScale(0.55f);
	addChild(practicePerc, 4);


	
	auto scale9 = ax::ui::Scale9Sprite::create("square02_001.png");
	//scale9->setPosition({170, 47.5});
	scale9->setContentSize({340, 95});
	scale9->setOpacity(125);
	
	auto levelName = ax::Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), level->_LevelName);
	levelName->setPosition(190, 50.5);
	// levelName->setScale(0.904);
	scale9->addChild(levelName, 0);

	auto diffIcon = ax::Sprite::createWithSpriteFrameName("diffIcon_01_btn_001.png");
	diffIcon->setScale(1.1);
	diffIcon->setPosition(35.75, 50.5);
	scale9->addChild(diffIcon, 0);

	//1.0 didnt have stars apparently
	// auto starIcon = ax::Sprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
	// starIcon->setScale(0.7);
	// starIcon->setPosition({325, 82});
	// mainNode->addChild(starIcon, 0);

	// auto starAmt = ax::Label::createWithBMFont("bigFont.fnt", std::to_string(level->_Stars));
	// starAmt->setPosition({313, 82.5});
	// starAmt->setScale(0.5);
	// starAmt->setAnchorPoint({1, 0.5});
	// mainNode->addChild(starAmt, 0);
	auto mainBtn = MenuItemSpriteExtra::create(scale9, [&, level](Node* btn) 
		{
			if (!LevelPage::replacingScene)
			{
				ax::AudioEngine::stopAll();
				ax::AudioEngine::play2d("playSound_01.ogg", false, 0.2f);
				ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f, PlayLayer::scene(level)));
				LevelPage::replacingScene = true;
				MenuLayer::music = false;
			}
		});
	mainBtn->setScaleMultiplier(1.1f);
	auto levelMenu = ax::Menu::create();
	levelMenu->addChild(mainBtn);
	levelMenu->setPosition({ winSize.width / 2.f, winSize.height / 2.f + 60 });
	addChild(levelMenu);
	
	return true;
}

LevelPage* LevelPage::create(GJGameLevel* level)
{
	LevelPage* pRet = new LevelPage();
	if (pRet->init(level))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}