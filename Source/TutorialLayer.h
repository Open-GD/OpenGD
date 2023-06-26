#pragma once
#include "2d/CCLayer.h"
#include "2d/CCNode.h"
#include "2d/CCLayer.h"
#include "CCDirector.h"
#include "2d/CCLabel.h"
#include "2d/CCMenu.h"

#include "PopupLayer.h"

class TutorialLayer : public PopupLayer
{
private:
	int currentTutorialIndex;
	ax::Layer* layer1;
	ax::Label* tap_label;
	ax::Label* hold_label;
public:
	bool init();
	void onNext();
	static TutorialLayer* create();
};
