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

#include "GroundLayer.h"
#include "PlayLayer.h"
#include "base/Director.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/conv.h"
#include "fmt/format.h"

USING_NS_AX;

bool GroundLayer::init(int groundID)
{
	if (!Layer::init()) return false;

	auto winSize = Director::getInstance()->getWinSize();

	auto name = fmt::format("groundSquare_{:02}_001.png", groundID);
	this->_sprite = Sprite::create(GameToolbox::getTextureString(name));
	if(!this->_sprite)
	{
		auto name2 = fmt::format("groundSquare_{:02}_001.png", 1);
		this->_sprite = Sprite::create(GameToolbox::getTextureString(name));
	}
	_sprite->setStretchEnabled(false);
	this->m_fOneGroundSize = this->_sprite->getTextureRect().size.width;
	this->_sprite->getTexture()->setTexParameters(
		{backend::SamplerFilter::NEAREST, backend::SamplerFilter::NEAREST, backend::SamplerAddressMode::REPEAT,
		 backend::SamplerAddressMode::REPEAT});
	this->_sprite->setTextureRect({0, 0, winSize.width + this->m_fOneGroundSize, _sprite->getTextureRect().size.height});
	this->_sprite->setAnchorPoint({0, 0});
	this->_sprite->setPosition({0, -50});
	this->_sprite->setColor({0, 102, 255});
	this->addChild(this->_sprite);

	auto line = Sprite::createWithSpriteFrameName("floorLine_001.png");
	line->setStretchEnabled(false);
	line->setBlendFunc(GameToolbox::getBlending());
	this->addChild(line);
	line->setPosition({winSize.width / 2, this->_sprite->getContentSize().height + this->_sprite->getPositionY()});

	auto gradient1 = Sprite::createWithSpriteFrameName("groundSquareShadow_001.png");
	gradient1->setStretchEnabled(false);
	this->addChild(gradient1);
	gradient1->setScale(0.7f);
	gradient1->setPositionY(33);

	auto gradient2 = Sprite::createWithSpriteFrameName("groundSquareShadow_001.png");
	gradient2->setStretchEnabled(false);
	this->addChild(gradient2);
	gradient2->setScale(0.7f);
	gradient2->setFlippedX(true);
	gradient2->setPositionX(winSize.width);
	gradient2->setPositionY(33);

	this->m_fSpeed = 5.770002f;

	//scheduleUpdate();

	return true;
}

void GroundLayer::updateTweenAction(float value, std::string_view key)
{
	if (key == "y") setPositionY(value);
}

void GroundLayer::update(float dt)
{

	if (auto pl = PlayLayer::getInstance())
	{
		if (pl->m_pColorChannels.contains(1001)) _sprite->setColor(pl->m_pColorChannels.at(1001)._color);
	}

	this->_sprite->setPositionX(this->_sprite->getPositionX() - dt * this->m_fSpeed);

	if (this->_sprite->getPositionX() <= -128.0f) this->_sprite->setPositionX(0);
}

GroundLayer* GroundLayer::create(int groundID)
{
	GroundLayer* pRet = new (std::nothrow) GroundLayer();

	if (pRet && pRet->init(groundID))
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