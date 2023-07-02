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

#include "PopupLayer.h"

#include <2d/ActionEase.h>
#include <2d/ActionInterval.h>
#include <base/Director.h>
#include <base/EventDispatcher.h>
#include <base/EventListenerTouch.h>
#include "GameToolbox/nodes.h"
#include "GameToolbox/log.h"
#include "GameToolbox/keyboard.h"
#include "EventListenerKeyboard.h"
#include "2d/ActionInterval.h"
#include "2d/ActionInstant.h"
#include "EventKeyboard.h"

USING_NS_AX;

void PopupLayer::show(Transitions transitions)
{
	//robtops system to add to highest z
	auto scene = Director::getInstance()->getRunningScene();
	int z = GameToolbox::getHighestChildZ(scene);
	if(z <= 104)
		z = 105;
	else
		z++;
	
	scene->addChild(this, z);
	this->setOpacity(0);

	switch (transitions)
	{
		case kNone:
			break;
		default:
			this->_mainLayer->setScale(0.1f);
			this->_mainLayer->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, 1.0f), 0.6f));
	}
	this->runAction(FadeTo::create(0.14, 150));
	
	GameToolbox::onKeyDown(true, this, [this](EventKeyboard::KeyCode code, Event*)
	{
		GameToolbox::log("key down popup: {}", static_cast<int>(code));
		switch(code)
		{
			case EventKeyboard::KeyCode::KEY_BACK:
				close();
			default:
				break;
		}
	});
}

bool PopupLayer::init()
{
	if (!this->initWithColor(Color4B(0, 0, 0, 106)))
		return false;
	
	this->_mainLayer = Layer::create();
	this->addChild(_mainLayer);

	// set up listener 
	auto listener = EventListenerTouchOneByOne::create();
	listener->setEnabled(true);
	listener->setSwallowTouches(true); 
	listener->onTouchBegan = [=](Touch*, Event*) -> bool {
		return true;
	};
	
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	this->setup();

	
	
	return true;
}

void PopupLayer::showOnLayer(ax::Node* node, Transitions transition)
{
	removeFromParent();
	node->addChild(this);
	show(transition);
}


void PopupLayer::keyBackClicked() {
	this->close();
}

void PopupLayer::close()
{
	this->runAction(Sequence::create(DelayTime::create(0), CallFunc::create([this]() {
		Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
		this->removeFromParent();
	}), nullptr));
}