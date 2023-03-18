#pragma once

#include <axmol.h>
#include "GJGameLevel.h"

class LevelPage : public ax::Layer
{
private:
	GJGameLevel* _level;
public:
	static bool replacingScene;
	bool init(GJGameLevel* level);
	static LevelPage* create(GJGameLevel* level);
};