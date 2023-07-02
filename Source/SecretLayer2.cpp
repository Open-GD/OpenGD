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

#include "SecretLayer2.h"
#include "2d/Menu.h"
#include "2d/Transition.h"
#include <AudioEngine.h>

#include "Director.h"
#include "EventListenerKeyboard.h"
#include "EventDispatcher.h"
#include "2d/Menu.h"
#include "MenuItemSpriteExtra.h"
#include "2d/Transition.h"
#include "2d/Label.h"
#include "CreatorLayer.h"

USING_NS_AX;

Scene* SecretLayer2::scene() {
	return SecretLayer2::create();
}

SecretLayer2* SecretLayer2::create()
{
	SecretLayer2* ret = new SecretLayer2();
	if (ret->init())
	{
		ret->autorelease();
		return ret;
	} 
	else
	{
		delete ret;
		ret = nullptr;
		return nullptr;
	}
}

bool SecretLayer2::init()
{
    if (!Scene::init()) return false;

    auto director = Director::getInstance();
	const auto& winSize = director->getWinSize();

    auto menu = Menu::create();

	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [](Node*) {
		Director::getInstance()->replaceScene(TransitionFade::create(.5, CreatorLayer::scene()));
		AudioEngine::stopAll();
		AudioEngine::play2d("menuLoop.mp3", true, 0.2f);
	});
	backBtn->setPosition(menu->convertToNodeSpace({ 24.0, winSize.height - 23.0f }));
	menu->addChild(backBtn);

    AudioEngine::stopAll();
    AudioEngine::play2d("secretLoop02.mp3", true, 0.2f);

    Sprite* gradient = Sprite::create("GJ_gradientBG.png");
    gradient->setAnchorPoint({0.0f, 0.0f});
    gradient->setScaleX((winSize.width + 10.0f) / gradient->getTextureRect().size.width);
	gradient->setScaleY((winSize.height + 10.0f) / gradient->getTextureRect().size.height);
	gradient->setPosition({-5.0f, -5.0f});
    gradient->setColor(Color3B(75, 0, 100));
    this->addChild(gradient);

    auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = AX_CALLBACK_2(SecretLayer2::onKeyPressed, this);

	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    this->addChild(menu);

    return true;
}

void SecretLayer2::onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event) {
	switch (keyCode) 
	{
	case EventKeyboard::KeyCode::KEY_BACK:
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, CreatorLayer::scene()));
		AudioEngine::stopAll();
		AudioEngine::play2d("menuLoop.mp3", true, 0.2f);
		break;
	default:
		break;
	}
}
