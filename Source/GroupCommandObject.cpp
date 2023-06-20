#include "GroupCommandObject.h"
#include "EffectGameObject.h"

GroupCommandObject* GroupCommandObject::create()
{
	GroupCommandObject* ret = new GroupCommandObject();
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

void GroupCommandObject::runMoveCommand(float duration, ax::Point offsetPos, int easeType, float easeAmt)
{
	this->_finalPoint = offsetPos;
	if (offsetPos.x != 0)
	{
		this->_action1 = EffectGameObject::actionEasing(ax::ActionTween::create(duration, "1", 0.0f, offsetPos.x),
														easeType, easeAmt);
		_action1->retain();
		this->runAction(_action1);
	}
	if (offsetPos.y != 0)
	{
		auto ac = EffectGameObject::actionEasing(ax::ActionTween::create(duration, "2", 0.0f, offsetPos.y), easeType,
												 easeAmt);
		ac->retain();
		this->runAction(ac);
		if (this->_action1)
			this->_action2 = ac;
		else
			this->_action1 = ac;
	}
}

void GroupCommandObject::updateTweenAction(float value, std::string_view key)
{
	if (key == "2") // y
	{
		float offsetY = value;
		float v18 = (value - this->_oldPos.y);
		this->_oldPos.y = value;
		float delta = value - this->_currentOffset.y;
		this->_unkPoint.y = v18 + this->_unkPoint.y;
		if (delta < 0.5f && delta > -0.5f)
		{
			if (value != this->_finalPoint.y)
			{
				delta = 0.0f;
				offsetY = this->_currentOffset.y;
			}
		}
		this->_currentOffset.y = offsetY;
		this->_newPos.y = delta;
	}
	else if (key == "1") // x
	{
		float offsetX = value;
		float v18 = (value - this->_oldPos.x);
		this->_oldPos.x = value;
		float delta = value - this->_currentOffset.x;
		this->_unkPoint.x = v18 + this->_unkPoint.x;
		if (delta < 0.5f && delta > -0.5f)
		{
			if (value != this->_finalPoint.x)
			{
				delta = 0.0f;
				offsetX = this->_currentOffset.x;
			}
		}
		this->_currentOffset.x = offsetX;
		this->_newPos.x = delta;
	}
}

void GroupCommandObject::step(float dt)
{
	_elapsed += dt;
	if (!_actionDone)
	{
		if (_action1)
		{
			_action1->step(dt);
			if (_action2)
				_action2->step(dt);
			if (_action1->isDone())
			{
				_action1->stop();
				_action1->release();
				if (_action2)
				{
					_action2->stop();
					_action2->release();
				}
				this->_actionDone = true;
			}
		}
		else if(this->_elapsed >= this->_duration)
		{
			this->_actionDone = true;
		}
	}
}