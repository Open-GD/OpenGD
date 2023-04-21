#include "LevelDebugLayer.h"
#include "GameToolbox.h"
#include "LevelSearchLayer.h"
#include "LevelSelectLayer.h"
#include "MenuItemSpriteExtra.h"
#include <AudioEngine.h>
#include <ccMacros.h>

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

ax::Scene* LevelDebugLayer::scene(GJGameLevel* level)
{
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

	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [this](Node const* btn) { this->exit(); });
	auto backMenu = Menu::create(backBtn, nullptr);
	backMenu->setPosition({25.0f, winSize.height - 22.0f});
	addChild(backMenu);

	scheduleUpdate();

	return true;
}

void LevelDebugLayer::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerKeyboard::create();
	auto dir = Director::getInstance();
	listener->onKeyPressed = AX_CALLBACK_2(LevelDebugLayer::onKeyPressed, this);
	listener->onKeyReleased = AX_CALLBACK_2(LevelDebugLayer::onKeyReleased, this);

	dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void LevelDebugLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	GameToolbox::log("Key with keycode {} pressed", static_cast<int>(keyCode));
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_A: {
		if (_camInput.x > -1.f)
			_camInput.x -= 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_D: {
		if (_camInput.x < 1.f)
			_camInput.x += 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_W: {
		if (_camInput.y < 1.f)
			_camInput.y += 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_S: {
		if (_camInput.y > -1.f)
			_camInput.y -= 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_ESCAPE: {
		this->exit();
	}
	break;
	}
}

void LevelDebugLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	GameToolbox::log("Key with keycode {} released", static_cast<int>(keyCode));
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_A: {
		if (_camInput.x < 1.f)
			_camInput.x += 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_D: {
		if (_camInput.x > -1.f)
			_camInput.x -= 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_W: {
		if (_camInput.y > -1.f)
			_camInput.y -= 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_S: {
		if (_camInput.y < 1.f)
			_camInput.y += 1.f;
	}
	break;
	}
}

void LevelDebugLayer::update(float delta)
{
	auto cam = Camera::getDefaultCamera();
	cam->setPosition(cam->getPosition() + (_camInput * _camSpeed * delta));
}

void LevelDebugLayer::exit()
{

	AudioEngine::stopAll();
	AudioEngine::play2d("quitSound_01.ogg", false, 0.1f);
	AudioEngine::play2d("menuLoop.mp3", true, 0.2f);

	int id = _bgl->_level->_levelID;
	if (id <= 0 || id > 22)
		return GameToolbox::popSceneWithTransition(0.5f);

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, LevelSelectLayer::scene(id - 1)));
}