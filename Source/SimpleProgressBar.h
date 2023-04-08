#pragma once

#include <axmol.h>

// opengd addition
class SimpleProgressBar : public ax::Layer {
protected:
	ax::Sprite *_pGroove;
	ax::Sprite *_pBar;

	float _pBarWidth;
public:
	CREATE_FUNC(SimpleProgressBar);
	virtual bool init();

	void update(float delta);

	AX_SYNTHESIZE(float, _pPercentage, Percentage);
};