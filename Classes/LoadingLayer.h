#pragma once
#include <axmol.h>

class LoadingLayer : public ax::Layer {
public:
    const char* getSplash();
    static ax::Scene* scene();
    bool init();
    void loadAssets();
    void assetLoaded(ax::Ref*);
    CREATE_FUNC(LoadingLayer);
    
    int m_nAssetsLoaded = 0;
    int m_nTotalAssets = 0;
    ax::Sprite* m_pBarSprite = nullptr;
    ax::Sprite* m_pGrooveSprite = nullptr;
    float m_fTotalBarWidth = 0.0f;
};
