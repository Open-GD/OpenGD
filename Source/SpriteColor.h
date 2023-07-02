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
#include "GDHSV.h"

class SpriteColor
{
  public:
	ax::Color3B _color;
	float _opacity = 255.f;
	bool _blending;
	int _copyingColorID = -1;
	bool _applyHsv;
	GDHSV _hsvModifier;

	SpriteColor(ax::Color3B color, float opacity, bool blending) : _color(color), _opacity(opacity), _blending(blending) {}
	SpriteColor() = default;
};