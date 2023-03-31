#pragma once
#include <axmol.h>
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "ListLayer.h"

class DropDownLayer : public ax::LayerColor {
protected:
	bool init(Node* scrollLayer, const char* label);
	virtual void customSetup() {};
public:
	ax::Layer* _dropLayer;

	static DropDownLayer* create(Node* scrollLayer, const char* label);
	void showLayer();
	void hideLayer();
};