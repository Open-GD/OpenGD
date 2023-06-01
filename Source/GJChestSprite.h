#pragma once

#include "2d/CCSprite.h"
#include "ccTypes.h"



class GJChestSprite : public ax::Sprite {
public:
	static GJChestSprite* create(int ChestID);

	static int _chestID;

	bool switchState(int state, bool idk);
};