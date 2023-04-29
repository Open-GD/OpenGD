#pragma once
#include "2d/CCLayer.h"
#include "CCPlatformMacros.h"

namespace ax 
{ 
	class Sprite; 
}


class LoadingCircle : public ax::Layer {
public:
	AX_SYNTHESIZE(ax::Sprite *, m_pCircle, Circle);

	bool init();
	static LoadingCircle* create();
	void update(float delta);
	void removeMeAndCleanup();
};
