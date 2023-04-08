#pragma once

#include <axmol.h>
#include "GroundLayer.h"
#include "GJGameLevel.h"
#include "BoomScrollLayer.h"

class LevelSelectLayer : public ax::Layer {
private:
	ax::Sprite* _background;
	GroundLayer* _ground;
public:
	BoomScrollLayer* _bsl;

public:

	static ax::Scene* scene(int page);
	bool init(int page);
	static LevelSelectLayer* create(int page);
};