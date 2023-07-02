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

#include "UILayer.h"
#include "PlayLayer.h"

#include "EventListenerTouch.h"
#include "EventDispatcher.h"
#include "base/Director.h"

UILayer* UILayer::create()
{
	UILayer* pRet = new UILayer();
	if (pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool UILayer::init()
{
	if (!ax::Layer::init()) return false;

	auto dir = ax::Director::getInstance();
	auto listener = ax::EventListenerTouchOneByOne::create();

	listener->setEnabled(true);
	listener->setSwallowTouches(true);

	// trigger when you start touch
	listener->onTouchBegan = AX_CALLBACK_2(UILayer::onTouchBegan, this);
	listener->onTouchEnded = AX_CALLBACK_2(UILayer::onTouchEnded, this);

	dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool UILayer::onTouchBegan(ax::Touch* touch, ax::Event* event)
{
	auto pl = PlayLayer::getInstance();
	pl->_player1->pushButton();
	if (pl->_isDualMode) pl->_player2->pushButton();
	return true;
}

void UILayer::onTouchEnded(ax::Touch* touch, ax::Event* event)
{
	auto pl = PlayLayer::getInstance();
	pl->_player1->releaseButton();
	if (pl->_isDualMode) pl->_player2->releaseButton();
}