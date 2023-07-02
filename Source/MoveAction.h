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

#include "2d/ActionInterval.h"

struct GroupProperties;
namespace ax 
{ 
	class Node; 
	class Vec2;
}

class MoveAction : public ax::MoveBy
{
private:
	GroupProperties* _group;
	std::vector<ax::Vec2> _prevPositions, _startPositions;
	ax::Vec2 _posDelta;

public:
	virtual void update(float time) override;
	virtual void stop() override;
	virtual void startWithTarget(ax::Node* target) override;
	bool initWithDuration(float duration, const ax::Vec2& deltaPosition, GroupProperties* group);
	static MoveAction* create(float duration, const ax::Vec2& deltaPosition, GroupProperties* group);
};