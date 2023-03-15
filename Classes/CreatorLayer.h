#pragma once
#include <axmol.h>
class CreatorLayer : public ax::Scene {
public:
	CREATE_FUNC(CreatorLayer)
	bool init();
	static ax::Scene* scene();
	void onDrawImGui();
};