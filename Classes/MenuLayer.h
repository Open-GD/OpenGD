#pragma once

#include <axmol.h>

extern bool music;

class MenuLayer : public ax::Layer {
public:
	static ax::Scene* scene();
	bool init();
	CREATE_FUNC(MenuLayer);
};