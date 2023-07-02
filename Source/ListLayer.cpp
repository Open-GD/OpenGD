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

#include "ListLayer.h"


#include "Director.h"
#include "2d/Label.h"
#include "GameToolbox/getTextureString.h"

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
	return ListLayer::create(scrollLayer, label, color, {356, 220});
}

ListLayer* ListLayer::create(ax::Node* scrollLayer, const char* label){
	return ListLayer::create(scrollLayer, label, {0, 0, 0, 180}, {356, 220});
}

bool ListLayer::init(ax::Node* scrollLayer, const char* label, ax::Color4B color, ax::Vec2 size){
	if(!this->initWithColor(color)) return false;
	
	const auto& winSize = ax::Director::getInstance()->getWinSize();

	this->setContentSize(size);
	
	//menu start
	if(scrollLayer != nullptr){
		scrollLayer->setPosition({size.x / 2, size.y / 2.0f});
		scrollLayer->setContentSize(size);
		this->addChild(scrollLayer);
	}

	auto bottom = ax::Sprite::createWithSpriteFrameName("GJ_table_bottom_001.png");
	bottom->setPosition({size.x / 2, -10});
	

	auto top = ax::Sprite::createWithSpriteFrameName("GJ_table_top_001.png");
	top->setPosition({size.x / 2, size.y + 15});
	

	auto left = ax::Sprite::createWithSpriteFrameName("GJ_table_side_001.png");
	left->setPosition(this->convertToNodeSpace({-20, 0}));
	left->setAnchorPoint({0, 0});
	left->setScaleY(size.height / left->getContentSize().height);
	

	auto right = ax::Sprite::createWithSpriteFrameName("GJ_table_side_001.png");
	right->setPosition(this->convertToNodeSpace({size.x + 20, 0}));
	right->setAnchorPoint({1, 0});
	right->setScaleY(left->getScaleY());
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
	
	return true;
}