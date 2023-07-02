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
	void showLayer(bool attachToScene = true, bool bounce = false);
	void hideLayer();
	virtual void hideBackButton();
	void keyBackClicked();
};