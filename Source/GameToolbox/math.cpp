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

#include "math.h"
#include <cmath>
#include "math/MathUtil.h"
#include <algorithm>

float GameToolbox::repeat(float a, float length) {
	return std::clamp(a - floorf(a / length) * length, 0.0f, length);
}
float GameToolbox::SquareDistance(float xa, float ya, float xb, float yb) {
	return ((xb - xa) * (xb - xa)) + ((yb - ya) * (yb - ya));
}
float GameToolbox::SquareDistance(ax::Vec2 a, ax::Vec2 b) {
	return SquareDistance(a.x, a.y, b.x, b.y);
}
float GameToolbox::slerp(float a, float b, float ratio) {
	float delta = repeat((b - a), 360.f);
	if (delta > 180.f)
		delta -= 360.f;
	return a + delta * std::clamp(ratio, 0.f, 1.f);
}
float GameToolbox::iLerp(float a, float b, float ratio, float dt) {
	const float rDelta = dt / (1.f / 60.f);
	const float s	  = 1.f - ratio;

	float iRatio = 1.f - powf(s, rDelta);

	return ax::MathUtil::lerp(a, b, iRatio);
}
float GameToolbox::iSlerp(float a, float b, float ratio, float dt) {
	const float rDelta = dt / (1.f / 60.f);
	const float s	  = 1.f - ratio;

	float iRatio = 1.f - powf(s, rDelta);

	return slerp(a, b, iRatio);
}