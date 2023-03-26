#pragma once

#include <axmol.h>

class MenuItemSpriteExtra;

extern bool music;

class MenuLayer : public ax::Layer 
{
public:
	static bool music;
	static ax::Scene* scene();
	bool init();
	static MenuLayer* create();
	
	MenuItemSpriteExtra* _profileBtn;
	ax::Label* _profileLabel;
};