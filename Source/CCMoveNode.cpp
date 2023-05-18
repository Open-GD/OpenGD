#include "CCMoveNode.h"

CCMoveNode* CCMoveNode::create()
{
	CCMoveNode* ret = new CCMoveNode();
	if (ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		delete ret;
		ret = nullptr;
		return nullptr;
	}
}