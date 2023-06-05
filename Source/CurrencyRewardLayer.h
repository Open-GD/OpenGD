#pragma once

#include <string>
#include <vector>

#include "2d/CCLayer.h"


class CurrencyRewardLayer : public ax::Layer {
public:
	static CurrencyRewardLayer* create(int orbs, int stars, int diamonds);

	bool init(int orbs, int stars, int diamonds);
};