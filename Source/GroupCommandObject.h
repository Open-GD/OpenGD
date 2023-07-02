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

#include "2d/ActionEase.h"
#include "2d/ActionTween.h"

class GroupCommandObject : public ax::Node, public ax::ActionTweenDelegate
{
  public:
	ax::Point _oldPos;
	ax::Point _currentOffset;
	ax::Point _newPos;
  ax::Point _unkPoint;
  ax::Point _finalPoint;

  int _groupID;
  int _actionID;

  float _elapsed, _duration, _delta1, _delta2;

  bool _followPlayerX, _followPlayerY;
  bool _actionDone, _actionDoneForNextLoop;

  ax::Action *_action1, *_action2;

	void runMoveCommand(float duration, ax::Point offsetPos, int easeType, float easeAmt);
	void step(float dt);
	virtual void updateTweenAction(float value, std::string_view key) override;

	static GroupCommandObject *create();
};