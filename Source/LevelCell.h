#pragma once
#include "ui/UIWidget.h"

class GJGameLevel;

namespace ax 
{ 
	class Layer;
	class LayerColor; 
}


class LevelCell : public ax::ui::Widget {
public:
	ax::LayerColor* _bgColor;
	ax::Layer* _layer;

	void updateBGColor(int num);
	static LevelCell* create(GJGameLevel* level);
	bool init(GJGameLevel* level);
};