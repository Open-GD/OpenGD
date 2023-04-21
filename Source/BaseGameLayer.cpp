#include "BaseGameLayer.h"

USING_NS_AX;

BaseGameLayer* BaseGameLayer::create(GJGameLevel* level)
{
	BaseGameLayer* ret = new BaseGameLayer();
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

bool BaseGameLayer::init(GJGameLevel* level)
{
	if (!Layer::init())
		return false;

	_level = level;

	return true;
}