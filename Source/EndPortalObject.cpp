#include "EndPortalObject.h"
#include "base/CCDirector.h"

USING_NS_AX;


EndPortalObject* EndPortalObject::create()
{
	auto pRet = new (std::nothrow) EndPortalObject();

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool EndPortalObject::init()
{
	if(!GameObject::init("block001_01_001.png"))
		return false;
	
	auto winSize = Director::getInstance()->getWinSize();
	_gradientBar = Sprite::createWithSpriteFrameName("gradientBar.png");
	return true;
}