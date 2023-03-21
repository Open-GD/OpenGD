#pragma once

#include <axmol.h>
#include "GJGameLevel.h"

class LevelPage : public ax::Layer
{
private:
	
public:
	GJGameLevel* _level;
	static bool replacingScene;
	static ax::Scene *scene(GJGameLevel *level);
	bool init(GJGameLevel* level);
	static LevelPage* create(GJGameLevel* level);
};