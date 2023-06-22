#pragma once
#include "ccTypes.h"
#include "enums.h"
#include "CCVector.h"

namespace ax
{
	class Label; 
	class Menu;
	class Node;
}


namespace GameToolbox
{
	void popSceneWithTransition(float time, popTransition type = popTransition::kTransitionFade);
	void limitLabelWidth(ax::Label* label, float width, float normalScale, float minScale = 0);
	ax::Label* createBMFont(std::string text, std::string font);
	ax::Label* createBMFont(std::string text, std::string font, int width);
	ax::Label* createBMFont(std::string text, std::string font, int width, ax::TextHAlignment alignment);

	//std::string getTextureString(std::string texture);
	void alignItemsInColumnsWithPadding(ax::Menu* menu, const int rows, const int x_padding, const int y_padding);
	void alignItemsVerticallyWithPadding(ax::Vector<ax::Node*> children, float padding);
	void alignItemsHorizontally(ax::Vector<ax::Node*> children, float padding, ax::Point location);
	void alignItemsHorizontallyWithPadding(ax::Vector<ax::Node*> children, float padding);
	void createBG(ax::Node* layer, ax::Color3B color);
	void createBG(ax::Node* layer);
	void createCorners(ax::Node* layer, bool topRight, bool topLeft, bool botLeft, bool botRight);
	inline void createAllCorners(ax::Node* self) { return createCorners(self, true, true, true, true); }
	
	int getHighestChildZ(ax::Node* node);
}