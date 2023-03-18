#include "DropDownLayer.h"

#define SETUP_SWALLOW_LISTENER(node) auto listener = ax::EventListenerTouchOneByOne::create();\
	listener->setEnabled(true);\
	listener->setSwallowTouches(true);\
	listener->onTouchBegan = [=](ax::Touch*, ax::Event*) -> bool {\
		return true;\
	};\
	ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, node)

#define REMOVE_SWALLOW_LISTENER(node) ax::Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(node)

DropDownLayer* DropDownLayer::create(){
    auto pRet = new(std::nothrow) DropDownLayer();

	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool DropDownLayer::init(){
    if(!this->initWithColor({0, 0, 0, 0})) return false;
    
    auto winSize = ax::Director::getInstance()->getWinSize();

    this->droplayer = ax::Layer::create();
    droplayer->setPosition({0, this->droplayer->getContentSize().height});
    
    //menu start
    auto menu = ax::Menu::create();

    auto backSpr = ax::Sprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto backBtn = MenuItemSpriteExtra::create(backSpr, [&](Node* btn){
        this->hideLayer();
    });

    backBtn->setPosition(menu->convertToNodeSpace({25, winSize.height - 25}));
    menu->addChild(backBtn);
    droplayer->addChild(menu);
    //menu end
    
    this->addChild(droplayer);

    SETUP_SWALLOW_LISTENER(this);

    return true;
}

void DropDownLayer::showLayer(){
    auto winSize = ax::Director::getInstance()->getWinSize();

    this->runAction(ax::FadeTo::create(0.5, 125));
    this->droplayer->runAction(
        ax::EaseInOut::create(ax::MoveTo::create(0.5f, {this->getPositionX(), 0}), 2)
    );

    auto scene = ax::Director::getInstance()->getRunningScene();
	scene->addChild(this);
}

void DropDownLayer::hideLayer(){
    auto winSize = ax::Director::getInstance()->getWinSize();

    this->runAction(ax::FadeTo::create(0.5, 0));
    this->droplayer->runAction(
        ax::Sequence::create(
            ax::EaseInOut::create(ax::MoveTo::create(0.5f, {0, this->droplayer->getContentSize().height}), 2),
            ax::CallFunc::create([&](){
                	REMOVE_SWALLOW_LISTENER(this);
	                this->removeFromParent();
            }),
            nullptr
        )
    );
}