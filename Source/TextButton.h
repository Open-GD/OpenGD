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
#include <functional>

#include "2d/MenuItem.h"
#include "PlatformMacros.h"

namespace ax 
{ 
	class Label;
	namespace ui 
	{ 
		class Scale9Sprite; 
	} 
}

class TextButton : public ax::MenuItemSprite {
private:
	bool init(std::string_view text, std::string_view font, float width, float height, std::string_view bgTexture, std::function<void(TextButton*)> callback);
	void selected() override;
	void unselected() override;
	void activate() override;

	ax::ui::Scale9Sprite* m_pBG;
	ax::Label* m_pText;
	std::function<void(TextButton*)> m_fCallback;
	float m_fWidth;
	float m_fHeight;
	AX_SYNTHESIZE(float, m_fNewScale, NewScale)

public:

	/// @brief Basically a ButtonSprite implementation.
	/// @param text The button text
	/// @param font The font of the text
	/// @param width The width of the bg. Leave 0 for auto-adjusting
	/// @param height The height of the bg. Leave 0 for auto-adjusting
	/// @param bgTexture The texture of the bg
	/// @param callback The callback function, called on button release. Can be a lambda. The Node* is the button itself
	/// @return A new button
	static TextButton* create(std::string_view text, std::string_view font, float width, float height, std::string_view bgTexture, std::function<void(TextButton*)> callback);

	/// @brief Basically a ButtonSprite implementation.
	/// @param text The button text
	/// @param font The font of the text
	/// @param width The width of the bg. Leave 0 for auto-adjusting
	/// @param height The height of the bg. Leave 0 for auto-adjusting
	/// @param callback The callback function, called on button release. Can be a lambda. The Node* is the button itself
	/// @return A new button
	static TextButton* create(std::string_view text, std::string_view font, float width, float height, std::function<void(TextButton*)> callback);

	/// @brief Basically a ButtonSprite implementation.
	/// @param text The button text
	/// @param font The font of the text
	/// @param callback The callback function, called on button release. Can be a lambda. The Node* is the button itself
	/// @return A new button
	static TextButton* create(std::string_view text, std::string_view font, std::function<void(TextButton*)> callback);

	/// @brief Basically a ButtonSprite implementation.
	/// @param text The button text
	/// @param callback The callback function, called on button release. Can be a lambda. The Node* is the button itself
	/// @return A new button
	static TextButton* create(std::string_view text, std::function<void(TextButton*)> callback);

	void setText(std::string_view text);
	void setCallback(std::function<void(TextButton*)> callback);
	
	ax::Label* getLabel();
	ax::ui::Scale9Sprite* getBG();
};