#pragma once

#include "2d/CCLayer.h"

class GJGameLevel;
namespace ax 
{ 
	class Node; 
}

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