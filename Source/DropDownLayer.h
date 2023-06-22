#pragma once
#include "2d/CCLayer.h"

class MenuItemSpriteExtra;

namespace ax 
{ 
	class Node; 
}

class DropDownLayer : public ax::LayerColor {
protected:
	virtual bool init(ax::Node* scrollLayer, const char* label);
	virtual void customSetup() {};
	MenuItemSpriteExtra *_backBtn;
public:
	ax::Layer* _dropLayer;

	static DropDownLayer* create(ax::Node* scrollLayer, const char* label);
	void showLayer(bool attachToScene, bool bounce);
	void hideLayer();
	virtual void hideBackButton();
	void keyBackClicked();
};