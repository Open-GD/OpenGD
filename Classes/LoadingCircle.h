#pragma once

#include <axmol.h>

class LoadingCircle : public ax::Layer {
public:
	AX_SYNTHESIZE(ax::Sprite *, m_pCircle, Circle);

	bool init();
	CREATE_FUNC(LoadingCircle);

	void update(float delta);
	void removeMeAndCleanup();
};
