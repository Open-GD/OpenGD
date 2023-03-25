#include "ListLayer.h"

ListLayer* ListLayer::create(ax::Node* scrollLayer, const char* label, ax::Color4B color, ax::Vec2 size){
	auto pRet = new(std::nothrow) ListLayer();

	if (pRet && pRet->init(scrollLayer, label, color, size)) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

ListLayer* ListLayer::create(ax::Node* scrollLayer, const char* label, ax::Color4B color){
	return ListLayer::create(scrollLayer, label, color, {356, 240});
}

ListLayer* ListLayer::create(ax::Node* scrollLayer, const char* label){
	return ListLayer::create(scrollLayer, label, {0, 0, 0, 125}, {356, 240});
}

bool ListLayer::init(ax::Node* scrollLayer, const char* label, ax::Color4B color, ax::Vec2 size){
	if(!this->initWithColor(color)) return false;
	
	auto winSize = ax::Director::getInstance()->getWinSize();

	//this->listlayer = ax::Layer::create();
	this->setContentSize(size);
	
	//menu start
	if(scrollLayer != nullptr){
		scrollLayer->setPosition({size.x / 2, size.y / 1.5f});
		scrollLayer->setContentSize(size);
		this->addChild(scrollLayer);
	}

	auto bottom = ax::Sprite::createWithSpriteFrameName("GJ_table_bottom_001.png");
	bottom->setPosition({size.x / 2, -10});
	

	auto top = ax::Sprite::createWithSpriteFrameName("GJ_table_top_001.png");
	top->setPosition({size.x / 2, size.y - 5});
	

	auto left = ax::Sprite::createWithSpriteFrameName("GJ_table_side_001.png");
	left->setPosition(this->convertToNodeSpace({-20, 0}));
	left->setAnchorPoint({0, 0});
	left->setScaleY(size.y / left->getContentSize().height);
	

	auto right = ax::Sprite::createWithSpriteFrameName("GJ_table_side_001.png");
	right->setPosition(this->convertToNodeSpace({size.x + 20, 0}));
	right->setAnchorPoint({1, 0});
	right->setScaleY(size.y / left->getContentSize().height);
	right->setFlippedX(true);
	
	this->addChild(left);
	this->addChild(right);
	this->addChild(top);
	this->addChild(bottom);

	auto text = ax::Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), label, ax::TextHAlignment::CENTER);
		
	text->setPositionX(top->getPositionX());
	text->setPositionY(top->getPositionY() * 1.01);
	text->setScale(0.8f);
	this->addChild(text);
	//menu end
	
	//this->addChild(listlayer);

	return true;
}