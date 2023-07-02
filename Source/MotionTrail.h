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

#include "2d/MotionStreak.h"

namespace ax 
{ 
	class Texture2D;
	struct Color3B; 
}


class MotionTrail : public ax::MotionStreak
{
public:
	MotionTrail();
	virtual ~MotionTrail();

	static MotionTrail* create(float timeToFade,
		float minSeg,
		float strokeWidth,
		const ax::Color3B& strokeColor,
		std::string_view imagePath);

	static MotionTrail* create(float timeToFade,
		float minSeg,
		float strokeWidth,
		const ax::Color3B& strokeColor,
		ax::Texture2D* texture);

	virtual void update(float delta) override;

	bool _appendNewPoints = true;
	void resumeStroke();
	void stopStroke();
};