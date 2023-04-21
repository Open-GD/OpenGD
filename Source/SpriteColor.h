#pragma once

#include <axmol.h>

class SpriteColor
{
  public:
	ax::Color3B _color;
	float _opacity = 255.f;
	bool _blending;

	SpriteColor(ax::Color3B color, float opacity, bool blending) : _color(color), _opacity(opacity), _blending(blending) {}
	SpriteColor() {};
};