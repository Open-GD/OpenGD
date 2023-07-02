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

#include "ButtonSprite.h"
#include "MenuItemSpriteExtra.h"
#include "2d/Label.h"
#include "ui/UIScale9Sprite.h"
#include "Director.h"
#include "renderer/TextureCache.h"
#include "GameToolbox/getTextureString.h"

USING_NS_AX;
//cmake --build build --config RelWithDebInfo --target OpenGD
ButtonSprite* ButtonSprite::create(std::string_view caption, int width, int minWidth, float scale, bool absoluteWidth, std::string_view font, std::string_view texture, float height)
{
	ButtonSprite* ret = new ButtonSprite();

	if (ret && ret->initWithText(caption, width, minWidth, scale, absoluteWidth, font, texture, height))
	{
		ret->autorelease();
		return ret;
	}

	AX_SAFE_DELETE(ret);
	return nullptr;
}

ButtonSprite* ButtonSprite::create(Sprite* iconSprite, int width, int minWidth, float height, float scale, bool absoluteWidth, std::string_view texture, bool dontUseScale9)
{
	ButtonSprite* ret = new ButtonSprite();

	if (ret && ret->initWithSprite(iconSprite, width, minWidth, height, scale, absoluteWidth, texture, dontUseScale9))
	{
		ret->autorelease();
		return ret;
	}

	AX_SAFE_DELETE(ret);
	return nullptr;
}

ButtonSprite* ButtonSprite::create(std::string_view caption, int width, float minWidth, float scale)
{
	return ButtonSprite::create(caption, width, static_cast<int>(minWidth), scale, false, GameToolbox::getTextureString("goldFont.fnt"), GameToolbox::getTextureString("GJ_button_01.png"), false);
}

ButtonSprite* ButtonSprite::create(std::string_view caption)
{
	return ButtonSprite::create(caption, 0, 0, 1.0);
}

bool ButtonSprite::initWithText(std::string_view caption, int width, int minWidth, float scale, bool absoluteWidth, std::string_view font, std::string_view texture, float height)
{
	if (!Sprite::init())
		return false;

	_hasTextProvided = true;
	_scale = scale;
	_width = static_cast<float>(width);
	_absoluteWidth = absoluteWidth;
	_height = height;
	_minWidth = static_cast<float>(minWidth);

	_spriteOffset = { 0, 2 };

	if (font == GameToolbox::getTextureString("bigFont.fnt"))
		_spriteOffset = { -1, 2 };

	_label = ax::Label::createWithBMFont(font, "");
	this->addChild(_label, 1);

	_buttonTexture = ui::Scale9Sprite::create(texture, ax::Rect(0, 0, 40, 40));
	_buttonTexture->setContentSize({ 16, 16 });
	this->addChild(_buttonTexture, 0);

	this->setString(caption);

	return true;
}

bool ButtonSprite::initWithSprite(Sprite* iconSprite, int width, int minWidth, float height, float scale, bool absoluteWidth, std::string_view texture, bool dontUseScale9)
{
	if (!Sprite::init())
		return false;

	_subSprite = iconSprite;
	_hasTextProvided = false;
	_scale = scale;
	_width = static_cast<float>(width);
	_absoluteWidth = absoluteWidth;
	_minWidth = static_cast<float>(minWidth);

	this->addChild(_subSprite, 1);

	if (dontUseScale9)
	{
		_subBGSprite = Sprite::create(texture);
		this->addChild(_subBGSprite, 0);
	}
	else
	{
		_buttonTexture = ui::Scale9Sprite::create(texture, ax::Rect(0, 0, 40, 40));
		_buttonTexture->setContentSize({ 16,16 });
		this->addChild(_buttonTexture, 0);
	}

	if (_width > 0)
	{
		if (_subSprite->getContentSize().width > _width)
		{
			_subSprite->setScale(_width / _subSprite->getContentSize().width);
		}
	}

	if (height > 0 && _subSprite->getContentSize().height > height)
	{
		float newSpriteScale = (height / _subSprite->getContentSize().height);
		if (newSpriteScale < _subSprite->getScale())
		{
			_subSprite->setScale(newSpriteScale);
		}
	}

	if (_subSprite->getScale() >= _scale)
		_subSprite->setScale(_scale);

	this->updateSpriteBGSize();

	return true;
}

void ButtonSprite::updateBGImage(std::string_view file)
{
	if (_buttonTexture)
	{
		_buttonTexture->removeFromParent();
		
		_buttonTexture = ax::ui::Scale9Sprite::create(file, ax::Rect(0, 0, 40, 40));
		_buttonTexture->setContentSize({ 16, 16 });

		this->addChild(_buttonTexture, 0);

		if (_hasTextProvided)
			this->setString(_label->getString());
		else
			this->updateSpriteBGSize();
	}
	else
	{
		if (_subBGSprite)
		{
			_subBGSprite->setTexture(Director::getInstance()->getTextureCache()->addImage(file));
		}
	}
}

void ButtonSprite::setString(std::string_view str)
{
	if (!_hasTextProvided)
		return;

	// Shrink label if too long
	_label->setScale(1);
	_label->setString(str);

	if (_width > 0)
	{
		float labelWidth = _label->getContentSize().width;
		if (labelWidth > _width)
			_label->setScale(_width / labelWidth);
	}

	if (_label->getScale() >= _scale)
	{
		_label->setScale(_scale);
	}

	// Update content size
	Vec2 newContentSize(16, 0);

	if (_absoluteWidth)
	{
		newContentSize.width += _width;
	}
	else
	{
		float labelWidth = _label->getContentSize().width;

		if (_minWidth < (labelWidth * _label->getScale()))
		{
			newContentSize.width += labelWidth * _label->getScale();
		}
		else
		{
			newContentSize.width += _minWidth;
		}
	}

	if (_height == 0)
	{
		newContentSize.height = _label->getContentSize().height + 4;
	}
	else
	{
		newContentSize.height = _height;
	}

	_buttonTexture->setContentSize(newContentSize);
	this->setContentSize(_buttonTexture->getContentSize());

	// Update positions
	_label->setPosition({ _spriteOffset.x + (this->getContentSize().width / 2), _spriteOffset.y + (this->getContentSize().height / 2) });
	_buttonTexture->setPosition(this->getContentSize() / 2);

	if (this->getParent())
	{
		MenuItemSpriteExtra* parent = dynamic_cast<MenuItemSpriteExtra*>(this->getParent());
		parent->setContentSize(this->getContentSize());
		this->setPosition(this->getContentSize() / 2);
	}
}

void ButtonSprite::setColor(Color3B color)
{
	if (_label)
		_label->setColor(color);

	if (_subBGSprite)
		_subBGSprite->setColor(color);

	if (_buttonTexture)
		_buttonTexture->setColor(color);

	if (_subSprite)
		_subSprite->setColor(color);
}

void ButtonSprite::updateSpriteBGSize()
{
	// Update content size
	Vec2 newContentSize;

	if (_subBGSprite)
	{
		newContentSize = _subBGSprite->getContentSize();
	}
	else
	{
		if (_absoluteWidth)
		{
			newContentSize.width = _width;
		}
		else
		{
			float labelWidth = _subSprite->getContentSize().width;

			if (_minWidth < (labelWidth * _subSprite->getScale()))
			{
				newContentSize.width = labelWidth * _subSprite->getScale();
			}
			else
			{
				newContentSize.width = _minWidth;
			}
		}

		if (_buttonTexture)
		{
			Rect defaultRect = { 0,0,40,40 };

			newContentSize.width += 8;

			if (defaultRect.size.height >= (_subSprite->getContentSize().height * _subSprite->getScale()) + 8)
				newContentSize.height = defaultRect.size.height;
			else
				newContentSize.height = (_subSprite->getContentSize().height * _subSprite->getScale()) + 8;

			_buttonTexture->setContentSize(newContentSize);
		}
	}

	this->setContentSize(newContentSize);

	// Update positions
	_subSprite->setPosition(
		(this->getContentSize().width / 2) + _spriteOffset.x,
		((this->getContentSize().height / 2) + 2) + _spriteOffset.y
	);

	if (_buttonTexture)
	{
		_buttonTexture->setPosition(this->getContentSize() / 2);
	}
	else if (_subBGSprite)
	{
		_subBGSprite->setPosition(this->getContentSize() / 2);
	}

	if (this->getParent())
	{
		MenuItemSpriteExtra* parent = dynamic_cast<MenuItemSpriteExtra*>(this->getParent());
		parent->setContentSize(this->getContentSize());
		this->setPosition(this->getContentSize() / 2);
	}
}
