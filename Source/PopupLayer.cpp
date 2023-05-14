#include "PopupLayer.h"

#include <2d/CCActionEase.h>
#include <2d/CCActionInterval.h>
#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListenerTouch.h>

USING_NS_AX;

void PopupLayer::show()
{
	this->_mainLayer->setScale(0.1f);
	this->_mainLayer->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, 1.0f), 0.6f));
	this->runAction(FadeTo::create(0.14, 100));

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


void PopupLayer::keyBackClicked() {
	this->close();
}

void PopupLayer::close()
{
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	this->removeFromParent();
}
