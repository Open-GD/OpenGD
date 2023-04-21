#include "LevelDebugLayer.h"
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "LevelSearchLayer.h"
#include <AudioEngine.h>
#include "LevelSelectLayer.h"

USING_NS_AX;
using namespace ax::network;

LevelDebugLayer* LevelDebugLayer::create(GJGameLevel* level)
{
	LevelDebugLayer* ret = new LevelDebugLayer();
	if (ret->init(level))
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

ax::Scene* LevelDebugLayer::scene(GJGameLevel* level) {
	auto scene = Scene::create();
	scene->addChild(LevelDebugLayer::create(level));
	return scene;
}
bool LevelDebugLayer::init(GJGameLevel* level)
{
	if (!Layer::init())
		return false;

	if (_bgl = BaseGameLayer::create(level); !_bgl)
		return false;
	addChild(_bgl);

	auto dir = Director::getInstance();
	auto winSize = dir->getWinSize();

	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [this](Node const* btn) {this->exit(); });
	auto backMenu = Menu::create(backBtn, nullptr);
	backMenu->setPosition({25.0f, winSize.height - 22.0f});
	addChild(backMenu);


	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [this](EventKeyboard::KeyCode code, Event const*)
	{
		using enum ax::EventKeyboard::KeyCode;
		if (code == KEY_ESCAPE)
		{
			this->exit();
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void LevelDebugLayer::exit() {

	AudioEngine::stopAll();
	AudioEngine::play2d("quitSound_01.ogg", false, 0.1f);
	AudioEngine::play2d("menuLoop.mp3", true, 0.2f);

	int id = _bgl->_level->_levelID;
	if (id <= 0 || id > 22)
		return GameToolbox::popSceneWithTransition(0.5f);

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, LevelSelectLayer::scene(id - 1)));
}