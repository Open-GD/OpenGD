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

#include "keyboard.h"
#include "log.h"
#include "nodes.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <Director.h>
#include <EventDispatcher.h>

USING_NS_AX;


void GameToolbox::onKeyDown(bool equalZOrder, ax::Node* node, const std::function<void(ax::EventKeyboard::KeyCode, ax::Event*)>& callback)
{
	if(!node) return;
	
	auto listener = EventListenerKeyboard::create();
	if(!equalZOrder)
	{
		listener->onKeyPressed = callback;
	}
	else
	{
		listener->onKeyPressed = [callback, node](EventKeyboard::KeyCode key, Event* event)
		{
			auto dir = Director::getInstance();
			auto scene = dir->getRunningScene();
			if(GameToolbox::getHighestChildZ(scene) == node->getLocalZOrder()) {
				callback(key, event);
			}
		};
	}
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, node);
}