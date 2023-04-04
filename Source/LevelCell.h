#pragma once
#include <axmol.h>
#include "GJGameLevel.h"
#include "ui/UIWidget.h"
#include "LevelTools.h"

class LevelCell : public ax::ui::Widget {
public:
	ax::LayerColor* BGColor;
	ax::Layer* layer;

	void updateBGColor(int num);
	static LevelCell* create(GJGameLevel* level);
	bool init(GJGameLevel* level);
};