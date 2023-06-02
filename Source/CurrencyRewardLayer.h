#pragma once

#include <string>
#include <vector>

#include "2d/CCLayer.h"


class CurrencyRewardLayer : public ax::Layer {
public:
	static CurrencyRewardLayer* create();

	bool init();
};