#include "BoomScrollLayer.h"
#include "GJGameLevel.h"
#include "MenuItemSpriteExtra.h"
#include "AudioEngine.h"
#include "GameToolbox.h"

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
	GameToolbox::log("current page: {}", current);
	//_internalLayer->setPositionX(Director::getInstance()->getWinSize().x * current);
	auto winSize = Director::getInstance()->getWinSize();
	auto pages = _layers;
	auto count = pages.size();
	
	for(int i = current; i < count; i++)
	{
		float newX = winSize.width * (i - 1);
		GameToolbox::log("1 {}", i);
		auto layer = pages.at(i);
		layer->setPositionX(newX);
		GameToolbox::log("setting {} to {}", i, newX);
	}
	
	for(int i = current - 1; i >= 0; i--)
	{
		float newX = (winSize.width * (i + 1)) * -1;
		GameToolbox::log("2 {}", i);
		auto layer = pages.at(i);
		layer->setPositionX(newX);
		GameToolbox::log("setting {} to {}", i, newX);
	}
}

bool BoomScrollLayer::init(std::vector<ax::Layer*> layers, int currentPage)
{
	if(!Layer::init()) return false;
	
	_totalPages = layers.size();
	if(currentPage > _totalPages) return false;
	
	_layers = layers;
	_currentPage = currentPage;
	_internalLayer = Layer::create();
	_internalLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto dir = Director::getInstance();
	auto winSize = dir->getWinSize();


	if(_internalLayer->getChildrenCount() < _totalPages)
	{
		int i = 0;
		for(auto l : _layers)
		{
			if(!l) return false;
			l->setPositionX(winSize.width * i);
			_internalLayer->addChild(l);
			i++;
		}
	}
	
	selectPage(currentPage);
	
	addChild(_internalLayer);
	
	auto listener = EventListenerTouchOneByOne::create();

	listener->setEnabled(true);
	listener->setSwallowTouches(true);

	// trigger when you start touch
	listener->onTouchBegan = AX_CALLBACK_2(BoomScrollLayer::onTouchBegan, this);
	listener->onTouchEnded = AX_CALLBACK_2(BoomScrollLayer::onTouchEnded, this);
	listener->onTouchMoved = AX_CALLBACK_2(BoomScrollLayer::onTouchMoved, this);

	dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	
	return true;
}

bool BoomScrollLayer::onTouchBegan(ax::Touch* touch, ax::Event *event)
{
	GameToolbox::log("began");
	return true;
}

void BoomScrollLayer::onTouchEnded(ax::Touch* touch, ax::Event *event)
{
	GameToolbox::log("ended");
}

void BoomScrollLayer::onTouchMoved(ax::Touch* touch, ax::Event *event)
{
	auto touchPos = touch->getLocationInView();
	auto previous = touch->getPreviousLocationInView();
	auto layerPos = _internalLayer->getPosition();
	
	_internalLayer->setPositionX(layerPos.x + (touchPos.x - previous.x));
	
	GameToolbox::log("moved");
}

void BoomScrollLayer::onExit()
{
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	Layer::onExit();
}