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

/*
!! WILL BE REWRITTEN !!
#pragma once
#include <functional>
#include <string_view>

#include "MenuItemSpriteExtra.h"

namespace ax 
{
	class Node; 
}


using CheckboxCallback = const std::function<void(ax::Node*, bool)>&;

class Checkbox : public MenuItemSpriteExtra {
public:
	Checkbox(std::string_view off, std::string_view on, bool toggled, CheckboxCallback callback);
	 bool init(std::string_view text);
	 void activate() override;
	
	std::string_view _offSpr;
	std::string_view _onSpr;
	bool _toggled;
	std::function<void(ax::Node*, bool)> _callback;
	
	static Checkbox* create(std::string_view text, std::string_view off, std::string_view on, bool toggled, CheckboxCallback callback);
	static Checkbox* create(std::string_view text, bool toggled, CheckboxCallback);
	static Checkbox* create(std::string_view text, CheckboxCallback);
};

*/