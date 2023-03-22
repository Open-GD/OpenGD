#pragma once

#include <axmol.h>
#include "GJGameLevel.h"

class LevelInfoLayer : public ax::Layer
{
public:
	static LevelInfoLayer* create(GJGameLevel* level);

	static ax::Scene* scene(GJGameLevel* level);
	bool init(GJGameLevel* level);
};
