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
