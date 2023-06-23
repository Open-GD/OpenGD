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
#include <string_view>
#include <external/json.hpp>

namespace ax 
{ 
	class Node; 
}

namespace NodeSerializer
{
	void from_json(const nlohmann::ordered_json&, ax::Node*);
	void to_json(nlohmann::ordered_json&, const ax::Node*);

	void saveNodeToJsonFile(const std::string_view filename, ax::Node*);
	void copyNodeJsonToClipboard(ax::Node*);
};