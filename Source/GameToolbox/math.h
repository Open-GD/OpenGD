#pragma once

#include "math/Vec2.h"
namespace GameToolbox
{	
	float SquareDistance(float xa, float ya, float xb, float yb);
	float SquareDistance(ax::Vec2 a, ax::Vec2 b);

	float iLerp(float a, float b, float ratio, float dt);
	float slerp(float a, float b, float ratio);
	float iSlerp(float a, float b, float ratio, float dt);

	float repeat(float a, float length);
	
	template <typename T>
	inline T inRange(T value, T min, T max) {
		return MIN(MAX(value, min), max);
	}
	
}