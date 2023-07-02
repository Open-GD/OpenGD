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
#include <functional>

#include "2d/MenuItem.h"

namespace ax 
{ 
	class Node; 
}


class PromoItemSprite : public ax::MenuItemSprite {
private:
	PromoItemSprite(const char* sprite, std::function<void(ax::Node*)> callback);
	bool init() override;
	void selected() override;
	void unselected() override;
	void activate() override;

	ax::Node* m_pSprite;
	std::function<void(ax::Node*)> m_fCallback;

public:
	/// @brief Create a promo button
	/// @param sprite The name of the sprite
	/// @param callback Runs when you release the button with mouse hovered. Can be a lambda func. The Node* param is the button itself
	/// @return The button
	static PromoItemSprite* create(const char* sprite, std::function<void(ax::Node*)> callback);
	
	ax::Node* getSprite();
};