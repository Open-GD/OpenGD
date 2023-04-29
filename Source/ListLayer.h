#pragma once
#include "2d/CCLayer.h"
#include "ccTypes.h"
#include "math/Vec2.h"

namespace ax 
{ 
	class Node; 
}


class ListLayer : public ax::LayerColor {
	private:		
		bool init(ax::Node* scrollLayer, const char* label, ax::Color4B color, ax::Vec2 size);
	public:
		static ListLayer* create(ax::Node* scrollLayer, const char* label, ax::Color4B color, ax::Vec2 size);
		static ListLayer* create(ax::Node* scrollLayer, const char* label, ax::Color4B color);
		static ListLayer* create(ax::Node* scrollLayer, const char* label);
};