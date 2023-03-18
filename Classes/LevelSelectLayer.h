#pragma once

#include <axmol.h>
#include "GroundLayer.h"
#include <GJGameLevel.h>

class LevelSelectLayer : public ax::Layer {
private:
	ax::Sprite* _background;
	GroundLayer* _ground;

public:
	static std::vector<GJGameLevel*> _levels;
	static ax::Scene* scene();
	bool init();
	CREATE_FUNC(LevelSelectLayer);
};