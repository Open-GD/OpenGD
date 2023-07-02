#include "GroupColorAction.h"
#include "2d/ActionTween.h"

GroupColorAction* GroupColorAction::create(float duration, GroupProperties* target, ax::Color3B from, ax::Color3B to, bool resetGroupState)
{
	GroupColorAction* ret = new GroupColorAction();
	if (ret->initWithDuration(duration, target, from, to, resetGroupState))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

bool GroupColorAction::initWithDuration(float duration, GroupProperties* target, ax::Color3B from, ax::Color3B to, bool resetGroupState)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_resetGroupState = resetGroupState;
		_target = target;
		_to = to;
		_from = from;
		return true;
	}

	return false;
}

void GroupColorAction::startWithTarget(ax::Node* target)
{
	ActionInterval::startWithTarget(target);
	_deltar = static_cast<float>(_to.r - _from.r);
	_deltag = static_cast<float>(_to.g - _from.g);
	_deltab = static_cast<float>(_to.b - _from.b);
}

void GroupColorAction::update(float dt)
{
	_target->_color.r = static_cast<uint8_t>(_to.r - _deltar * (1 - dt));
	_target->_color.g = static_cast<uint8_t>(_to.g - _deltag * (1 - dt));
	_target->_color.b = static_cast<uint8_t>(_to.b - _deltab * (1 - dt));
	if(_elapsed >= _duration && _resetGroupState) _target->groupState = GroupProperties::GroupState::NOT_CHANGING;
}