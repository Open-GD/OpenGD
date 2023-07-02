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

#include "2d/Layer.h"
#include "2d/ActionTween.h"
#include "2d/Action.h"

namespace ax 
{ 
	class Sprite; 
}


class GroundLayer : public ax::Layer, public ax::ActionTweenDelegate 
{
private:
	virtual void updateTweenAction(float value, std::string_view key) override;
	
	float m_fOneGroundSize;

	AX_SYNTHESIZE(float, m_fSpeed, Speed)

public:
	ax::Sprite* _sprite;
	void update(float dt) override;
	bool init(int groundID);
	static GroundLayer* create(int groundID);
};