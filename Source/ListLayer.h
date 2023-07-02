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
#include "2d/Layer.h"
#include "Types.h"
#include "math/Vec2.h"

namespace ax 
{ 
	class Node; 
}


class ListLayer : public ax::LayerColor {
	private:		
		bool init(ax::Node* scrollLayer, const char* label, ax::Color4B color, ax::Vec2 size);
	public:
		static ListLayer* create(ax::Node* scrollLayer, const char* label, ax::Color4B color, ax::Vec2 size);
		static ListLayer* create(ax::Node* scrollLayer, const char* label, ax::Color4B color);
		static ListLayer* create(ax::Node* scrollLayer, const char* label);
};