#include "CurrencyRewardLayer.h"
#include "2d/CCLabel.h"
#include "format.h"
#include "2d/CCActionInterval.h"
#include <2d/CCActionEase.h>
#include "2d/CCActionInstant.h"
#include "base/CCDirector.h"
#

USING_NS_AX;

CurrencyRewardLayer* CurrencyRewardLayer::create(int orbs, int stars, int diamonds)
{
    auto pRet = new (std::nothrow) CurrencyRewardLayer();

	if (pRet && pRet->init(orbs, stars, diamonds)) {
		pRet->autorelease();
		return pRet;
	}
	AX_SAFE_DELETE(pRet);
	return pRet;
}

bool CurrencyRewardLayer::init(int orbs, int stars, int diamonds)
{
	const auto& winSize = Director::getInstance()->getWinSize();

	Node* dropDownStats = Node::create();

	this->addChild(dropDownStats, 4);

	dropDownStats->setPosition(winSize.width - 40.f, (winSize.height - 5.0f) /*+ param_10*/);

	if (orbs > 0)
	{
		auto label = Label::createWithBMFont("bigFont.fnt", fmt::format("{}", 1000)); // replace with GameManager stats
		label->setScale(2.f); // change this, seems to be related to the size of the window
		dropDownStats->addChild(label, 10);

		auto sprite = Sprite::createWithSpriteFrameName("currencyOrbIcon_001.png");
		dropDownStats->addChild(sprite, 3);
		sprite->setPosition(15.0f, -0.5 /* - local_68.height * 0.5*/);

		auto sequence = Sequence::create(TintTo::create(0.3,0,255,255),TintTo::create(0.3,150,255,255),0);
		label->runAction(RepeatForever::create(sequence));
	}

	Vec2 movetoPos; // finish


    //dropDownStats->runAction(EaseInOut::create(MoveTo::create(0.5, ??),2.0));

    return true;
}