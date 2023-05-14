#pragma once
#include <span>
#include <string_view>
#include <vector>

#include "2d/CCLayer.h"
#include "CCEventKeyboard.h"
#include "BaseGameLayer.h"
#include "SpriteColor.h"
#include "CCPlatformMacros.h"
#include "ccTypes.h"

enum PlayerGamemode;

class GJGameLevel;
class GameObject;
class SimpleProgressBar;
class UILayer;
class PlayerObject;
class GroundLayer;
class MenuItemSpriteExtra;

namespace ax 
{ 
	class Event; 
	class Sprite;
	class Label;
}


class PlayLayer : public BaseGameLayer
{
  private:
	bool init(GJGameLevel* level) override;
	void onEnter() override;
	void onExit() override;
	void onDrawImGui();
	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void createLevelEnd();

	void fillColorChannel(std::span<std::string_view> colorString, int id) override;

	ax::Sprite* m_pBG;
	GroundLayer *_bottomGround, *_ceiling;
	
	ax::Vec2 m_obCamPos;

	MenuItemSpriteExtra* backbtn;

	ax::DrawNode* dn;

	std::vector<GameObject*> _pObjects;

	float m_fCameraYCenter;
	float m_lastObjXPos = 570.0f;
	bool m_bFirstAttempt = true;
	bool m_bMoveCameraX;
	bool m_bMoveCameraY;
	bool m_bShakingCamera;
	float m_fEndOfLevel = FLT_MAX;
	float m_fShakeIntensity = 1;

	bool m_bIsJumpPressed;

	SimpleProgressBar* m_pBar;
	ax::Label* m_pPercentage;

	//----IMGUI DEBUG MEMBERS----
	bool m_freezePlayer;
	bool m_platformerMode;

	bool m_bEndAnimation;

public:
	int _enterEffectID = 0;

	int _groundID = 1;
	int _bgID = 1;

	UILayer* m_pHudLayer;

	int _secondsSinceStart;
	int _attempts;
	int _jumps;
	bool _everyplay_recorded;

	std::vector<bool> _coinsCollected;

	bool _isDualMode;

	void destroyPlayer(PlayerObject* player);

	void loadLevel(std::string_view levelStr);

	void spawnCircle();
	void showEndLayer();
	void showCompleteText();

	void update(float delta) override;
	void updateCamera(float dt);
	void updateVisibility();
	void moveCameraToPos(ax::Vec2);
	void changeGameMode(GameObject* obj, PlayerObject* player, PlayerGamemode gameMode);
	void resetLevel();
	void exit();

	void tweenBottomGround(float y);
	void tweenCeiling(float y);

	// dt?
	void checkCollisions(PlayerObject* player, float delta);
	void renderRect(ax::Rect rect, ax::Color4B col);

	void applyEnterEffect(GameObject* obj);
	float getRelativeMod(ax::Vec2 objPos, float v1, float v2, float v3);
	
	bool isObjectBlending(GameObject* obj) override;

	int sectionForPos(float x);

	void changePlayerSpeed(int speed);
	void changeGravity(bool gravityFlipped);

	void incrementTime();

	ax::Color3B getLightBG();

	static ax::Scene* scene(GJGameLevel* level);
	static PlayLayer* create(GJGameLevel* level);

	static PlayLayer* getInstance();
};