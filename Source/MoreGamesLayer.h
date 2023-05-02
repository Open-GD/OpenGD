#pragma once
#include <axmol.h>
#include "PromoItemSprite.h"

class MoreGamesLayer : public ax::Layer {
private:
	bool init();
public:
	static MoreGamesLayer* create();
};