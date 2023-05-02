#include "MoveAction.h"
#include "2d/CCActionInterval.h"

MoveAction* MoveAction::create(float duration, const ax::Vec2& deltaPosition, GroupProperties* group)
{
	MoveAction* ret = new MoveAction();

	if (ret->initWithDuration(duration, deltaPosition, group))
	{
		ret->autorelease();

		return ret;
	}

	delete ret;
	return nullptr;
}

bool MoveAction::initWithDuration(float duration, const ax::Vec2& deltaPosition, GroupProperties* group)
{
	_group = group;
	return MoveBy::initWithDuration(duration, deltaPosition);
}

void MoveAction::startWithTarget(ax::Node* target)
{
	ActionInterval::startWithTarget(target);
	for (auto obj : _group->_objects)
		obj->_prevPos = obj->_startPosition = obj->getPosition();
	GameToolbox::log("action start");
}

void MoveAction::update(float time)
{
#if AX_ENABLE_STACKABLE_ACTIONS
	for (auto obj : _group->_objects)
	{
		ax::Vec2 currentPos = obj->getPosition();
		ax::Vec2 diff = currentPos - obj->_prevPos;
		obj->_startPosition = obj->_startPosition + diff;
		ax::Vec2 newPos = obj->_startPosition + (ax::Vec2(_positionDelta.x, _positionDelta.y) * time);
		obj->setPosition(newPos);
		obj->_prevPos = newPos;
	}
#else
	for (auto obj : _group->_objects)
		_obj->setPosition3D(_startPosition + _positionDelta * t);
#endif // AX_ENABLE_STACKABLE_ACTIONS
}

void MoveAction::stop()
{
	return;
}
