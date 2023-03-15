#pragma once

#include <axmol.h>

class BoomScrollLayer : public ax::Layer {
private:
	std::vector<ax::Layer*> _layers;

public:
	static ax::Scene* scene();
	bool init();
	CREATE_FUNC(BoomScrollLayer);
};