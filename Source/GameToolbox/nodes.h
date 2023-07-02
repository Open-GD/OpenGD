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
#include "Types.h"
#include "enums.h"
#include "Vector.h"

namespace ax
{
	class Label; 
	class Menu;
	class Node;
}


namespace GameToolbox
{
	void popSceneWithTransition(float time, popTransition type = popTransition::kTransitionFade);
	void limitLabelWidth(ax::Label* label, float width, float normalScale, float minScale = 0);
	ax::Label* createBMFont(std::string text, std::string font);
	ax::Label* createBMFont(std::string text, std::string font, int width);
	ax::Label* createBMFont(std::string text, std::string font, int width, ax::TextHAlignment alignment);

	//std::string getTextureString(std::string texture);
	void alignItemsInColumnsWithPadding(ax::Menu* menu, const int rows, const int x_padding, const int y_padding);
	void alignItemsVerticallyWithPadding(ax::Vector<ax::Node*> children, float padding);
	void alignItemsHorizontally(ax::Vector<ax::Node*> children, float padding, ax::Point location);
	void alignItemsHorizontallyWithPadding(ax::Vector<ax::Node*> children, float padding);
	void createBG(ax::Node* layer, ax::Color3B color);
	void createBG(ax::Node* layer);
	void createCorners(ax::Node* layer, bool topRight, bool topLeft, bool botLeft, bool botRight);
	inline void createAllCorners(ax::Node* self) { return createCorners(self, true, true, true, true); }
	
	int getHighestChildZ(ax::Node* node);
}