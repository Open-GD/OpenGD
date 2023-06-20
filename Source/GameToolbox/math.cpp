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