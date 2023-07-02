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

#include "MenuGameLayer.h"

#include "GroundLayer.h"
#include "2d/Menu.h"
#include "PlayerObject.h"
#include "base/Director.h"
#include "EventListenerTouch.h"
#include "EventDispatcher.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/rand.h"

USING_NS_AX;

MenuGameLayer* MenuGameLayer::create()
{
	MenuGameLayer* pRet = new MenuGameLayer();
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

Scene* MenuGameLayer::scene() {
	auto scene = Scene::create();
	scene->addChild(MenuGameLayer::create());
	return scene;
}

bool MenuGameLayer::init(){
	if (!Layer::init()) return false;

	this->startPos = Vec2(0, 105);

	auto dir = Director::getInstance();
	const auto& winSize = dir->getWinSize();

	groundLayer = GroundLayer::create(1);
	
	addChild(groundLayer, 2);
	
	//should work but doesnt?
	auto playerTest = PlayerObject::create(GameToolbox::randomInt(1,14), this);
	playerTest->setMainColor(GameToolbox::randomColor3B());
	playerTest->setSecondaryColor(GameToolbox::randomColor3B());
	playerTest->setPosition({-100, 93.f});
	addChild(playerTest, 16);
	this->player = playerTest;

	const Texture2D::TexParams texParams = {
		backend::SamplerFilter::LINEAR, 
		backend::SamplerFilter::LINEAR, 
		backend::SamplerAddressMode::REPEAT, 
		backend::SamplerAddressMode::REPEAT
	};

	bgSprites = Menu::create();

	// this->bgSpr = bg;
	// this->bgSpr->getTexture()->setTexParameters(texParams);
	// this->bgSpr->setTextureRect(Rect(0, 0, 2048, 1024));
	// this->bgSpr->setPosition(winSize / 2);

	for(int i = 0; i < 4; i++) {
		auto gr = Sprite::create(GameToolbox::getTextureString("game_bg_01_001.png"));
		gr->setStretchEnabled(false);
		gr->getTexture()->setTexParameters(texParams);
		gr->setTextureRect(Rect(0, 0, 2048, 1024));
		gr->setPosition(winSize / 2);
		gr->setColor({ 0, 102, 255 });
		// bg scale
		gr->setScale(1.185f); // epic hardcore (please fix lmao)
		bsizeX = gr->getContentSize().width;

		this->bgSprites->addChild(gr);
	}
	this->bgSprites->alignItemsHorizontallyWithPadding(0);
	this->bgStartPos = bgSprites->getPositionX();
	this->addChild(bgSprites, -3);
	sep = 0.3f;
	bgSprites->setScale(winSize.width / bgSprites->getContentSize().width);
	bgSprites->setPositionY(0);

	//this is not how it works lol
	// bgSprites->setColor(GameToolbox::randomColor3B());

	// // this->addChild(this->bgSpr, -1);
	
	// bgSprites->runAction(
	// 	 RepeatForever::create(
	// 		 Sequence::create(
	// 			 TintTo::create(4.0f, {255, 0, 0}),
	// 			 TintTo::create(4.0f, {255, 255, 0}),
	// 			 TintTo::create(4.0f, {0, 255, 0}),
	// 			 TintTo::create(4.0f, {0, 255, 255}),
	// 			 TintTo::create(4.0f, {0, 0, 255}),
	// 			 TintTo::create(4.0f, {255, 0, 255}),
	// 			 TintTo::create(4.0f, {255, 0, 0}),
	// 			 nullptr
	// 		 )
	// 	 )
	// );
	
	scheduleUpdate();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setEnabled(true);
	listener->setSwallowTouches(true);
	listener->onTouchBegan = AX_CALLBACK_2(MenuGameLayer::onTouchBegan, this);
	dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
void MenuGameLayer::processPlayerMovement(float delta) {
	float step = std::min(2.0f, delta * 60.0f);
	const auto& winSize = Director::getInstance()->getWinSize();
	step /= 4.0f;

	this->player->setPositionX(this->player->getPositionX() + (step * 18.f));

	if(this->player->getPositionX() >= winSize.width + player->getContentSize().width) {
		this->resetPlayer(false);
	}
}

void MenuGameLayer::resetPlayer(bool touched)
{
	float x = touched ? -600.0f : -300.0f;
	player->setPosition({x, 93.f});
	auto mainColor = GameToolbox::randomColor3B();
	auto secondaryColor = GameToolbox::randomColor3B();
	player->setMainColor(mainColor);
	player->setSecondaryColor(secondaryColor);
}

void MenuGameLayer::processBackground(float delta) {
	float xpos =  bgSprites->getPositionX();
	if(this->bgStartPos - xpos < this->bsizeX) 
	{
		bgSprites->setPositionX(xpos - this->sep);
	}
	else 
	{
		bgSprites->setPositionX(xpos + this->bsizeX);
	}
}

void MenuGameLayer::update(float delta) {
	processBackground(delta);
	processPlayerMovement(delta);
	groundLayer->update(delta * 60.0f);
}

//void MenuGameLayer::renderRect(ax::Rect rect, ax::Color4B col)
//{
//	_dnHitbox->drawRect({rect.getMinX(), rect.getMinY()}, {rect.getMaxX(), rect.getMaxY()}, col);
//	_dnHitbox->drawSolidRect({rect.getMinX(), rect.getMinY()}, {rect.getMaxX(), rect.getMaxY()},
//					  Color4B(col.r, col.g, col.b, 100));
//}

bool MenuGameLayer::onTouchBegan(ax::Touch* touch, ax::Event* event)
{
	auto touchPos = touch->getLocation();
	ax::Rect hitbox = player->getBoundingBox();

	//hitbox probably needs to be adjusted because it feels a bit off atm
	//constexpr float hitboxMult = 2.5f;
	//hitbox.origin -= {hitboxMult, hitboxMult};
	//hitbox.size += {hitboxMult * 2, hitboxMult * 2};
	
	if(hitbox.containsPoint(touchPos)) {
		this->resetPlayer(true);
	}
	return true;
}
