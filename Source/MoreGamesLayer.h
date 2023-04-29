#pragma once

#include "2d/CCLayer.h"

class MoreGamesLayer : public ax::Layer {
private:
	bool init();
public:
	static MoreGamesLayer* create();
};