#pragma once

#include "2d/CCLayer.h"

enum Transitions
{
	kScaleUp,
	kNone,
	kFadeIn
};

class PopupLayer : public ax::LayerColor {
public:
	ax::Layer* _mainLayer;

	virtual void setup() {};
	
	bool init();
	void keyBackClicked();

	virtual void show(Transitions = kScaleUp);
	virtual void close();
};