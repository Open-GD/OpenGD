#pragma once

#include <axmol.h>

#include "PlayerObject.h"
#include "GroundLayer.h"
#include "MenuLayer.h"
#include "SimpleProgressBar.h"
#include "MenuItemSpriteExtra.h"

#include "GJGameLevel.h"

class PlayLayer : public ax::Layer
{
private:
    bool init(GJGameLevel *level);
    void onEnter() override;
    void onExit() override;
    void onDrawImGui();

    ax::Sprite *m_pBG;
    GroundLayer *m_pGround;
    PlayerObject *m_pPlayer;
    ax::Vec2 m_obCamPos;

    MenuItemSpriteExtra *backbtn;

    ax::DrawNode *dn;

    std::vector<GameObject *> _pObjects;

    std::vector<std::vector<GameObject*>> m_pSectionObjects;

    float m_fCameraYCenter;
    bool m_bFirstAttempt = true;
    bool m_bMoveCameraX;
    bool m_bMoveCameraY;
    bool m_bShakingCamera;
    float m_fEndOfLevel = FLT_MAX;
    float m_fShakeIntensity = 1;

    SimpleProgressBar *m_pBar;

    //----IMGUI DEBUG MEMBERS----
    bool m_freezePlayer;

public:
    AX_SYNTHESIZE(GJGameLevel *, _pLevel, Level);

    void update(float delta);
    void updateCamera(float dt);

    // dt?
    void checkCollisions(float delta);
    void renderRect(ax::Rect rect, ax::Color4B col);

    int sectionForPos(float x)
    {
        int section = x / 100;
        if (section < 0)
            section = 0;
        return section;
    }

    static ax::Scene *scene(GJGameLevel *level);
    static PlayLayer *create(GJGameLevel *level)
    {
        auto ret = new (std::nothrow) PlayLayer();
        if (ret && ret->init(level))
        {
            ret->autorelease();
            return ret;
        }

        AX_SAFE_DELETE(ret);
        return nullptr;
    }
};