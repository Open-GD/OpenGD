#pragma once

#include <axmol.h>

#include "PlayerObject.h"
#include "GroundLayer.h"
#include "MenuLayer.h"

class PlayLayer : public ax::Layer {
private:
    bool init();

    ax::Sprite* m_pBG;
    GroundLayer* m_pGround;
    PlayerObject* m_pPlayer;
    ax::Vec2 m_obCamPos;

    float m_fCameraYCenter;
    bool m_bFirstAttempt = true;
    bool m_bMoveCameraX;
    bool m_bMoveCameraY;
    bool m_bShakingCamera;
    float m_fEndOfLevel = 2000;
    float m_fShakeIntensity = 1;

public:
    void update(float delta);
    void updateCamera(float dt);
    static ax::Scene* scene();
    CREATE_FUNC(PlayLayer);
};