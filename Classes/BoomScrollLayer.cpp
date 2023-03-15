#include "BoomScrollLayer.h"
#include "GJGameLevel.h"
#include "MenuItemSpriteExtra.h"
#include "AudioEngine.h"
#include "PlayLayer.h"

USING_NS_AX;

Scene* BoomScrollLayer::scene()
{
	auto scene = Scene::create();
	scene->addChild(BoomScrollLayer::create());
	return scene;
}

bool BoomScrollLayer::init()
{
	return true;
}