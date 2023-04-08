#include "LevelEndStatsLayer.h"
#include "GameToolbox.h"

LevelEndStatsLayer* LevelEndStatsLayer::create(PlayLayer *pl)
{
	auto pRet = new(std::nothrow) LevelEndStatsLayer();

	if (pRet && pRet->init(pl))
    {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool LevelEndStatsLayer::init(PlayLayer *pl) {
    auto nd = ax::Layer::create();
    _statsLayer = DropDownLayer::create(nd, "");
    _statsLayer->showLayer(false, true);
    _playlayer = pl;
    pl->m_pHudLayer->addChild(_statsLayer);
	auto wsize = ax::Director::getInstance()->getWinSize();

	// logo init
	auto sprite = ax::Sprite::createWithSpriteFrameName("GJ_levelComplete_001.png");
	sprite->setScale(0.8f);
	sprite->setPositionY(wsize.height / 5);
	nd->addChild(sprite);

	// attempts

	std::string attemptsText = "Attempts: ";
	attemptsText += std::to_string(pl->_attempts);
	auto attempts = ax::Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), attemptsText);
	attempts->setPositionY(wsize.height / 5 - 25 - 15);
	attempts->setScale(0.8f);
	nd->addChild(attempts);

	// jumps

	std::string jumpsText = "Jumps: ";
	jumpsText += std::to_string(pl->_jumps);
	auto jumps = ax::Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), jumpsText);
	jumps->setPositionY(wsize.height / 5 - 50 - 15);	
	jumps->setScale(0.8f);
	nd->addChild(jumps);

	// time

	float div = (float)pl->_secondsSinceStart / 60.f;
	int minutes = (int)div;
	int seconds = (div - (float)minutes) * 60;

	std::string minutes_string = "";
	if (minutes < 10) minutes_string += "0";
	minutes_string += std::to_string(minutes);

	std::string seconds_string = "";
	if(seconds < 10) seconds_string += "0";
	seconds_string += std::to_string(seconds);

	std::string timeText = "Time: " + minutes_string + ":" + seconds_string;
	auto time = ax::Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), timeText);
	time->setPositionY(wsize.height / 5 - 75 - 15);
	time->setScale(0.8f);
	nd->addChild(time);

    return true;
}