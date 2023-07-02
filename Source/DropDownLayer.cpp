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

#include "DropDownLayer.h"
#include "Director.h"
#include "2d/Menu.h"
#include "MenuItemSpriteExtra.h"
#include "ListLayer.h"
#include "EventListenerTouch.h"
#include "EventDispatcher.h"
#include "2d/ActionInterval.h"
#include "2d/ActionEase.h"
#include "2d/ActionInstant.h"
#include "GameToolbox/keyboard.h"

USING_NS_AX;

DropDownLayer* DropDownLayer::create(Node* scrollLayer, const char* label)
{
	auto pRet = new(std::nothrow) DropDownLayer();

	if (pRet && pRet->init(scrollLayer, label))
	{
		pRet->autorelease();
		return pRet;
	} 
	else
	{
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool DropDownLayer::init(Node* scrollLayer, const char* label)
{
	if(!this->initWithColor({0, 0, 0, 0})) return false;
	
	auto winSize = Director::getInstance()->getWinSize();

	this->_dropLayer = Layer::create();
	_dropLayer->setPosition({0, this->_dropLayer->getContentSize().height});
	
	//menu start
	auto menu = Menu::create();

	auto backSpr = Sprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	_backBtn = MenuItemSpriteExtra::create(backSpr, [&](Node* btn){
		this->hideLayer();
	});

	_backBtn->setPosition(menu->convertToNodeSpace({24, winSize.height - 23}));
	menu->addChild(_backBtn);
	_dropLayer->addChild(menu);

	auto chain1 = Sprite::createWithSpriteFrameName("chain_01_001.png");
	chain1->setPosition({(winSize.width / 2) - 156, winSize.height - 43});
	chain1->setAnchorPoint({0.5f, 0.0f});
	_dropLayer->addChild(chain1);

	auto chain2 = Sprite::createWithSpriteFrameName("chain_01_001.png");
	chain2->setPosition({(winSize.width / 2) + 156, winSize.height - 43});
	chain2->setAnchorPoint({0.5f, 0.0f});
	_dropLayer->addChild(chain2);


	auto listlayer = ListLayer::create(scrollLayer, label);
	listlayer->setPosition({ (winSize.width - 356) / 2, ((winSize.height - 220) / 2) - 5 });
	_dropLayer->addChild(listlayer);
	//menu end

	this->addChild(_dropLayer);

	this->customSetup();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setEnabled(true);
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch*, Event*) -> bool {
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void DropDownLayer::showLayer(bool attachToScene, bool bounce)
{
	const auto& winSize = Director::getInstance()->getWinSize();

	this->runAction(FadeTo::create(0.5, 125));
	if (!bounce) this->_dropLayer->runAction(EaseInOut::create(MoveTo::create(0.5f, {this->getPositionX(), 0}), 2));
	else this->_dropLayer->runAction(EaseBounceOut::create(MoveTo::create(1.f, {this->getPositionX(), 0})));

	if (attachToScene)
	{
		auto scene = Director::getInstance()->getRunningScene();
		scene->addChild(this, 100);
	}
	
	GameToolbox::onKeyDown(true, this, [this](ax::EventKeyboard::KeyCode, ax::Event*)
	{
		hideLayer();
	});
}

void DropDownLayer::hideLayer()
{
	//const auto& winSize = Director::getInstance()->getWinSize();

	this->runAction(FadeTo::create(0.5, 0));
	this->_dropLayer->runAction(
		Sequence::create(
			EaseInOut::create(MoveTo::create(0.5f, {0, this->_dropLayer->getContentSize().height}), 2),
			CallFunc::create([&]()
			{
				Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
				this->removeFromParent();
			}),
			nullptr
		)
	);
}

void DropDownLayer::hideBackButton() {
	_backBtn->setEnabled(false);
	_backBtn->setVisible(false);
}