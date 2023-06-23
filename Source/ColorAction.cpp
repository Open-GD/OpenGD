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
	_deltar = static_cast<float>(_to.r - _from.r);
	_deltag = static_cast<float>(_to.g - _from.g);
	_deltab = static_cast<float>(_to.b - _from.b);
	_deltaa = _ato - _afrom;
}

void ColorAction::update(float dt)
{
	_target->_color.r = static_cast<uint8_t>(_to.r - _deltar * (1 - dt));
	_target->_color.g = static_cast<uint8_t>(_to.g - _deltag * (1 - dt));
	_target->_color.b = static_cast<uint8_t>(_to.b - _deltab * (1 - dt));
	if(_ato >= 0) _target->_opacity = (_ato * 255.f) - _deltaa * (1 - dt);
}