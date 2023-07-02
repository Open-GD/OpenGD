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
#include <string_view>

#include "2d/DrawNode.h"
#include "2d/ActionTween.h"
#include "PlatformMacros.h"

namespace ax 
{
	class Renderer;
	class Mat4;
	class Node;
}

class CircleWave : public ax::DrawNode, public ax::ActionTweenDelegate {
private:
	void draw(ax::Renderer *renderer, const ax::Mat4& transform, uint32_t flags) override;
	virtual void updateTweenAction(float value, std::string_view key) override;
	bool init(float duration, ax::Color4B color, float radiusMin, float radiusMax, bool easing, float lineWidth, bool filled);
	void update(float dt) override;

	ax::Color4F _color; // damn CC_SYNTHESIZE doesnt work with it :skull:
	ax::Node* _followedNode;
	AX_SYNTHESIZE(float, _lineWidth, LineWidth)
	AX_SYNTHESIZE(float, _radius, Radius)
	AX_SYNTHESIZE(float, _width, Width)
	AX_SYNTHESIZE(bool, _filled, Filled)
	
public:
	void followNode(ax::Node* node);
	static CircleWave* create(float duration, ax::Color4B color, float radiusMin, float radiusMax, bool easing, bool filled, float lineWidth = 2.0f);

	void setColor(ax::Color4B col);
	ax::Color4B getColor();
};