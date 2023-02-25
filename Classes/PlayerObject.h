#pragma once
#include <axmol.h>
#include "GameObject.h"
#include "CircleWave.h"

class PlayerObject : public GameObject {
private:
    void updateJump(float dt);
    bool init(int, ax::Layer*);
    void runRotateAction();
    void stopRotation();

    void logValues();

    ax::Layer* gameLayer;
    bool inPlayLayer;

    ax::Sprite* m_pMainSprite;
    ax::Sprite* m_pSecondarySprite;
    ax::Sprite* m_pShipSprite;

    ax::ParticleSystemQuad* dragEffect1;
    ax::ParticleSystemQuad* dragEffect2;
    ax::ParticleSystemQuad* dragEffect3;
    ax::ParticleSystemQuad* shipDragEffect;
    ax::ParticleSystemQuad* landEffect1;
    ax::ParticleSystemQuad* landEffect2;

    ax::MotionStreak* motionStreak;

    double m_dXVel = 5.770002;
    double m_dYVel = 0;
    double m_dGravity = 0.958199;
    double m_dJumpHeight = 11.180032;

    bool m_bUpsideDown;
    bool m_bOnGround;

    float m_fSpeed = 0.9f;
    
    bool m_bIsDead;
    bool m_bIsLocked;

    bool m_bIsHolding;

    bool m_bFlying;

    ax::Vec2 m_obLastGroundPos;

public:
    static PlayerObject* create(int, ax::Layer*);
     
    bool onTouchBegan(ax::Touch* touch, ax::Event* event);

    void setMainColor(ax::Color3B col);
    void setSecondaryColor(ax::Color3B col);
    void setShipColor(ax::Color3B col);

    ax::Color3B getMainColor();
    ax::Color3B getSecondaryColor();
    ax::Color3B getShipColor();

    void jump();

    bool isFlying();
    bool isUpsideDown();
    bool isDead();
    
    ax::Vec2 getLastGroundPos();
    void update(float dt);
};