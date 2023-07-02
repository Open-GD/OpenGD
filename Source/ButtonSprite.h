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

#include <string_view>

#include "2d/Sprite.h"
#include "math/Vec2.h"
#include "Types.h"

namespace ax 
{ 
	class Label;

	namespace ui 
	{ 
		class Scale9Sprite; 
	} 
}


class ButtonSprite : public ax::Sprite
{
	ax::ui::Scale9Sprite* _buttonTexture;
	Sprite* _subSprite;
	Sprite* _subBGSprite;

	float _minWidth;
	ax::Point _spriteOffset;

	ax::Label* _label;

	float _scale;
	float _width;
	float _height;

	bool _absoluteWidth;
	bool _hasTextProvided;

public:
	/**
	 * Creates a text button.
	 *
	 * @param caption   Text to display.
	 * @param width   The maximum width the label can have before it shrinks.
	 * @param minWidth   The minimum width of the button.
	 * @param scale   The maximum scale of the label.
	 * @param absoluteWidth   Whether or not the width of the button is locked.
	 * @param font   The font to be used for the label.
	 * @param texture   The name of the button texture to be used.
	 * @param height   The height for the button.
	 * 
	 * @return A ButtonSprite
	 */
	static ButtonSprite* create(std::string_view caption,
		int width, int minWidth, float scale, bool absoluteWidth,
		std::string_view font, std::string_view texture, float height
	);

	/**
	 * Creates a sprite button.
	 *
	 * @param iconSprite	The icon sprite for the button.
	 * @param width   The maximum width the icon sprite can have before it shrinks.
	 * @param minWidth   The minimum width of the button.
	 * @param height   The height for the button.
	 * @param scale   The maximum scale of the label.
	 * @param absoluteWidth   Whether or not the width of the button is locked.
	 * @param texture   The name of the button texture to be used (Note: Must be from a sprite frame).
	 * @param dontUseScale9   Uses Sprite if set to true, uses Scale9Sprite otherwise.
	 *
	 * @return A ButtonSprite
	 */
	static ButtonSprite* create(Sprite* iconSprite,
		int width, int minWidth, float height, float scale,
		bool absoluteWidth, std::string_view texture, bool dontUseScale9
	);

	static ButtonSprite* create(std::string_view caption, int width, float minWidth, float scale);
	static ButtonSprite* create(std::string_view caption);

	bool initWithText(std::string_view caption,
		int width, int minWidth, float scale, bool absoluteWidth,
		std::string_view font, std::string_view texture, float height
	);

	bool initWithSprite(Sprite* iconSprite,
		int width, int minWidth, float height, float scale,
		bool absoluteWidth, std::string_view texture, bool dontUseScale9
	);

	void updateBGImage(std::string_view file);
	void setString(std::string_view str);
	void setColor(ax::Color3B color);
	void updateSpriteBGSize();
};
