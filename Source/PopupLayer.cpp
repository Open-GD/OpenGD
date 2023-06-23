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

#include <2d/CCActionEase.h>
#include <2d/CCActionInterval.h>
#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListenerTouch.h>

USING_NS_AX;

void PopupLayer::show(Transitions transitions)
{
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

	if(!getParent())
	{
		auto scene = Director::getInstance()->getRunningScene();
		scene->addChild(this);
	}
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
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	this->removeFromParent();
}
