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

enum IconType {
	kIconTypeCube		= 0,
	kIconTypeShip		= 1,
	kIconTypeBall		= 2,
	kIconTypeUfo		 = 3,
	kIconTypeWave		= 4,
	kIconTypeRobot	   = 5,
	kIconTypeSpider	  = 6,
	kIconTypeSwing	   = 7,
	kIconTypeDeathEffect = 98,
	kIconTypeSpecial	 = 99,
};

enum popTransition {
	kTransitionFade = 0,
	kTransitionShop
};

namespace GameToolbox
{
	enum GTRes
	{
		Low = 0,
		Medium,
		High
	};
}