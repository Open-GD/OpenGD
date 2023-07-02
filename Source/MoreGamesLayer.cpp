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

#include "MoreGamesLayer.h"
#include "DropDownLayer.h"
#include "2d/Menu.h"
#include "2d/Label.h"
#include "base/Director.h"
#include "Application.h"

#include "GameToolbox/getTextureString.h"

MoreGamesLayer* MoreGamesLayer::create(){
	auto pRet = new(std::nothrow) MoreGamesLayer();

	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool MoreGamesLayer::init()
{
	auto layer = ax::Layer::create();

	std::string bigFontStr = GameToolbox::getTextureString("bigFont.fnt");
	const auto& winSize = ax::Director::getInstance()->getWinSize();

	auto label = ax::Label::createWithBMFont(bigFontStr, "Nothing here yet... sorry :(");
	label->setScale(0.6f);
	label->setPosition(0, 0);
	layer->addChild(label);

	auto dropdownlayer = DropDownLayer::create(layer, "RobTop Games");
	dropdownlayer->showLayer(true, false);

	return true;
}