#pragma once

#include <axmol.h>
#include "PopupLayer.h"
#include "TextButton.h"

class RateLevelLayer : public PopupLayer
{
	static void selectRating(ax::Node* btn);

	static unsigned int m_dSelectedDiff;
	static TextButton* m_pSubmitButton;
public:
	static RateLevelLayer* create(int levelID);

	bool init(int levelID);
};
