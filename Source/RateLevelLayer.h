#pragma once

#include <axmol.h>
#include "PopupLayer.h"
#include "MenuItemSpriteExtra.h"

class RateLevelLayer : public PopupLayer
{
	static void selectRating(ax::Node* btn);

	static unsigned int m_dSelectedDiff;
	static MenuItemSpriteExtra* m_pSubmitButton;
public:
	static RateLevelLayer* create(int levelID);

	bool init(int levelID);
};
