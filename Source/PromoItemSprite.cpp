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

#include "PromoItemSprite.h"
#include "GameManager.h"
#include "2d/Sprite.h"
USING_NS_AX;


static std::string getTextureString_PromoSprite(std::string texture) {

	auto pos = texture.find(".");
	if (pos != std::string::npos && GameManager::getInstance()->isHigh()) {
		texture.insert(pos, "-hd");	
	}
	return texture;
}

PromoItemSprite::PromoItemSprite(const char* spriteStr, std::function<void(Node*)> callback) {
	m_pSprite = ax::Sprite::create(getTextureString_PromoSprite(spriteStr));
	m_fCallback = callback;
}

void PromoItemSprite::selected() {
	m_pSprite->setColor({125, 125, 125});
	MenuItemSprite::selected();
}

void PromoItemSprite::unselected() {
	m_pSprite->setColor({255, 255, 255});
	MenuItemSprite::unselected();
}

void PromoItemSprite::activate() {
	m_fCallback(this);
}

Node* PromoItemSprite::getSprite() { return m_pSprite; }

bool PromoItemSprite::init() {
	if (!initWithNormalSprite(m_pSprite, m_pSprite, m_pSprite, nullptr)) return false;
	
	m_pSprite->setAnchorPoint({0.5, 0.5});
	m_pSprite->setPosition(m_pSprite->getContentSize() * 0.5f);
	return true;
}

PromoItemSprite* PromoItemSprite::create(const char* sprite, std::function<void(Node*)> callback) {
	PromoItemSprite* pRet = new(std::nothrow) PromoItemSprite(sprite, callback);

	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}