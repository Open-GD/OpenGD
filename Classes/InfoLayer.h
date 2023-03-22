#pragma once

#include <axmol.h>
#include "GJGameLevel.h"
#include "PopupLayer.h"
#include "LevelTools.h"

class InfoLayer : public PopupLayer
{
public:
	static InfoLayer* create(std::vector<std::string> levelinfo);

	bool init(std::vector<std::string> levelinfo);
};

