#pragma once

#include <axmol.h>
#include <ui/UIWidget.h>
#include "SongObject.h"

class SongCell : public ax::ui::Widget
{
private:
	ax::Layer* _layer;
	ax::LayerColor* _bgColor;
public:
	static SongCell* create(SongObject*);
	void updateBGColor(int);
	bool init(SongObject*);
};
