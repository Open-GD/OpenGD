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

#include "CurrencyRewardLayer.h"
#include "2d/Label.h"
#include "format.h"
#include "2d/ActionInterval.h"
#include <2d/ActionEase.h>
#include "2d/ActionInstant.h"
#include "base/Director.h"

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

		auto sequence = Sequence::create(TintTo::create(0.3,0,255,255),TintTo::create(0.3,150,255,255), nullptr);
		label->runAction(RepeatForever::create(sequence));
	}

	Vec2 movetoPos; // finish


    //dropDownStats->runAction(EaseInOut::create(MoveTo::create(0.5, ??),2.0));

    return true;
}