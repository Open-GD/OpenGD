#pragma once

#include <axmol.h>

extern bool music;

class MenuLayer : public ax::Layer 
{
public:
	static bool music;
	static ax::Scene* scene();
	bool init();
	static MenuLayer* create();
};