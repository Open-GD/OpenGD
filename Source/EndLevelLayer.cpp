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

#include "EndLevelLayer.h"
#include "PlayLayer.h"
#include "DropDownLayer.h"
#include "MenuItemSpriteExtra.h"
#include "UILayer.h"

#include "2d/Label.h"
#include "2d/Menu.h"
#include "base/Director.h"
#include "AudioEngine.h"

#include <fmt/chrono.h>
#include <array>
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/rand.h"

//same as LoadingLayer
constexpr static auto _endingStrings = std::to_array <const char*>({
	"Awesome!", "Good Job!", "Well Done!", "Impressive!",
	"Amazing!", "Incredible!", "Skillful!", "Brilliant!",
	"You are... The One!", "How this is possible?",
	"You beat me...", "Challenge Breaker!", "I am speechless...",
	"Not bad!", "Warp Speed", "Y u do dis?", "lol is lol backwards",
	"DROP THE BEAT", "Took you long enough...", "Teach me oh great one",
	"Haxxor?", "Tripple spikes?", "RubRubRubRubRubRub", "SO FAST",
	"Hmmmmmmmmmmm", "Ship part was cool!", "Timing could be better",
	"I cant hear the music.", "Pump. It. Up.", "I am Batman",
	"Take a break.", "AFK", "he protec"
});

std::string_view EndLevelLayer::getRandomEndingString()
{
	return _endingStrings[GameToolbox::randomInt(0, _endingStrings.size() - 1)];
}

EndLevelLayer *EndLevelLayer::create(PlayLayer *pl)
{
	auto pRet = new (std::nothrow) EndLevelLayer();

	if (pRet && pl != nullptr)
	{
		pRet->_attempts = pl->_attempts;
		pRet->_jumps = pl->_jumps;
		pRet->_time = pl->_secondsSinceStart;
		pRet->_createdWithoutPlaylayer = false;
		pRet->_everyplay_included = pl->_everyplay_recorded;
	} else {
		pRet->_createdWithoutPlaylayer = true;
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

bool EndLevelLayer::init(PlayLayer *pl)
{
	auto nd = ax::Layer::create();
	_statsLayer = DropDownLayer::create(nd, "");
	_statsLayer->showLayer(_createdWithoutPlaylayer, true);
	_statsLayer->hideBackButton();
	_playlayer = pl;
	if (!_createdWithoutPlaylayer)
		pl->m_pHudLayer->addChild(_statsLayer);
	const auto& wsize = ax::Director::getInstance()->getWinSize();

	// image

	auto sprite = ax::Sprite::createWithSpriteFrameName("GJ_levelComplete_001.png");
	sprite->setScale(0.8f);
	sprite->setPositionY(wsize.height / 5);
	nd->addChild(sprite);
	
	//just call once and use
	std::string goldFontStr = GameToolbox::getTextureString("goldFont.fnt");
	std::string bigFontStr = GameToolbox::getTextureString("bigFont.fnt");

	// attempts
	std::string attemptsText = fmt::format("Attempts: {}", _attempts);
	auto attempts = ax::Label::createWithBMFont(goldFontStr, attemptsText);
	attempts->setPositionY(wsize.height / 5 - 25 - 15);
	attempts->setScale(0.8f);
	nd->addChild(attempts);

	// jumps
	std::string jumpsText = fmt::format("Jumps: {}", _jumps);
	auto jumps = ax::Label::createWithBMFont(goldFontStr, jumpsText);
	jumps->setPositionY(wsize.height / 5 - 50 - 15);
	jumps->setScale(0.8f);
	nd->addChild(jumps);

	// time
	std::chrono::seconds duration{_time};
	std::string timeText = fmt::format("Time: {:%H:%M:%S}", duration);
	auto time = ax::Label::createWithBMFont(goldFontStr, timeText);
	time->setPositionY(wsize.height / 5 - 75 - 15);
	time->setScale(0.8f);
	nd->addChild(time);

	// random string

	std::string_view randomText = EndLevelLayer::getRandomEndingString();
	auto randomt = ax::Label::createWithBMFont(bigFontStr, randomText);
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
		if (!this->_createdWithoutPlaylayer) {
			ax::AudioEngine::stopAll();
			ax::AudioEngine::play2d("playSound_01.ogg", false, 0.5f);
		}
		this->scheduleOnce([=](float d) {
			if(!this->_createdWithoutPlaylayer) _playlayer->resetLevel();
			this->removeFromParentAndCleanup(true);
		}, 0.5f, "EndLevelLayer_resetlevel"); });

	btnmenu->addChild(replaybtn);

	// exit
	auto exitspr = ax::Sprite::createWithSpriteFrameName("GJ_menuBtn_001.png");

	auto exitbtn = MenuItemSpriteExtra::create(exitspr, [&](Node *nd)
	{
		this->_statsLayer->hideLayer();
		if (!this->_createdWithoutPlaylayer) {
			this->_playlayer->exit();
		}
	});

	exitbtn->setPositionX(wsize.width / 3);

	btnmenu->addChild(exitbtn);

	// stars

	// everyplay if possible

	if (_everyplay_included)
	{
		auto emenu = ax::Menu::create();
		emenu->setPosition({wsize.width / 4.1f, wsize.height / 2.2f});
		_statsLayer->_dropLayer->addChild(emenu);

		auto everyplayspr = ax::Sprite::createWithSpriteFrameName("GJ_watchReplay_001.png");
		auto everyplaybtn = MenuItemSpriteExtra::create(everyplayspr, [&](Node *nd) {

		});

		everyplaybtn->setRotation(-10.f);

		emenu->addChild(everyplaybtn);
	}

	return true;
}