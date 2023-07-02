/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#pragma once
#include "GameObject.h"
#include "Types.h"
#include "math/Vec2.h"

class GameObject;
class MotionTrail;

namespace ax 
{ 
	class Layer; 
	class Sprite;
	class ParticleSystemQuad;
	class Texture2D;
}


enum PlayerGamemode
{
	PlayerGamemodeCube = 0,
	PlayerGamemodeShip = 1,
	PlayerGamemodeBall = 2,
	PlayerGamemodeUFO = 3,
	PlayerGamemodeWave = 4,
	PlayerGamemodeRobot = 5,
	PlayerGamemodeSpider = 6,
};

class PlayerObject : public GameObject
{
  private:
	void updateJump(float dt);
	bool init(int, ax::Layer*);
	void runRotateAction();
	void runBallRotation();

	void logValues();

	ax::Layer* gameLayer;
	bool inPlayLayer;

	ax::Sprite* m_pMainSprite;
	ax::Sprite* m_pSecondarySprite;
	ax::Sprite* m_pShipSprite;
	ax::Sprite* m_pShipSecondarySprite;
	ax::Sprite* _ballSprite;
	ax::Sprite* _ballSecondarySprite;
	ax::Sprite* _ufoSprite;
	ax::Sprite* _ufoSecondarySprite;
	ax::Sprite* _ufoTertiarySprite;

	ax::ParticleSystemQuad* dragEffect1;
	ax::ParticleSystemQuad* dragEffect2;
	ax::ParticleSystemQuad* dragEffect3;
	ax::ParticleSystemQuad* shipDragEffect;
	ax::ParticleSystemQuad* landEffect1;
	ax::ParticleSystemQuad* landEffect2;

	
	double m_dYVel = 0;
	double m_dGravity = 0.958199;
	double m_dJumpHeight = 11.180032;

	bool m_bOnGround;

	bool m_bIsDead;
	bool m_bIsLocked;

	bool m_bGravityFlipped;

	bool m_isRising;

	bool _particles1Activated;
	bool _particles2Activated;
	bool _particles3Activated;

	float m_playerSpeed = 0.9f;
	float m_snapDifference;

	ax::Vec2 m_obLastGroundPos;
	ax::Vec2 m_prevPos;

	GameObject* m_snappedObject;

  public:

  double m_dXVel = 5.770002;

	GameObject* _touchedPadObject;

	bool _mini = false;
	bool m_bIsHolding;
	bool _hasJustHeld;

	int _jumpedTimes;

	static ax::Texture2D* motionStreakTex;
	MotionTrail* motionStreak;

	PlayerGamemode _currentGamemode;

	GameObject* _touchedRingObject;
	
	bool _hasRingJumped;
	bool _queuedHold;

	void reset();

	bool m_bIsPlatformer;
	float direction, _vehicleSize = 1.f;

	static PlayerObject* create(int, ax::Layer*);

	void setMainColor(ax::Color3B col);
	void setSecondaryColor(ax::Color3B col);

	ax::Color3B getMainColor();
	ax::Color3B getSecondaryColor();

	void jump();
	void collidedWithObject(float dt, GameObject* obj);
	void checkSnapJumpToObject(GameObject* obj);

	void updateShipRotation(float dt);
	bool isDead();
	bool isOnGround();
	bool isGravityFlipped();
	bool isRestricted()
	{
		return _currentGamemode == PlayerGamemodeShip || _currentGamemode == PlayerGamemodeSpider ||
			   _currentGamemode == PlayerGamemodeBall;
	}
	void stopRotation();
	void toggleMini(bool active);
	float flipMod();

	bool playerIsFalling();

	double getYVel()
	{
		return m_dYVel;
	}
	void setYVel(double yVel)
	{
		m_dYVel = yVel;
	}

	void setIsDead(bool);
	void setIsOnGround(bool);
	void setGamemode(PlayerGamemode mode);

	ax::Layer* getPlayLayer()
	{
		return gameLayer;
	}

	void playDeathEffect();

	void hitGround(bool reverseGravity);

	void flipGravity(bool gravity);

	bool noclip;

	ax::Vec2 getLastGroundPos();
	void setLastGroundPos(ax::Vec2 pos)
	{
		m_obLastGroundPos = pos;
	}
	void update(float dt);

	float getPlayerSpeed()
	{
		return m_playerSpeed;
	}
	void setPlayerSpeed(float v)
	{
		m_playerSpeed = v;
	}

	void propellPlayer(double force);

	void setTouchedRing(GameObject* obj);
	void ringJump(GameObject* obj);

	void activateStreak();
	void deactivateStreak();

	void pushButton();
	void releaseButton();

	ax::Vec2 _portalP;
	ax::Vec2 _lastP;

	ax::Vec2 getPortalP()
	{
		return _portalP;
	}
	ax::Vec2 getLastP()
	{
		return _lastP;
	}
	void setPortalP(ax::Vec2 portalP)
	{
		_portalP = portalP;
	}
	void setLastP(ax::Vec2 lastP)
	{
		_lastP = lastP;
	}

	GameObject* _portalObject;

	GameObject* getPortalObject()
	{
		return _portalObject;
	}
	void setPortalObject(GameObject* portal)
	{
		_portalObject = portal;
	}

	void spawnPortalCircle(ax::Color4B color, float radius);
};