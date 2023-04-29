#pragma once
#include "2d/CCLayer.h"

class GroundLayer;
class BoomScrollLayer;

namespace ax 
{ 
	class Sprite;
	class Scene;
}


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