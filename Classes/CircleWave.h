#pragma once
#include <axmol.h>

class CircleWave : public ax::DrawNode, public ax::ActionTweenDelegate {
private:
    void draw(ax::Renderer *renderer, const ax::Mat4& transform, uint32_t flags) override;
    virtual void updateTweenAction(float value, std::string_view key) override;
    bool init(float duration, ax::Color4B color, float radiusMin, float radiusMax, bool easing, float lineWidth, bool filled);
    void update(float dt);

    ax::Color4F m_eColor; // damn CC_SYNTHESIZE doesnt work with it :skull:
    ax::Node* m_pFollowedNode;
    AX_SYNTHESIZE(float, m_fLineWidth, LineWidth)
    AX_SYNTHESIZE(float, m_fRadius, Radius)
    AX_SYNTHESIZE(float, m_fWidth, Width)
    AX_SYNTHESIZE(bool, m_bFilled, Filled)
    
public:
    void followNode(ax::Node* node);
    static CircleWave* create(float duration, ax::Color4B color, float radiusMin, float radiusMax, bool easing, bool filled, float lineWidth = 2.0f);

    void setColor(ax::Color4B col);
    ax::Color4B getColor();
};