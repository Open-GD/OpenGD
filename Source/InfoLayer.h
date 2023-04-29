#pragma once

#include "PopupLayer.h"

class GJGameLevel;

class InfoLayer : public PopupLayer
{
public:
	static InfoLayer* create(GJGameLevel* level);

	bool init(GJGameLevel* level);
};

