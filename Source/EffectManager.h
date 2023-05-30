#pragma once

#include "GroupCommandObject.h"
#include "CCMoveNode.h"
#include "2d/CCNode.h"
#include <deque>

class EffectManager : public ax::Node
{
	public:
		std::vector<GroupCommandObject*> _groupActions;
		std::vector<GroupCommandObject*> _completedMoveActions;
		std::deque<GroupCommandObject*> _tempCompletedActions;
		std::unordered_map<int, CCMoveNode*> _activeMoveActions;

		float _xAccel, _yAccel;

		void runMoveCommand(float duration, ax::Point offsetPos, int easeType, float easeAmt, int groupID);

		void prepareMoveActions(float dt, bool idk);

		static EffectManager* create();
};