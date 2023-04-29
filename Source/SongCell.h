#pragma once

#include "ui/UIWidget.h"

class SongObject;

namespace ax 
{
	class Layer; 
	class LayerColor;
}


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
