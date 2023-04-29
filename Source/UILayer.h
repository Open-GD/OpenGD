#pragma once

#include "2d/CCLayer.h"

namespace ax 
{ 
	class Touch; 
	class Event;
}

class UILayer : public ax::Layer {
public:
	static UILayer* create();
	bool init();

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);
	void onTouchEnded(ax::Touch* touch, ax::Event* event);
};