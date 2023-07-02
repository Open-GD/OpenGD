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

#include "TextButton.h"
#include <ui/CocosGUI.h>

#include "2d/ActionEase.h"
#include "GameToolbox/getTextureString.h"

USING_NS_AX;

void TextButton::selected() {
	MenuItemSprite::selected();
	this->runAction(EaseBounceOut::create(ScaleTo::create(.3f, m_fNewScale)));
}

void TextButton::unselected() {
	MenuItemSprite::unselected();
	this->stopAllActions();
	this->runAction(EaseBounceOut::create(ScaleTo::create(0.3f, 1.0f)));
}

void TextButton::activate() {
	MenuItemSprite::activate();
	this->stopAllActions();
	this->setScale(1.0f);
	this->m_fCallback(this);
}

bool TextButton::init(std::string_view text, std::string_view font, float width, float height, std::string_view bgTexture, std::function<void(TextButton*)> callback) {
	if (!MenuItemSprite::initWithNormalSprite(nullptr, nullptr, nullptr, nullptr)) return false;

	this->m_pText = Label::createWithBMFont(font, text);

	this->m_pBG = ui::Scale9Sprite::create(bgTexture);
	m_pBG->setStretchEnabled(true);
	
	this->m_pBG->setContentSize({
		(width == 0) ? this->m_pText->getContentSize().width + 16 : width,
		(height == 0) ? this->m_pText->getContentSize().height + 4 : height
	});
	this->addChild(this->m_pBG);
	this->m_pBG->setPosition(this->m_pBG->getContentSize() / 2);
	this->m_pBG->addChild(this->m_pText);

	this->m_pText->setPosition(this->m_pBG->getContentSize() / 2);
	this->m_pText->setScale((this->m_pBG->getContentSize().width - 16) / this->m_pText->getContentSize().width);

	this->setContentSize(this->m_pBG->getContentSize());

	this->setNewScale(1.15f);

	this->m_fCallback = callback;

	this->m_fWidth = width;
	this->m_fHeight = height;

	return true;
}


// Getters and Setters
void TextButton::setText(std::string_view text) {
	this->m_pText->setString(text);

	this->m_pBG->setContentSize({
		(this->m_fWidth == 0) ? this->m_pText->getContentSize().width + 32 : this->m_fWidth,
		(this->m_fHeight == 0) ? this->m_pText->getContentSize().height + 32 : this->m_fHeight
	});

	this->m_pText->setPosition(this->m_pBG->getContentSize() / 2);
}

Label* TextButton::getLabel()
{
	return this->m_pText;
}

ui::Scale9Sprite* TextButton::getBG()
{
	return this->m_pBG;
}

void TextButton::setCallback(std::function<void(TextButton*)> callback) {
	this->m_fCallback = callback;
}

// Create funcs
#pragma region
TextButton* TextButton::create(std::string_view text, std::string_view font, float width, float height, std::string_view bgTexture, std::function<void(TextButton*)> callback) {
	auto pRet = new(std::nothrow) TextButton();

	if (pRet && pRet->init(text, font, width, height, bgTexture, callback)) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

TextButton* TextButton::create(std::string_view text, std::function<void(TextButton*)> callback) {
	return TextButton::create(text, GameToolbox::getTextureString("goldFont.fnt"), 0, 0, GameToolbox::getTextureString("GJ_button_01.png"), callback);
}

TextButton* TextButton::create(std::string_view text, std::string_view font, std::function<void(TextButton*)> callback) {
	return TextButton::create(text, font, 0, 0, GameToolbox::getTextureString("GJ_button_01.png"), callback);
}

TextButton* TextButton::create(std::string_view text, std::string_view font, float width, float height, std::function<void(TextButton*)> callback) {
	return TextButton::create(text, font, width, height, GameToolbox::getTextureString("GJ_button_01.png"), callback);
}