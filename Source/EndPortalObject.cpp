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

#include "EndPortalObject.h"
#include "base/Director.h"

USING_NS_AX;


EndPortalObject* EndPortalObject::create()
{
	auto pRet = new (std::nothrow) EndPortalObject();

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool EndPortalObject::init()
{
	if(!GameObject::init("block001_01_001.png"))
		return false;
	
	auto winSize = Director::getInstance()->getWinSize();
	_gradientBar = Sprite::createWithSpriteFrameName("gradientBar.png");
	return true;
}