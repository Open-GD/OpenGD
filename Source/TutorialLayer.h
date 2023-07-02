#pragma once
#include "2d/Layer.h"
#include "2d/Node.h"
#include "2d/Layer.h"
#include "Director.h"
#include "2d/Label.h"
#include "2d/Menu.h"

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
