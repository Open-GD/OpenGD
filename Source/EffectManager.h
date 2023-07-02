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

#include "GroupCommandObject.h"
#include "MoveNode.h"
#include "2d/Node.h"
#include <deque>
#include "GameToolbox/conv.h"

class EffectManager : public ax::Node
{
	public:
		std::vector<GroupCommandObject*> _groupActions;
		std::vector<GroupCommandObject*> _completedMoveActions;
		std::deque<GroupCommandObject*> _tempCompletedActions;
		std::unordered_map<int, CCMoveNode*, my_string_hash> _activeMoveActions;

		float _xAccel, _yAccel;

		void runMoveCommand(float duration, ax::Point offsetPos, int easeType, float easeAmt, int groupID);

		void prepareMoveActions(float dt, bool idk);

		static EffectManager* create();
};