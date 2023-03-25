#pragma once
#include <axmol.h>
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "ListLayer.h"

class DropDownLayer : public ax::LayerColor {
	private:
		bool init(ax::Layer* scrollLayer, const char* label);
	public:
		ax::Layer* _dropLayer;

		static DropDownLayer* create(ax::Layer* scrollLayer, const char* label);
		void showLayer();
		void hideLayer();
};