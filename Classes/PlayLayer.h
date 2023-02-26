#pragma once

#include <axmol.h>

#include "PlayerObject.h"
#include "GroundLayer.h"
#include "MenuLayer.h"

// temporary
struct GJGameLevel 
{
	std::string _pLevelName;
	std::string _pLevelCreator;
	std::string _pLevelData;
	unsigned int _pLevelID;

	GJGameLevel() 
		: _pLevelName(""), 
		_pLevelCreator(""), 
		_pLevelData(""), 
		_pLevelID(1) 
	{

	}

	GJGameLevel(std::string_view levelName, std::string_view levelCreator, unsigned int levelID)
		: _pLevelName(levelName),
		_pLevelCreator(levelCreator),
		_pLevelData(""),
		_pLevelID(levelID)
	{

	}
};

class PlayLayer : public ax::Layer {
private:
	bool init(GJGameLevel* level);

	ax::Sprite* m_pBG;
	GroundLayer* m_pGround;
	PlayerObject* m_pPlayer;
	ax::Vec2 m_obCamPos;
	
	std::vector<GameObject*> _pObjects;

	float m_fCameraYCenter;
	bool m_bFirstAttempt = true;
	bool m_bMoveCameraX;
	bool m_bMoveCameraY;
	bool m_bShakingCamera;
	float m_fEndOfLevel = INT_MAX;
	float m_fShakeIntensity = 1;

public:
	void update(float delta);
	void updateCamera(float dt);

	// dt?
	void checkCollisions(float dtMaybe);
	
	static ax::Scene* scene(GJGameLevel* level);
	static PlayLayer* create(GJGameLevel* level)
	{
		auto ret = new (std::nothrow) PlayLayer();
		if(ret && ret->init(level))
		{ 
			ret->autorelease();
			return ret;
		}

		AX_SAFE_DELETE(ret);
		return nullptr;
	}
};

