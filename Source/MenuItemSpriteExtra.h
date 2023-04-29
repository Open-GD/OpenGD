#pragma once

#include <string_view>
#include <functional>

#include "2d/CCMenuItem.h"
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
};