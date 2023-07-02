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

#include "TextInputNode.h"

#include <fmt/format.h>
#include <EventListenerTouch.h>
#include <EventListenerKeyboard.h>
#include <Director.h>
#include <EventDispatcher.h>
#include <ui/UITextField.h>

#ifdef AX_PLATFORM_PC
#include <platform/desktop/GLViewImpl-desktop.h>
#endif

USING_NS_AX;

TextInputNode::TextInputNode()
	: _allowedChars(" abcdefghijkmlnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890")
	, _placeholderString("")
	, _previousString("")
	, _placeholderColor({ 120, 170, 240 })
	, _textColor(Color3B::WHITE)
	, _maxDisplayLabelScale(1.f)
	, _placeholderScale(0.0f)
	, _delegate(nullptr)
	, _textField(nullptr)
	, _displayedLabel(nullptr)
	, _cursor(nullptr)
	, _onCommandMode(false)
{
}

TextInputNode* TextInputNode::create(float width, float height, std::string_view font, std::string_view placeholder, int scale)
{
	TextInputNode* pRet = new TextInputNode();

	if (pRet && pRet->init(width, height, placeholder, font, scale))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool TextInputNode::init(float width, float height, std::string_view placeholder, std::string_view font, int scale)
{
	if (!Layer::init())
		return false;

	_placeholderString = placeholder;

	this->setContentSize({ width, height });

	_textField = ui::TextField::create(placeholder, "Thonburi", scale);
	_textField->setVisible(false);
	this->addChild(_textField);

	if (!font.empty())
	{
		_cursor = Label::createWithBMFont(font, "|");
		_cursor->setBMFontSize(scale);
		_cursor->setOpacity(150);
		_cursor->setPositionY(height / 2);
		_cursor->setAnchorPoint({ -0.5, 0.5 });
		_cursor->setVisible(false);
		this->addChild(_cursor);

		_displayedLabel = Label::createWithBMFont(font, placeholder, TextHAlignment::CENTER, 0);
		_displayedLabel->setBMFontSize(scale);
		_displayedLabel->setPositionY(height / 2);
		_displayedLabel->setPositionX(width / 2);
		_displayedLabel->setAnchorPoint({ 0.5, 0.5 });
		this->addChild(_displayedLabel);

		updateDisplayedLabel();
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	_textField->addEventListener([&](Ref* re, ui::TextField::EventType event)
		{
			switch (event)
			{
			case ui::TextField::EventType::DETACH_WITH_IME:
				onTextFieldDetachWithIME();
				break;
			case ui::TextField::EventType::ATTACH_WITH_IME:
				onTextFieldAttachWithIME();
				break;
			default: // INSERT_TEXT / DELETE_BACKWARD
				onTextFieldChanged();
			}
		});

	touchListener->onTouchBegan = AX_CALLBACK_2(TextInputNode::onTouchBegan, this);

#ifdef AX_PLATFORM_PC
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = AX_CALLBACK_2(TextInputNode::onKeyPressed, this);
	listener->onKeyReleased = AX_CALLBACK_2(TextInputNode::onKeyReleased, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

bool TextInputNode::onTouchBegan(Touch* touch, Event* event)
{
	Director* director = Director::getInstance();

	auto nodeTouchPoint = this->convertTouchToNodeSpace(touch);

	const auto& contentSize = this->getContentSize();
	auto boundingBox = Rect(0, 0, contentSize.width, contentSize.height);

	bool touchInside = boundingBox.containsPoint(nodeTouchPoint);

	if (touchInside)
	{
		_textField->attachWithIME();
		return true;
	}
	else
	{
		_textField->detachWithIME();
		return false;
	}
};

void TextInputNode::onTextFieldAttachWithIME()
{
	if (_displayedLabel && _cursor)
	{
		if (_textField->getString().empty())
		{
			_displayedLabel->setString("");
			updateDisplayedLabelScale();
		}
		else
		{
			_textField->setString(sanitizeString(_textField->getString()));
			updateDisplayedLabel();
		}

		_cursor->setVisible(true);
	}

	if (_delegate)
		_delegate->textInputOpened(this);
}

void TextInputNode::onTextFieldDetachWithIME()
{
	if (_displayedLabel && _cursor)
	{
		_textField->setString(sanitizeString(_textField->getString()));

		_cursor->setVisible(false);

		updateDisplayedLabel();
	}

	if (_delegate)
		_delegate->textInputClosed(this);
}

void TextInputNode::onTextFieldChanged()
{
	std::string str = sanitizeString(_textField->getString());
	bool didTextChange = (str != _previousString);

	if (didTextChange)
	{
		_previousString = str;
		_textField->setString(str);

		updateDisplayedLabel();

		if (_delegate)
			_delegate->textChanged(this);
	}
}

void TextInputNode::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	// if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) GameToolbox::_isCtrlPressed = true;
	// i don't think that ctrl flag should be placed in gametoolbox

	switch (keyCode)
	{
#ifdef AX_PLATFORM_PC
	case EventKeyboard::KeyCode::KEY_CTRL:
		_onCommandMode = true;
		break;
	case EventKeyboard::KeyCode::KEY_C:
		if (_onCommandMode)
			glfwSetClipboardString(static_cast<GLViewImpl*>(Director::getInstance()->getOpenGLView())->getWindow(), _textField->getString().data());
		_onCommandMode = false;
		break;
	case EventKeyboard::KeyCode::KEY_V:
		if (_onCommandMode)
		{
			_textField->setString(sanitizeString(fmt::format("{}{}", _textField->getString(), glfwGetClipboardString(static_cast<GLViewImpl*>(Director::getInstance()->getOpenGLView())->getWindow()))));
			updateDisplayedLabel();
		}
		_onCommandMode = false;
		break;
#endif
	default:
		break;
	}
}

void TextInputNode::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_CTRL)
		_onCommandMode = false;
}

// Updates the displayed label
void TextInputNode::updateDisplayedLabel()
{
	if (!_displayedLabel || !_cursor)
		return;

	if (_textField->getString().empty())
	{
		_displayedLabel->setString(_placeholderString);
		_displayedLabel->setColor(_placeholderColor);
	}
	else
	{
		_displayedLabel->setString(_textField->getString());
		_displayedLabel->setColor(_textColor);
	}

	updateDisplayedLabelScale();
}

// Updates the displayed label's scale
void TextInputNode::updateDisplayedLabelScale()
{
	if (!_displayedLabel || !_cursor)
		return;

	if (_placeholderScale != 0.0f && _textField->getString().empty())
	{
		_displayedLabel->setScale(_placeholderScale);
	}
	else
	{
		float newScale = this->getContentSize().width / _displayedLabel->getContentSize().width;

		if (newScale < _maxDisplayLabelScale)
			_displayedLabel->setScale(newScale);
		else
			_displayedLabel->setScale(_maxDisplayLabelScale);
	}

	updateCursor();
}

// Updates the x-position and scale of the cursor
void TextInputNode::updateCursor()
{
	if (!_displayedLabel || !_cursor)
		return;

	float scale = (_placeholderScale != 0.0f && _textField->getString().empty()) ? _placeholderScale : _displayedLabel->getScale();

	_cursor->setScale(scale);

	if (_displayedLabel->getAnchorPoint().x)
		_cursor->setPositionX((_displayedLabel->getPositionX() + ((_displayedLabel->getContentSize().width * scale) * _displayedLabel->getAnchorPoint().x)));
	else
		_cursor->setPositionX((_displayedLabel->getPositionX() + (_displayedLabel->getContentSize().width * scale)));
}

std::string TextInputNode::sanitizeString(std::string_view str)
{
	std::string filteredStr("");

	for (char character : str)
	{
		bool isAllowed = false;

		for (char allowed_char : _allowedChars)
			if (character == allowed_char)
				isAllowed = true;

		if (isAllowed)
			filteredStr += character;
	}

	return filteredStr;
}

Label* TextInputNode::getDisplayedLabel()
{
	return _displayedLabel;
}

ax::ui::TextField* TextInputNode::getTextField()
{
	return _textField;
}

float TextInputNode::getMaxDisplayLabelScale()
{
	return _maxDisplayLabelScale;
}

float TextInputNode::getPlaceholderScale()
{
	return _placeholderScale;
}

std::string_view TextInputNode::getAllowedChars()
{
	return _allowedChars;
}

std::string_view TextInputNode::getString()
{
	return _textField->getString();
}

TextInputDelegate* TextInputNode::getDelegate()
{
	return _delegate;
}

void TextInputNode::setMaxDisplayLabelScale(float scale)
{
	_maxDisplayLabelScale = scale;
}

void TextInputNode::setPlaceholderScale(float scale)
{
	_placeholderScale = scale;
	updateDisplayedLabel();
}

void TextInputNode::setPlaceholderColor(ax::Color3B color)
{
	_placeholderColor = color;
	updateDisplayedLabel();
}

void TextInputNode::setDisplayedLabelColor(ax::Color3B color)
{
	_textColor = color;
	updateDisplayedLabel();
}

void TextInputNode::setAllowedChars(std::string_view allowedChars)
{
	_allowedChars = allowedChars;
	_textField->setString(sanitizeString(_textField->getString()));
	updateDisplayedLabel();
}

void TextInputNode::setString(std::string_view str)
{
	_textField->setString(str);
	updateDisplayedLabel();
}

void TextInputNode::setDelegate(TextInputDelegate* delegate)
{
	_delegate = delegate;
}
