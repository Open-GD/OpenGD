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

#include "BoomScrollLayer.h"
#include "GJGameLevel.h"
#include "MenuItemSpriteExtra.h"
#include "AudioEngine.h"
#include "2d/ActionEase.h"
#include "Director.h"
#include "EventDispatcher.h"
#include "GameToolbox/log.h"

USING_NS_AX;

BoomScrollLayer* BoomScrollLayer::create(std::vector<ax::Layer*> layers, int currentPage)
{
	BoomScrollLayer* pRet = new BoomScrollLayer();
	if (pRet->init(layers, currentPage))
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

void BoomScrollLayer::selectPage(int current)
{
	auto winSize = Director::getInstance()->getWinSize();
	auto count = _internalLayer->getChildrenCount();
	GameToolbox::log("current: {}", current);

	int change = _currentPage - current;
	if (change < -1) change = 1;
	if (change > 1) change = -1;

	_currentPage = current - 1;
}

void BoomScrollLayer::changePageRight()
{
	_layers[_leftPage]->retain();
	_internalLayer->removeChild(_layers[_leftPage]);

	_leftPage = _leftPage + 1 >= _layers.size() ? 0 : _leftPage + 1;

	_rightPage = _rightPage + 1 >= _layers.size() ? 0 : _rightPage + 1;

	_internalLayer->addChild(_layers[_rightPage]);
	_layers[_rightPage]->setPositionX(Director::getInstance()->getWinSize().width * 2);
	_layers[_rightPage]->release();

	float newX = Director::getInstance()->getWinSize().width * -1.f;
	newX -= _dragMovement;

	for (auto l : _internalLayer->getChildren())
	{
		auto ac = ax::MoveBy::create(1.5f, {newX, _internalLayer->getPositionY()});
		l->runAction(ax::EaseElasticOut::create(ac));
	}

	_currentPage = _currentPage + 1 >= _layers.size() ? 0 : _currentPage + 1;
}

void BoomScrollLayer::changePageLeft()
{
	_layers[_rightPage]->retain();
	_internalLayer->removeChild(_layers[_rightPage]);

	_rightPage = _rightPage - 1 < 0 ? _layers.size() - 1 : _rightPage - 1;
	_leftPage = _leftPage - 1 < 0 ? _layers.size() - 1 : _leftPage - 1;

	_internalLayer->addChild(_layers[_leftPage]);
	_layers[_leftPage]->setPositionX(Director::getInstance()->getWinSize().width * -2.f);
	_layers[_leftPage]->release();

	float newX = Director::getInstance()->getWinSize().width * 1.f;
	newX -= _dragMovement;

	for (auto l : _internalLayer->getChildren())
	{
		auto ac = ax::MoveBy::create(1.5f, {newX, _internalLayer->getPositionY()});
		l->runAction(ax::EaseElasticOut::create(ac));
	}

	_currentPage = _currentPage - 1 < 0 ? _layers.size() - 1 : _currentPage - 1;
}

bool BoomScrollLayer::init(std::vector<ax::Layer*> layers, int currentPage)
{
	if (!Layer::init()) return false;

	_totalPages = layers.size();

	currentPage = std::clamp(currentPage, 0, _totalPages);

	_layers = layers;
	_currentPage = currentPage;
	_internalLayer = Layer::create();
	_internalLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto dir = Director::getInstance();
	const auto& winSize = dir->getWinSize();
	if (_internalLayer->getChildrenCount() < _totalPages)
	{
		int i = 0;
		for (auto l : _layers)
		{
			if (!l) return false;
			// l->setPositionX(winSize.width * i);
			// _internalLayer->addChild(l);
			l->retain();
			i++;
		}
	}

	_internalLayer->addChild(_layers[currentPage]);
	_layers[currentPage]->setName("1");
	_layers[currentPage]->release();

	_leftPage = currentPage - 1 < 0 ? _layers.size() - 1 : currentPage - 1;
	_internalLayer->addChild(_layers[_leftPage]);
	_layers[_leftPage]->setPositionX(winSize.width * -1.f);
	_layers[_leftPage]->release();

	_rightPage = currentPage + 1 >= _layers.size() ? 0 : currentPage + 1;
	_internalLayer->addChild(_layers[_rightPage]);
	_layers[_rightPage]->setPositionX(winSize.width);
	_layers[_rightPage]->release();

	//selectPage(currentPage);

	addChild(_internalLayer);

	// auto listener = EventListenerTouchOneByOne::create();

	// listener->setEnabled(true);
	// listener->setSwallowTouches(true);

	//trigger when you start touch
	
	// listener->onTouchBegan = AX_CALLBACK_2(BoomScrollLayer::onTouchBegan, this);
	// listener->onTouchEnded = AX_CALLBACK_2(BoomScrollLayer::onTouchEnded, this);
	// listener->onTouchMoved = AX_CALLBACK_2(BoomScrollLayer::onTouchMoved, this);

	// dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

/*
bool BoomScrollLayer::onTouchBegan(ax::Touch* touch, ax::Event* event)
{
	//GameToolbox::log("began");
	return true;
}

void BoomScrollLayer::onTouchEnded(ax::Touch* touch, ax::Event* event)
{
	//GameToolbox::log("ended");
	if (_dragMovement < -30) changePageRight();
	else if (_dragMovement > 30) changePageLeft();
	_dragMovement = 0;
}

void BoomScrollLayer::onTouchMoved(ax::Touch* touch, ax::Event* event)
{
	auto touchPos = touch->getLocationInView();
	auto previous = touch->getPreviousLocationInView();
	for (auto l : _internalLayer->getChildren())
	{
		auto layerPos = l->getPosition();
		l->setPositionX(layerPos.x + (touchPos.x - previous.x));
	}

	_dragMovement += touchPos.x - previous.x;

	GameToolbox::log("{}", _dragMovement);
}
*/

void BoomScrollLayer::onExit()
{
	for (auto layer : _layers)
	{
		if (layer->getParent() == nullptr) 
			layer->autorelease();
	}
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	Layer::onExit();
}