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

#include "GameToolbox/enums.h"
#include "SimplePlayer.h"

#include "UTF8.h"
#include "base/Director.h"

#include "GameToolbox/math.h"
#include "GameToolbox/conv.h"

USING_NS_AX;

bool SimplePlayer::init(int cubeID) {
	if (!Sprite::init()) return false;

	this->updateGamemode(cubeID, IconType::kIconTypeCube);
	this->setContentSize({ 60, 60 });
	this->setAnchorPoint({.25f, .25f});

	return true;
}

void SimplePlayer::updateGamemode(int iconID, IconType mode) {
		iconID = GameToolbox::inRange(iconID, 1, GameToolbox::getValueForGamemode(mode));

	auto tipo = GameToolbox::getNameGamemode(mode);

	auto mainFrame = StringUtils::format("%s_%02d_001.png", tipo, iconID);
	auto secFrame = StringUtils::format("%s_%02d_2_001.png", tipo, iconID);
	auto extFrame = StringUtils::format("%s_%02d_extra_001.png", tipo, iconID);
	auto glowFrame = StringUtils::format("%s_%02d_glow_001.png", tipo, iconID);
	auto domeFrame = StringUtils::format("%s_%02d_3_001.png", tipo, iconID);

	if (m_pMainSprite) this->removeChild(m_pMainSprite);
	if (m_pSecondarySprite) this->removeChild(m_pSecondarySprite);
	if (m_pGlowSprite) this->removeChild(m_pGlowSprite);
	if (m_pExtraSprite) this->removeChild(m_pExtraSprite);
	if (m_pDomeSprite) this->removeChild(m_pDomeSprite);

	// Main Color
	m_pMainSprite = Sprite::createWithSpriteFrameName(mainFrame);
	if (!m_pMainSprite) m_pMainSprite = Sprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	m_pMainSprite->setAnchorPoint({ 0, 0 });
	m_pMainSprite->setStretchEnabled(false);
	this->addChild(m_pMainSprite);

	// Second Color
	m_pSecondarySprite = Sprite::createWithSpriteFrameName(secFrame);
	if (!m_pSecondarySprite) m_pSecondarySprite = Sprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	m_pSecondarySprite->setPosition(m_pMainSprite->getContentSize() / 2);
	m_pSecondarySprite->setStretchEnabled(false);
	this->addChild(m_pSecondarySprite, -1);

	// Glow
	m_pGlowSprite = Sprite::createWithSpriteFrameName(glowFrame);
	if (!m_pGlowSprite) m_pGlowSprite = Sprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	m_pGlowSprite->setPosition(m_pMainSprite->getContentSize() / 2);
	m_pGlowSprite->setStretchEnabled(false);
	m_pGlowSprite->setVisible(m_bHasGlow);
	this->addChild(m_pGlowSprite, -1);

	// Extra
	m_pExtraSprite = Sprite::createWithSpriteFrameName(extFrame);
	if (m_pExtraSprite) {
		m_pExtraSprite->setPosition(this->m_pMainSprite->getContentSize() / 2);
		m_pExtraSprite->setStretchEnabled(false);
		this->addChild(m_pExtraSprite);
	}

	// Dome
	if (mode == IconType::kIconTypeUfo) {
		m_pDomeSprite = Sprite::createWithSpriteFrameName(domeFrame);
		if (m_pDomeSprite) {
			m_pDomeSprite->setPosition(this->m_pMainSprite->getContentSize() / 2);
			m_pDomeSprite->setStretchEnabled(false);
			this->addChild(m_pDomeSprite, -1);
		}
	}

	this->updateIconColors();
}

void SimplePlayer::updateIconColors() {
	m_pMainSprite->setColor(m_MainColor);
	m_pSecondarySprite->setColor(m_SecondaryColor);
	m_pGlowSprite->setColor(m_GlowColor);
}

void SimplePlayer::setMainColor(Color3B col) {
	m_pMainSprite->setColor(col);
	m_MainColor = col;
}

void SimplePlayer::setSecondaryColor(Color3B col) {
	m_pSecondarySprite->setColor(col);
	m_SecondaryColor = col;
}

void SimplePlayer::setGlow(bool glow) {
	m_pGlowSprite->setVisible(glow);
	m_bHasGlow = glow;
}

void SimplePlayer::setGlowColor(Color3B col) {
	m_pGlowSprite->setColor(col);
	m_GlowColor = col;
}

SimplePlayer* SimplePlayer::create(int cubeID) {
	auto pRet = new (std::nothrow) SimplePlayer();

	if (pRet && pRet->init(cubeID)) {
		pRet->autorelease();
		return pRet;
	}
	AX_SAFE_DELETE(pRet);
	return pRet;
}
