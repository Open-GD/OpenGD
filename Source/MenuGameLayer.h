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
	class Touch;
	class Event;
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
	void resetPlayer(bool touched);
	bool onTouchBegan(ax::Touch *touch, ax::Event *event);
	void renderRect(ax::Rect rect, ax::Color4B col);


	static ax::Scene* scene();
	static MenuGameLayer* create();
	bool init();
};
