#pragma once

#include <axmol.h>
#include <ui/CocosGUI.h>

class TextInputNode : public ax::Layer
{
public:
	std::string _pAllowedChars;
	ax::ui::TextField* _pTextField;
	ax::Label* _pPlaceholder;

	bool _pCommandMode;

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

	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);

	bool init(float width, float height, const char* placeholder, const char* font, int scale);
};
