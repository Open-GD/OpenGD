#pragma once

#include "PopupLayer.h"

class MenuItemSpriteExtra;
class RateLevelLayer;

namespace ax 
{ 
	class Node; 
}


class RateLevelLayer : public PopupLayer
{
	static void selectRating(ax::Node* btn);

	static unsigned int m_dSelectedDiff;
	static MenuItemSpriteExtra* m_pSubmitButton;
public:
	static RateLevelLayer* create(int levelID);

	bool init(int levelID);
};
