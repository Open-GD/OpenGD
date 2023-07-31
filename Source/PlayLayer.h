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
#include <span>
#include <string_view>
#include <vector>

#include "2d/Layer.h"
#include "EventKeyboard.h"
#include "BaseGameLayer.h"
#include "SpriteColor.h"
#include "PlatformMacros.h"
#include "Types.h"
#include "GameToolbox/conv.h"

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

	int sectionForPos(float x);

	void changePlayerSpeed(int speed);
	void changeGravity(bool gravityFlipped);

	void incrementTime();

	ax::Color3B getLightBG();

	static ax::Scene* scene(GJGameLevel* level);
	static PlayLayer* create(GJGameLevel* level);

	static PlayLayer* getInstance();
};