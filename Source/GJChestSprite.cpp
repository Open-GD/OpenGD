#include "GJChestSprite.h"
#include "GameToolbox.h"
#include "fmt/format.h"

USING_NS_AX;

int GJChestSprite::_chestID = 0;

GJChestSprite* GJChestSprite::create(int chestID) {
	auto pRet = new (std::nothrow) GJChestSprite();
	_chestID = chestID;

	if (pRet && pRet->switchState(2, false)) {
		pRet->autorelease();
		return pRet;
	}
	AX_SAFE_DELETE(pRet);
	return pRet;
}

bool GJChestSprite::switchState(int state, bool idk)
{
	if (state == 2)
	{
		Sprite* chestSprite = Sprite::createWithSpriteFrameName(fmt::format("chest_0{}_0{}_001.png", _chestID, state));
		this->addChild(chestSprite);
	}
	return true;
}