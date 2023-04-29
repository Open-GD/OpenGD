#pragma once

#include "2d/CCLayer.h"
#include "CCPlatformMacros.h"

namespace ax 
{ 
	class Sprite; 
}


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