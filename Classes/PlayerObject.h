#pragma once
#include "CircleWave.h"
#include "GameObject.h"
#include <axmol.h>
#include <MotionTrail.h>

class PlayerObject : public GameObject 
{
private:
	void updateJump(float dt);
	bool init(int, ax::Layer*);
	void runRotateAction();

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

	MotionTrail* motionStreak;

	double m_dXVel = 5.770002;
	double m_dYVel = 0;
	double m_dGravity = 0.958199;
	double m_dJumpHeight = 11.180032;

	bool m_bOnGround;

	bool m_bIsDead;
	bool m_bIsLocked;
	bool m_bIsHolding;
	bool _isHoldingFromGround;

	bool m_bGravityFlipped;

	bool m_bIsShip;

	float m_playerSpeed = 0.9f;

	ax::Vec2 m_obLastGroundPos;

	GameObject* m_snappedObject;
	float m_snapDifference;

	ax::Vec2 m_prevPos;

	bool m_isRising;

	bool _particles1Activated;
	bool _particles2Activated;
	bool _particles3Activated;

public:

	GameObject* _touchedRingObject;
	bool _hasRingJumped;

	void reset();

	bool m_bIsPlatformer;
	float direction;

	static PlayerObject* create(int, ax::Layer*);

	bool onTouchBegan(ax::Touch* touch, ax::Event* event);
	void onTouchEnded(ax::Touch* touch, ax::Event* event);

	void setMainColor(ax::Color3B col);
	void setSecondaryColor(ax::Color3B col);
	void setShipColor(ax::Color3B col);

	ax::Color3B getMainColor();
	ax::Color3B getSecondaryColor();
	ax::Color3B getShipColor();

	void jump();
	void collidedWithObject(float dt, GameObject* obj);
	void checkSnapJumpToObject(GameObject* obj);

	void updateShipRotation(float dt);
	bool isShip();
	void setIsShip(bool);
	bool isDead();
	bool isOnGround();
	bool isGravityFlipped();
	void stopRotation();
	float flipMod();

	bool playerIsFalling();

	double getYVel() { return m_dYVel; }

	void setIsDead(bool);
	void setIsOnGround(bool);

	ax::Layer* getPlayLayer() { return gameLayer; }

	void playDeathEffect();

	void hitGround(bool reverseGravity);

	void flipGravity(bool gravity);

	bool noclip;

	ax::Vec2 getLastGroundPos();
	void setLastGroundPos(ax::Vec2 pos) { m_obLastGroundPos = pos; }
	void update(float dt);

	float getPlayerSpeed() { return m_playerSpeed; }
	void setPlayerSpeed(float v) { m_playerSpeed = v; }

	void propellPlayer();

	void setTouchedRing(GameObject* obj);
	void ringJump();

	void activateStreak();
	void deactivateStreak();

	ax::Vec2 _portalP;
	ax::Vec2 _lastP;

	ax::Vec2 getPortalP() { return _portalP; }
	ax::Vec2 getLastP() { return _lastP; }
	void setPortalP(ax::Vec2 portalP) { _portalP = portalP; }
	void setLastP(ax::Vec2 lastP) { _lastP = lastP; }



	GameObject* _portalObject;

	GameObject* getPortalObject() { return _portalObject; }
	void setPortalObject(GameObject* portal) { _portalObject = portal; }

	void spawnPortalCircle(ax::Color4B color, float radius);
};