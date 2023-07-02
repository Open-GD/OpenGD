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

#include "GJChestSprite.h"

#include "fmt/format.h"
#include "2d/ParticleSystemQuad.h"
#include "2d/ActionInterval.h"
#include <2d/ActionEase.h>
#include "2d/ActionInstant.h"
#include "base/Director.h"
#include "GameToolbox/conv.h"

USING_NS_AX;

int GJChestSprite::_chestID = 0;

GJChestSprite* GJChestSprite::create(int chestID) {
	auto pRet = new (std::nothrow) GJChestSprite();
	_chestID = chestID;

	if (pRet && pRet->switchState(2, false)) {
		pRet->autorelease();
		return pRet;
	}
	AX_SAFE_DELETE(pRet);
	return pRet;
}

bool GJChestSprite::switchState(int state, bool idk)
{
	this->removeAllChildren();
	if (state == 2)
	{
		Sprite* chestSprite = Sprite::createWithSpriteFrameName(fmt::format("chest_0{}_0{}_001.png", _chestID, state));
		this->addChild(chestSprite);
		return true;
	}

	Color3B glowColor = Color3B(255, 150, 0);

	switch (_chestID)
	{
		case 2:
			glowColor = Color3B(0, 200, 255);
			break;
		case 3:
			glowColor = Color3B(0, 200, 0);
			break;
	}

	if (state == 3)
	{
		Sprite* chestSprite = Sprite::createWithSpriteFrameName(fmt::format("chest_0{}_0{}_001.png", _chestID, state));
		Sprite* chestSpriteBack = Sprite::createWithSpriteFrameName(fmt::format("chest_0{}_0{}_back_001.png", _chestID, state));
		Sprite* chestGlow = Sprite::createWithSpriteFrameName(fmt::format("chest_0{}_0{}_glow_001.png", _chestID, state));
		chestGlow->setBlendFunc(GameToolbox::getBlending());
		
		this->addChild(chestSpriteBack);
		this->addChild(chestGlow);
		this->addChild(chestSprite);

		if (!idk)
		{
			auto chestOpenParticle = ParticleSystemQuad::create("chestOpen.plist");
			chestOpenParticle->setStartColor({(float) glowColor.r, (float) glowColor.g, (float) glowColor.b, 255});
			chestOpenParticle->setEndColor({(float) glowColor.r, (float) glowColor.g, (float) glowColor.b, 255});
			chestOpenParticle->setPosition({0.0f, 0.0f});
			this->addChild(chestOpenParticle);
		}
		return true;
	}
	if (state != 4) return true;
	if (state == 4)
	{
		Sprite* chestSprite = Sprite::createWithSpriteFrameName(fmt::format("chest_0{}_0{}_001.png", _chestID, state));
		Sprite* chestSpriteBack = Sprite::createWithSpriteFrameName(fmt::format("chest_0{}_0{}_back_001.png", _chestID, state));
		Sprite* chestGlow = Sprite::createWithSpriteFrameName(fmt::format("chest_0{}_0{}_glow_001.png", _chestID, state));
		Sprite* shineSprite1 = Sprite::createWithSpriteFrameName("shineBurst_001.png");
		Sprite* shineSprite2 = Sprite::createWithSpriteFrameName("shineBurst_001.png");
		chestGlow->setBlendFunc(GameToolbox::getBlending());
		shineSprite1->setBlendFunc(GameToolbox::getBlending());
		shineSprite1->setColor({glowColor});
		shineSprite2->setBlendFunc(GameToolbox::getBlending());
		shineSprite2->setColor({glowColor});

		auto glow1 = EaseInOut::create(FadeTo::create(1.0,175),2.0);
		auto glow2 = EaseInOut::create(FadeTo::create(1.0,255),2.0);
		chestGlow->runAction(RepeatForever::create(Sequence::create(glow1, glow2, 0, nullptr)));

		float offset = 1.0f;
		if (_chestID == 1)
		{
		offset = 0.8f;
		}

		float randi = rand();
		float rotationDirection = 1.0f;
		if (randi / 32767.0 <= 0.5) {
		rotationDirection = -1.0;
		}
		randi = rand();
		auto rAction1 = RepeatForever::create(RotateBy::create(1.0,((randi / 32767.0) * 0.5 + 0.5) * 180.0 * rotationDirection));
		shineSprite1->setPosition(0.0,-5.0);
		shineSprite1->setScale(4 * offset);
		shineSprite1->runAction(rAction1);

		randi = rand();
		rAction1 = RepeatForever::create(RotateBy::create(1.0,((randi / 32767.0) * 0.5 + 0.5) * -180.0 * rotationDirection));
		shineSprite2->setPosition(0.0,-5.0);
		shineSprite2->setScale(3 * offset);
		shineSprite2->runAction(rAction1);

		auto chestOpenedParticle = ParticleSystemQuad::create("chestOpened.plist");
			chestOpenedParticle->setStartColor({255, 255, 255, 255});
			chestOpenedParticle->setEndColor({(float) glowColor.r, (float) glowColor.g, (float) glowColor.b, 255});
			chestOpenedParticle->setPosition({0.0f, 0.0f});
			chestOpenedParticle->setScale(offset);
			
		auto gradient1 = Sprite::createWithSpriteFrameName("chest_glow_bg_001.png");
		gradient1->setPositionX(offset);
		gradient1->setScale(offset);
		gradient1->setColor({51, 68, 153});

		auto gradient2 = Sprite::createWithSpriteFrameName("chest_glow_bg_001.png");
		gradient2->setPositionX(offset);
		gradient2->setFlippedX(true);
		gradient2->setScale(offset);
		gradient2->setColor({51, 68, 153});
		
		auto blueSquare = Sprite::createWithSpriteFrameName("whiteSquare60_001.png");
		blueSquare->setPosition(0.0, offset * -52.0);
		blueSquare->setScaleX(offset * 5.2);
		blueSquare->setScaleY(offset * 2.2);
		blueSquare->setColor({51, 68, 153});

		auto shadowSprite = Sprite::createWithSpriteFrameName("chest_shadow_001.png");
		shadowSprite->setPosition(0.0, offset * -48.0);
		shadowSprite->setScale(offset * 0.95f);
		shadowSprite->setOpacity(90);

		this->addChild(chestSpriteBack);
		this->addChild(chestGlow);
		this->addChild(shineSprite1);
		this->addChild(shineSprite2);
		this->addChild(chestOpenedParticle);
		this->addChild(gradient1);
		this->addChild(gradient2);
		this->addChild(blueSquare);
		this->addChild(shadowSprite);
		this->addChild(chestSprite);
	}
	return true;
}