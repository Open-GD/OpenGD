#pragma once
#include <axmol.h>

class UILayer : public ax::Layer {
public:
	static UILayer* create();
	bool init();

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);
	void onTouchEnded(ax::Touch* touch, ax::Event* event);
};