#pragma once
#include <axmol.h>

class SimplePlayer : public ax::Sprite {
private:
    bool init(int cubeID);

    ax::Sprite* m_pMainSprite;
    ax::Sprite* m_pSecondarySprite;
    
public:
    static SimplePlayer* create(int cubeID);

    void setMainColor(ax::Color3B col);
    void setSecondaryColor(ax::Color3B col);
};