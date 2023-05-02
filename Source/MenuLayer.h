#pragma once

#include <axmol.h>
#include "MenuGameLayer.h"

class MenuItemSpriteExtra;

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