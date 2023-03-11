#pragma once
#include <axmol.h>

class MenuItemSpriteExtra : public ax::MenuItemSprite {
private:
    MenuItemSpriteExtra(const char* sprite, ax::Node* sprNode, std::function<void(Node*)> callback);
    bool init() override;
    void selected() override;
    void unselected() override;
    void activate() override;

    ax::Node* m_pSprite;
    float m_fAnimDuration;
    std::function<void(ax::Node*)> m_fCallback;
    AX_SYNTHESIZE(float, m_fNewScale, NewScale)

public:
    /// @brief Create a funny scaling button like in gd
    /// @param sprite The name of the sprite FRAME (it should be in a plist)
    /// @param callback Runs when you release the button with mouse hovered. Can be a lambda func. The Node* param is the button itself
    /// @return The button
    static MenuItemSpriteExtra* create(const char* sprite, std::function<void(Node*)> callback);
    static MenuItemSpriteExtra* createWithNode(ax::Node* sprite, std::function<void(Node*)> callback);
    ax::Node* getSprite() {return m_pSprite;}
};