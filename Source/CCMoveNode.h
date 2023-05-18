#pragma once

#include "2d/CCNode.h"

class CCMoveNode : public ax::Node
{
  public:
	ax::Point _newPosStatic, _newPosOptimized;

    static CCMoveNode* create();
};