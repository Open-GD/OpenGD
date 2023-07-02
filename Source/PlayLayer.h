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


class PlayLayer : public ax::Layer
{
  private:
	bool init(GJGameLevel* level);
	void onEnter() override;
	void onExit() override;
	void onDrawImGui();
	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void createLevelEnd();

	void fillColorChannel(std::span<std::string_view> colorString, int id);

	ax::Sprite* m_pBG;
	GroundLayer *_bottomGround, *_ceiling;
	
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
	ax::Label* m_pPercentage;

	LevelSettings _levelSettings;

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

	PlayerObject* _player1;
	PlayerObject* _player2;

	bool _isDualMode;

	std::string _mainBatchNodeTexture = "GJ_GameSheet.png";
	std::string _main2BatchNodeTexture = "GJ_GameSheet02.png";

	ax::SpriteBatchNode *_mainBatchNodeB4, *_mainBatchNodeB3, *_mainBatchNodeB2, *_mainBatchNodeB1, *_mainBatchNodeT1,
		*_mainBatchNodeT2, *_mainBatchNodeT3;
	ax::SpriteBatchNode *_blendingBatchNodeB4, *_blendingBatchNodeB3, *_blendingBatchNodeB2, *_blendingBatchNodeB1,
		*_blendingBatchNodeT1, *_blendingBatchNodeT2, *_blendingBatchNodeT3;
	ax::SpriteBatchNode* _main2BatchNode;
	ax::SpriteBatchNode* _glowBatchNode;
	ax::ParticleBatchNode* _particleBatchNode;

	std::unordered_map<int, SpriteColor, my_string_hash> m_pColorChannels, _originalColors;
	std::unordered_map<int, GroupProperties, my_string_hash> _groups;

	AX_SYNTHESIZE(GJGameLevel*, _pLevel, Level);

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
	
	bool isObjectBlending(GameObject* obj);

	int sectionForPos(float x);

	void changePlayerSpeed(int speed);
	void changeGravity(bool gravityFlipped);

	void incrementTime();

	ax::Color3B getLightBG();

	static ax::Scene* scene(GJGameLevel* level);
	static PlayLayer* create(GJGameLevel* level);

	static PlayLayer* getInstance();
};