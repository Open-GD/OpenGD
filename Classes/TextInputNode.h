#pragma once

#include <axmol.h>
#include <ui/CocosGUI.h>

class TextInputNode : public ax::Layer
{
public:
	std::string m_pAllowedChars;
	ax::ui::TextField* m_pTextField;
	ax::Label* m_pPlaceholder;

	static TextInputNode* create(float width, float height, const char* font, const char* placeholder, int scale)
	{
		TextInputNode* pRet = new TextInputNode();
		if (pRet->init(width, height, placeholder, font, scale))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}

	bool onTouchBegan(ax::Touch* touch, ax::Event* event);
	bool init(float width, float height, const char* placeholder, const char* font, int scale);
};
