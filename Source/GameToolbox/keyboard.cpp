#include "keyboard.h"
#include "log.h"
#include "nodes.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <CCDirector.h>
#include <CCEventDispatcher.h>

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