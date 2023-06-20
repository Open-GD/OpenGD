#pragma once

#include "2d/CCLayer.h"

enum Transitions
{
	kNone,
	kScaleUp
};

class PopupLayer : public ax::LayerColor {
public:
	ax::Layer* _mainLayer;

	virtual void setup() {};
	
	bool init();
	void keyBackClicked();

	virtual void show(Transitions = kScaleUp);
	virtual void showOnLayer(ax::Node*, Transitions = kScaleUp);
	virtual void close();
};
