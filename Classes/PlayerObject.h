#pragma once
#include <axmol.h>
#include "GameObject.h"
#include "CircleWave.h"

class PlayerObject : public GameObject
{
private:
    void updateJump(float dt);
    bool init(int, ax::Layer *);
    void runRotateAction();

    void logValues();

    ax::Layer *gameLayer;
    bool inPlayLayer;

    ax::Sprite *m_pMainSprite;
    ax::Sprite *m_pSecondarySprite;
    ax::Sprite *m_pShipSprite;

    ax::ParticleSystemQuad *dragEffect1;
    ax::ParticleSystemQuad *dragEffect2;
    ax::ParticleSystemQuad *dragEffect3;
    ax::ParticleSystemQuad *shipDragEffect;
    ax::ParticleSystemQuad *landEffect1;
    ax::ParticleSystemQuad *landEffect2;

    ax::MotionStreak *motionStreak;

    double m_dXVel = 5.770002;
    double m_dYVel = 0;
    double m_dGravity = 0.958199;
    double m_dJumpHeight = 11.180032;

    bool m_bUpsideDown;
    bool m_bOnGround;

    bool m_bIsDead;
    bool m_bIsLocked;
    bool m_bIsRising;
    bool m_bIsHolding;

    bool m_bGravityFlipped;

    bool m_bIsShip;

    float m_playerSpeed = 0.9f;

    ax::Vec2 m_obLastGroundPos;

    GameObject* m_snappedObject;
    float m_snapDifference;

    ax::Vec2 m_prevPos;

    bool m_isRising;
public:
    void reset();

    bool m_bIsPlatformer;
    float direction;

    static PlayerObject *create(int, ax::Layer *);

    bool onTouchBegan(ax::Touch *touch, ax::Event *event);
    void onTouchEnded(ax::Touch *touch, ax::Event *event);

    void setMainColor(ax::Color3B col);
    void setSecondaryColor(ax::Color3B col);
    void setShipColor(ax::Color3B col);

    ax::Color3B getMainColor();
    ax::Color3B getSecondaryColor();
    ax::Color3B getShipColor();

    void jump();
    void collidedWithObject(float dt, GameObject* obj);
    void checkSnapJumpToObject(GameObject* obj);

    void updateShipRotation();
    bool isShip();
    void setShip(bool ship);
    bool isUpsideDown();
    bool isDead();
    bool isOnGround();
    bool isGravityFlipped();
    void stopRotation();
    float flipMod();

    bool playerIsFalling();

    double getYVel() { return m_dYVel; }

    inline void setDead(bool const &value) { m_bIsDead = value; }
    inline void setOnGround(bool const &value) { m_bOnGround = value; }

    ax::Layer *getPlayLayer() { return gameLayer; }

    void playDeathEffect();

    void hitGround(bool reverseGravity);

    void flipGravity(bool gravity);

    bool noclip;

    ax::Vec2 getLastGroundPos();
    void setLastGroundPos(ax::Vec2 pos) {m_obLastGroundPos = pos;}
    void update(float dt);

    float getPlayerSpeed() { return m_playerSpeed; }
    void setPlayerSpeed(float v) { m_playerSpeed = v; }

    void propellPlayer();
};