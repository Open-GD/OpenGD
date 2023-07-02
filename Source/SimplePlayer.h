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

#include "2d/Sprite.h"
#include "Types.h"

#include "GameToolbox/enums.h"

class SimplePlayer : public ax::Sprite {
public:
	bool init(int cubeID);

	ax::Sprite* m_pMainSprite;
	ax::Sprite* m_pSecondarySprite;
	ax::Sprite* m_pGlowSprite;
	ax::Sprite* m_pExtraSprite;
	ax::Sprite* m_pDomeSprite;

	bool m_bHasGlow = false;

	ax::Color3B m_MainColor = { 255, 255, 255 };
	ax::Color3B m_SecondaryColor = {255, 255, 255};
	ax::Color3B m_GlowColor = {255, 255, 255};

public:
	static SimplePlayer* create(int cubeID);

	void updateGamemode(int iconID, IconType mode);
	void setMainColor(ax::Color3B col);
		void setSecondaryColor(ax::Color3B col);
		void setGlowColor(ax::Color3B col);
		void updateIconColors();
		void setGlow(bool glow);
};
