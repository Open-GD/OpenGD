#pragma once
#include <axmol.h>

class MenuItemSpriteExtra : public ax::MenuItemSprite {
private:
	MenuItemSpriteExtra(const char* sprite, ax::Sprite* sprNode, std::function<void(Node*)> callback);
	bool init() override;
	void selected() override;
	void unselected() override;
	void activate() override;

	ax::Sprite* m_pSprite;
	float m_fAnimDuration;
	float m_fSelectedScale;
	float m_fUnselectedScale;
	std::function<void(ax::Node*)> m_fCallback;

public:
	/// @brief Create a funny scaling button like in gd
	/// @param sprite The name of the sprite FRAME (it should be in a plist)
	/// @param callback Runs when you release the button with mouse hovered. Can be a lambda func. The Node* param is the button itself
	/// @return The button
	static MenuItemSpriteExtra* create(const char* sprite, std::function<void(Node*)> callback);
	static MenuItemSpriteExtra* create(ax::Sprite* , std::function<void(Node*)> callback);
	
	ax::Sprite* getSprite();
	void setScale(float) override;
};