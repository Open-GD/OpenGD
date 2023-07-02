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

#include <string>
#include <string_view>

#include "2d/Layer.h"
#include "Types.h"

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
