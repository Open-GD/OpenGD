#pragma once
#include <axmol.h>

class GroundLayer : public ax::Layer {
    ax::Sprite* m_pSprite;
    float m_fOneGroundSize;

    AX_SYNTHESIZE(float, m_fSpeed, Speed)

public:
    void update(float dt);
    bool init(int groundID);
    static GroundLayer* create(int groundID);
};