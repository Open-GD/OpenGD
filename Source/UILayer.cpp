#include "UILayer.h"
#include "PlayLayer.h"
#include "GameToolbox.h"
#include "CCEventListenerTouch.h"
#include "CCEventDispatcher.h"
#include "base/CCDirector.h"

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
	GameToolbox::log("as-fsaf");
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