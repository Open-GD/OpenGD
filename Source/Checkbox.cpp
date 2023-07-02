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

/*---------------------------
!! WILL BE REWRITTEN !!


#pragma once
#include "Checkbox.h"


#include <2d/Sprite.h>
#include "GameToolbox/getTextureString.h"

USING_NS_AX;

Checkbox* Checkbox::create(std::string_view text, std::string_view on, std::string_view off, bool toggled, CheckboxCallback callback) {
	Checkbox* pRet = new(std::nothrow) Checkbox(on, off, toggled, callback);

	if (pRet && pRet->init(text)) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

Checkbox* Checkbox::create(std::string_view text, bool toggled, CheckboxCallback callback) {
	return Checkbox::create(text, "GJ_checkOn_001.png", "GJ_checkOff_001.png", toggled, callback);
}

Checkbox* Checkbox::create(std::string_view text, CheckboxCallback callback) {
	return Checkbox::create(text, false, callback);
}

Checkbox::Checkbox(std::string_view on, std::string_view off, bool toggled, CheckboxCallback callback) :
	MenuItemSpriteExtra(toggled ? on : off, nullptr, [](Node const*) {}), _offSpr(off), _onSpr(on), _toggled(toggled), _callback(callback)
{
}


bool Checkbox::init(std::string_view text)
{
	if(!MenuItemSpriteExtra::init()) return false;

	//TODO: RE GameToolbox::createToggleButton and add text
	//auto label = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), text);
	//float s = GameToolbox::stof(FileUtils::getInstance()->getStringFromFile("Custom/float.txt"));
	//label->setBMFontSize(20.0f);
	//label->setPositionX(120.0f);
	//label->setPositionY(15.0f);
	//addChild(label);

	return true;
}


void Checkbox::activate()
{
	MenuItemSpriteExtra::activate();
	_toggled = !_toggled;
	dynamic_cast<Sprite*>(getSprite())->setSpriteFrame(_toggled ? _onSpr : _offSpr);
	_callback(this, _toggled);
}

*/