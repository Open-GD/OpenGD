#pragma once

#include <axmol.h>
#include "GJGameLevel.h"

class LevelPage : public ax::Layer
{
	
public:
	GJGameLevel* _level;
	bool _openBGL = false;
	static bool replacingScene;
	bool init(GJGameLevel* level);
	static LevelPage* create(GJGameLevel* level);

	void onPlay(ax::Node*);
};