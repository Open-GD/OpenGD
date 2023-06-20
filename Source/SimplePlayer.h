#pragma once

#include "2d/CCSprite.h"
#include "ccTypes.h"

#include "GameToolbox/enums.h"

class SimplePlayer : public ax::Sprite {
public:
	bool init(int cubeID);

	ax::Sprite* m_pMainSprite;
	ax::Sprite* m_pSecondarySprite;
	ax::Sprite* m_pGlowSprite;
	ax::Sprite* m_pExtraSprite;
	ax::Sprite* m_pDomeSprite;

	bool m_bHasGlow = false;

	ax::Color3B m_MainColor = { 255, 255, 255 };
	ax::Color3B m_SecondaryColor = {255, 255, 255};
	ax::Color3B m_GlowColor = {255, 255, 255};

public:
	static SimplePlayer* create(int cubeID);

	void updateGamemode(int iconID, IconType mode);
	void setMainColor(ax::Color3B col);
		void setSecondaryColor(ax::Color3B col);
		void setGlowColor(ax::Color3B col);
		void updateIconColors();
		void setGlow(bool glow);
};
