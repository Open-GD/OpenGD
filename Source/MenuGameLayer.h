#pragma once
#include "2d/CCLayer.h"
#include "math/Vec2.h"
#include "CCPlatformMacros.h"

class PlayerObject;
class GroundLayer;

namespace ax 
{ 
	class Sprite; 
	class Menu;
	class Scene;
}



class MenuGameLayer : public ax::Layer {
public:
	PlayerObject* player;
	ax::Vec2 startPos;
	GroundLayer* groundLayer;
	ax::Sprite* bgSpr;
	ax::Menu* bgSprites;
	float bgStartPos;
	float sep;
	float bsizeX;

public:
	void processPlayerMovement(float delta);
	void processBackground(float delta);
	void update(float delta);
	void updateVisibility(float delta);

	static ax::Scene* scene();
	bool init();
	CREATE_FUNC(MenuGameLayer);
};