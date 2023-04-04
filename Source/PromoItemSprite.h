#pragma once
#include <axmol.h>

class PromoItemSprite : public ax::MenuItemSprite {
private:
	PromoItemSprite(const char* sprite, std::function<void(Node*)> callback);
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
	static PromoItemSprite* create(const char* sprite, std::function<void(Node*)> callback);
	
	ax::Node* getSprite();
};