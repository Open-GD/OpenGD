#include "LevelEndStatsLayer.h"
#include "GameToolbox.h"
#include "AudioEngine.h"
#include "LevelEndStrings.h"

LevelEndStatsLayer *LevelEndStatsLayer::create(PlayLayer *pl)
{
	auto pRet = new (std::nothrow) LevelEndStatsLayer();

	if (pRet)
	{
		pRet->_attempts = pl->_attempts;
		pRet->_jumps = pl->_jumps;
		pRet->_time = pl->_secondsSinceStart;
		pRet->_createdWithoutPlaylayer = false;
		pRet->_everyplay_included = pl->_everyplay_recorded;
	}

	if (pRet && pRet->init(pl))
	{
		pRet->autorelease();

		return pRet;
	}
	else
	{
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}
LevelEndStatsLayer *LevelEndStatsLayer::create()
{
	auto pRet = new (std::nothrow) LevelEndStatsLayer();

	if (pRet)
	{
		pRet->_createdWithoutPlaylayer = true;
		pRet->_everyplay_included = true;
	}

	if (pRet && pRet->init(nullptr))
	{
		pRet->autorelease();

		return pRet;
	}
	else
	{
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool LevelEndStatsLayer::init(PlayLayer *pl)
{
	auto nd = ax::Layer::create();
	_statsLayer = DropDownLayer::create(nd, "");
	_statsLayer->showLayer(_createdWithoutPlaylayer, true);
	_statsLayer->hideBackButton();
	_playlayer = pl;
	if (!_createdWithoutPlaylayer)
		pl->m_pHudLayer->addChild(_statsLayer);
	auto wsize = ax::Director::getInstance()->getWinSize();

	// image

	auto sprite = ax::Sprite::createWithSpriteFrameName("GJ_levelComplete_001.png");
	sprite->setScale(0.8f);
	sprite->setPositionY(wsize.height / 5);
	nd->addChild(sprite);

	// attempts

	std::string attemptsText = "Attempts: ";
	attemptsText += std::to_string(_attempts);
	auto attempts = ax::Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), attemptsText);
	attempts->setPositionY(wsize.height / 5 - 25 - 15);
	attempts->setScale(0.8f);
	nd->addChild(attempts);

	// jumps

	std::string jumpsText = "Jumps: ";
	jumpsText += std::to_string(_jumps);
	auto jumps = ax::Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), jumpsText);
	jumps->setPositionY(wsize.height / 5 - 50 - 15);
	jumps->setScale(0.8f);
	nd->addChild(jumps);

	// time

	float div = (float)_time / 60.f;
	int minutes = (int)div;
	int seconds = (div - (float)minutes) * 60;

	std::string minutes_string = "";
	if (minutes < 10)
		minutes_string += "0";
	minutes_string += std::to_string(minutes);

	std::string seconds_string = "";
	if (seconds < 10)
		seconds_string += "0";
	seconds_string += std::to_string(seconds);

	std::string timeText = "Time: " + minutes_string + ":" + seconds_string;
	auto time = ax::Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), timeText);
	time->setPositionY(wsize.height / 5 - 75 - 15);
	time->setScale(0.8f);
	nd->addChild(time);

	// random string

	auto randomText = LevelEndStrings::getRandomString();
	auto randomt = ax::Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), randomText);
	randomt->setPositionY(wsize.height / 5 - 120 - 15);
	if (randomText.length() > 13)
	{
		randomt->setScale(0.75f);
	}
	else
	{
		randomt->setScale(0.9f);
	}
	nd->addChild(randomt);

	// buttons

	auto btnmenu = ax::Menu::create();
	_statsLayer->_dropLayer->addChild(btnmenu);
	btnmenu->setPosition({wsize.width / 3, wsize.height / 8});

	// replay

	auto replayspr = ax::Sprite::createWithSpriteFrameName("GJ_replayBtn_001.png");

	auto replaybtn = MenuItemSpriteExtra::create(replayspr, [&](Node *nd)
												 {
		this->_statsLayer->hideLayer();
		if(!this->_createdWithoutPlaylayer) {
			ax::AudioEngine::stopAll();
			ax::AudioEngine::play2d("playSound_01.ogg", false, 0.5f);
		}
		this->scheduleOnce([=](float d) {
			if(!this->_createdWithoutPlaylayer) _playlayer->resetLevel();
			this->removeFromParentAndCleanup(true);
		}, 0.5f, "levelendstatslayer_resetlevel"); });

	btnmenu->addChild(replaybtn);

	// exit

	auto exitspr = ax::Sprite::createWithSpriteFrameName("GJ_menuBtn_001.png");

	auto exitbtn = MenuItemSpriteExtra::create(exitspr, [&](Node *nd)
											   {
		this->_statsLayer->hideLayer();
		if(!this->_createdWithoutPlaylayer) {
			this->_playlayer->exit();
		} });

	exitbtn->setPositionX(wsize.width / 3);

	btnmenu->addChild(exitbtn);

	// stars

	// everyplay if possible

	if (_everyplay_included)
	{
		auto emenu = ax::Menu::create();
		emenu->setPosition({wsize.width / 4.1, wsize.height / 2.2});
		_statsLayer->_dropLayer->addChild(emenu);

		auto everyplayspr = ax::Sprite::createWithSpriteFrameName("GJ_watchReplay_001.png");
		auto everyplaybtn = MenuItemSpriteExtra::create(everyplayspr, [&](Node *nd) {

		});

		everyplaybtn->setRotation(-10.f);

		emenu->addChild(everyplaybtn);
	}

	return true;
}