#pragma once

#include <axmol.h>

#include "GroundLayer.h"
#include "MenuItemSpriteExtra.h"
#include "MenuLayer.h"
#include "PlayerObject.h"
#include "SimpleProgressBar.h"

#include "GJGameLevel.h"

class PlayLayer : public ax::Layer {
private:
	bool init(GJGameLevel* level);
	void onEnter() override;
	void onExit() override;
	void onDrawImGui();
	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);

	void fillColorChannel(std::vector<std::string>& colorString, int id);

	ax::Sprite* m_pBG;
	ax::SpriteBatchNode* _mainBatchNode;
	GroundLayer* _bottomGround, * _ceiling;
	PlayerObject* m_pPlayer;
	ax::Vec2 m_obCamPos;

	MenuItemSpriteExtra* backbtn;

	ax::DrawNode* dn;

	std::vector<GameObject*> _pObjects;

	std::vector<std::vector<GameObject*>> m_pSectionObjects;

	float m_fCameraYCenter;
	float m_lastObjXPos = 570.0f;
	bool m_bFirstAttempt = true;
	bool m_bMoveCameraX;
	bool m_bMoveCameraY;
	bool m_bShakingCamera;
	float m_fEndOfLevel = FLT_MAX;
	float m_fShakeIntensity = 1;

	int _prevSection, _nextSection;

	bool m_bIsJumpPressed;

	SimpleProgressBar* m_pBar;

	//----IMGUI DEBUG MEMBERS----
	bool m_freezePlayer;
	bool m_platformerMode;

public:
	ax::ParticleBatchNode* _particleBatchNode;

	std::map<int, ax::Color3B> m_pColorChannels, _originalColors;

	AX_SYNTHESIZE(GJGameLevel*, _pLevel, Level);

	void destroyPlayer();

	void loadLevel(std::string levelStr);

	void update(float delta);
	void updateCamera(float dt);
	void updateVisibility();
	void moveCameraToPos(ax::Vec2);
	void changeGameMode(GameObject*, int);
	void resetLevel();

	void tweenBottomGround(float y);
	void tweenCeiling(float y);

	// dt?
	void checkCollisions(float delta);
	void renderRect(ax::Rect rect, ax::Color4B col);

	void processTriggers();

	int sectionForPos(float x);

	static ax::Scene* scene(GJGameLevel* level);
	static PlayLayer* create(GJGameLevel* level);

	static PlayLayer* getInstance();
};