#pragma once

#include <axmol.h>
#include "GJGameLevel.h"
#include "PopupLayer.h"

class InfoLayer : public PopupLayer
{
public:
	static InfoLayer* create(GJGameLevel* level);

	bool init(GJGameLevel* level);
};

