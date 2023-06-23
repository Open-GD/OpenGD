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

#include "PopupLayer.h"

class MenuItemSpriteExtra;
class RateLevelLayer;

namespace ax 
{ 
	class Node; 
}


class RateLevelLayer : public PopupLayer
{
	static void selectRating(ax::Node* btn);

	static unsigned int m_dSelectedDiff;
	static MenuItemSpriteExtra* m_pSubmitButton;
public:
	static RateLevelLayer* create(int levelID);

	bool init(int levelID);
};
