/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

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