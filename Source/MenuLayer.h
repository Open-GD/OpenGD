#pragma once

#include "2d/CCLayer.h"

class MenuItemSpriteExtra;
class MenuGameLayer;

namespace ax 
{ 
	class Scene; 
	class Label;
}

extern bool music;

class MenuLayer : public ax::Layer 
{
public:
	static bool music;
	static ax::Scene* scene();
	bool init();
	static MenuLayer* create();

	MenuGameLayer *_mgl;	
	MenuItemSpriteExtra* _profileBtn;
	ax::Label* _profileLabel;
};