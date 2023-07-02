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

#include "2d/ActionInterval.h"
#include "Types.h"
#include "GDHSV.h"

class SpriteColor;

class ColorAction : public ax::ActionInterval
{
  private:
	ax::Color3B _from, _to;
	float _afrom, _ato;
	SpriteColor* _target;
	GDHSV* _hsv;

	int _copyColorId;

	float _deltar, _deltag, _deltab, _deltaa;

  public:
	static ColorAction* create(float duration, SpriteColor* target, ax::Color3B from, ax::Color3B to, float afrom = -1, float ato = -1, int copyColor = -1, GDHSV* hsv = nullptr);

	void startWithTarget(ax::Node* target) override;
	void update(float dt) override;

	bool initWithDuration(float duration, SpriteColor* target, ax::Color3B from, ax::Color3B to, float afrom = -1, float ato = -1, int copyColor = -1, GDHSV* hsv = nullptr);
};