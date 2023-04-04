#include "DropDownLayer.h"

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
	auto backBtn = MenuItemSpriteExtra::create(backSpr, [&](Node* btn){
		this->hideLayer();
	});

	backBtn->setPosition(menu->convertToNodeSpace({24, winSize.height - 23}));
	menu->addChild(backBtn);
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
	listener->onTouchBegan = [=](Touch*, Event*) -> bool {
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void DropDownLayer::showLayer()
{
	auto winSize = Director::getInstance()->getWinSize();

	this->runAction(FadeTo::create(0.5, 125));
	this->_dropLayer->runAction(EaseInOut::create(MoveTo::create(0.5f, {this->getPositionX(), 0}), 2));

	auto scene = Director::getInstance()->getRunningScene();
	scene->addChild(this);
}

void DropDownLayer::hideLayer(){
	auto winSize = Director::getInstance()->getWinSize();

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