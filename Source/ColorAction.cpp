#include "ColorAction.h"
#include "2d/CCActionTween.h"
#include "SpriteColor.h"

ColorAction* ColorAction::create(float duration, SpriteColor* target, ax::Color3B from, ax::Color3B to, float afrom ,
								 float ato)
{
	ColorAction* ret = new ColorAction();
	if (ret->initWithDuration(duration, target, from, to))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

bool ColorAction::initWithDuration(float duration, SpriteColor* target, ax::Color3B from, ax::Color3B to,
								   float afrom, float ato)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_target = target;
		_to = to;
		_from = from;
		_afrom = afrom;
		_ato = ato;
		return true;
	}

	return false;
}

void ColorAction::startWithTarget(ax::Node* target)
{
	ActionInterval::startWithTarget(target);
	_deltar = _to.r - _from.r;
	_deltag = _to.g - _from.g;
	_deltab = _to.b - _from.b;
	_deltaa = _ato - _afrom;
}

void ColorAction::update(float dt)
{
	_target->_color.r = _to.r - _deltar * (1 - dt);
	_target->_color.g = _to.g - _deltag * (1 - dt);
	_target->_color.b = _to.b - _deltab * (1 - dt);
	if(_ato >= 0) _target->_opacity = (_ato * 255.f) - _deltaa * (1 - dt);
}