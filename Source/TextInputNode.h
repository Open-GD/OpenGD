#pragma once

#include <string>
#include <string_view>

#include "2d/CCLayer.h"
#include "ccTypes.h"

enum class KeyCode;

class TextInputDelegate;

namespace ax
{ 
	class Label;
	class Event; 
	namespace ui
	{
		class TextField;
	}
}

class TextInputNode : public ax::Layer
{
public:
	TextInputNode();

	static TextInputNode* create(float width, float height, std::string_view font, std::string_view placeholder, int scale);

	bool init(float width, float height, std::string_view placeholder, std::string_view font, int scale);

	void onTextFieldChanged();
	void onTextFieldAttachWithIME();
	void onTextFieldDetachWithIME();

	void updateDisplayedLabel();
	void updateDisplayedLabelScale();
	void updateCursor();

	std::string sanitizeString(std::string_view);

	ax::Label* getDisplayedLabel();
	ax::ui::TextField* getTextField();
	float getMaxDisplayLabelScale();
	float getPlaceholderScale();
	std::string_view getAllowedChars();
	std::string_view getString();
	TextInputDelegate* getDelegate();

	void setMaxDisplayLabelScale(float scale);
	void setPlaceholderScale(float scale);
	void setPlaceholderColor(ax::Color3B color);
	void setDisplayedLabelColor(ax::Color3B color);
	void setAllowedChars(std::string_view allowedChars);
	void setString(std::string_view str);
	void setDelegate(TextInputDelegate*);

	bool onTouchBegan(ax::Touch* touch, ax::Event* event);

	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
private:
	float _maxDisplayLabelScale;
	float _placeholderScale;

	std::string_view _allowedChars;
	std::string_view _placeholderString;

	std::string _previousString;

	ax::Color3B _placeholderColor;
	ax::Color3B _textColor;

	bool _onCommandMode;

	ax::ui::TextField* _textField;

	ax::Label* _displayedLabel;
	ax::Label* _cursor;

	TextInputDelegate* _delegate;
};

class TextInputDelegate
{
public:
	virtual void textChanged(TextInputNode*) {}
	virtual void textInputOpened(TextInputNode*) {}
	virtual void textInputClosed(TextInputNode*) {}
};
