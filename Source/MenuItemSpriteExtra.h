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
#include "math/Vec2.h"

namespace ax 
{ 
	class Node; 
}


class MenuItemSpriteExtra : public ax::MenuItemSprite {
public:
	MenuItemSpriteExtra(std::string_view sprite, ax::Node* sprNode, std::function<void(ax::Node*)> callback);
	bool init() override;
	void selected() override;
	void unselected() override;
	void activate() override;

	ax::Node* m_pSprite;
	float m_fAnimDuration;
	float m_fSelectedScale;
	float m_fUnselectedScale;
	float m_fScaleMult;
	std::function<void(ax::Node*)> m_fCallback;

	ax::Vec2 _startPos;
	ax::Vec2 _offsetPos;
	bool _altAnim = false;

	/// @brief Create a funny scaling button like in gd
	/// @param sprite The name of the sprite FRAME (it should be in a plist)
	/// @param callback Runs when you release the button with mouse hovered. Can be a lambda func. The Node* param is the button itself
	/// @return The button
	static MenuItemSpriteExtra* create(std::string_view sprite, std::function<void(Node*)> callback);
	static MenuItemSpriteExtra* create(ax::Node* , std::function<void(ax::Node*)> callback);
	virtual void setScaleMultiplier(float);
	void setDestination(ax::Vec2 dest);
	
	ax::Node* getSprite();
	void setScale(float) override;
	void setCallback(std::function<void(ax::Node*)> callback);
	void setSpriteFrame(std::string_view spriteFrame);
};
