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
#include "ccTypes.h"
#include "enums.h"
#include <string_view>
#include <string>
#include <vector>

namespace GameToolbox
{
	ax::Color3B colorForIdx(int col);
	const char* getNameGamemode(IconType mode);
	int getValueForGamemode(IconType mode);
	ax::Color3B hsvToRgb(const ax::HSV& hsv);
	float stof(const std::string_view str);
	int stoi(const std::string_view str);
	const char* levelLengthString(int len);
	std::vector<std::string> splitByDelim(const std::string& s, char delim);
	std::vector<std::string_view> splitByDelimStringView(std::string_view str, char delim);
	ax::BlendFunc getBlending();
	std::string xorCipher(const std::string& message, const std::string& key);

}