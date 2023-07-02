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

#include <vector>
#include <string>
#include <unordered_map>
#include <span>

#include "PlayerObject.h"
#include "SpriteColor.h"
#include "2d/Layer.h"
#include "EffectManager.h"
#include "GameToolbox/conv.h"

class GameObject;
class BaseGameLayer;
class GJGameLevel;

namespace ax 
{ 
	class SpriteBatchNode; 
	class ParticleBatchNode;
}

//follow structure
//1. private members
//2. public members
//3. private functions
//4. public functions

struct LevelSettings
{
	PlayerGamemode gamemode;
	bool mini, dual, twoPlayer, flipGravity;
	int speed;
	float songOffset;
	int _groundID = 1, _bgID = 1;
};

struct GroupProperties
{
	enum GroupState { NOT_CHANGING, MAIN_DETAIL, MAIN_ONLY, DETAIL_ONLY };
	std::vector<GameObject*> _objects;
	float _alpha = 1.f;
	ax::Color3B _color;
	GroupState groupState = NOT_CHANGING;
};

class BaseGameLayer : public ax::Layer {
protected:

	static BaseGameLayer* _instance;

	std::string _mainBatchNodeTexture = "GJ_GameSheet.png";
	std::string _main2BatchNodeTexture = "GJ_GameSheet02.png";

	ax::SpriteBatchNode* _mainBatchNodeB4;
	ax::SpriteBatchNode* _mainBatchNodeB3;
	ax::SpriteBatchNode* _mainBatchNodeB2;
	ax::SpriteBatchNode* _mainBatchNodeB1;
	ax::SpriteBatchNode* _mainBatchNodeT1;
	ax::SpriteBatchNode* _mainBatchNodeT2;
	ax::SpriteBatchNode* _mainBatchNodeT3;

	ax::SpriteBatchNode* _blendingBatchNodeB4;
	ax::SpriteBatchNode* _blendingBatchNodeB3;
	ax::SpriteBatchNode* _blendingBatchNodeB2;
	ax::SpriteBatchNode* _blendingBatchNodeB1;
	ax::SpriteBatchNode* _blendingBatchNodeT1;
	ax::SpriteBatchNode* _blendingBatchNodeT2;
	ax::SpriteBatchNode* _blendingBatchNodeT3;

	ax::SpriteBatchNode* _main2BatchNode;
	ax::SpriteBatchNode* _glowBatchNode;
	ax::ParticleBatchNode* _particleBatchNode;

	float _lastObjXPos;
	LevelSettings _levelSettings;
	int _prevSection, _nextSection;

	EffectManager* _effectManager;

public:

	GJGameLevel* _level;
	std::vector<GameObject*> _allObjects;
	std::vector<std::vector<GameObject*>> _sectionObjects;
	std::unordered_map<int, SpriteColor, my_string_hash> _colorChannels, _originalColors;
	std::unordered_map<int, GroupProperties, my_string_hash> _groups;

	PlayerObject* _player1, _player2;

protected:
	void loadLevel();
	void initBatchNodes();
	void createObjectsFromSetup(std::string_view uncompressedLevelString);
	void setupLevel(std::string_view uncompressedLevelString);
	void addObject(GameObject* obj);
	void loadLevelData(std::string_view data);
	void fillColorChannel(std::span<std::string_view> colorString, int id);
	void processMoveActionsStep(float dt);
	void processMoveActions(float dt);
	ax::Color3B getLightBG(ax::Color3B bg, ax::Color3B p1);
public:
	static BaseGameLayer* create(GJGameLevel*);
	bool init(GJGameLevel*);
	static int sectionForPos(float x);
	static BaseGameLayer* getInstance() {return _instance;}
	void runMoveCommand(float duration, ax::Point offsetPos, int easeType, float easeAmt, int groupID);
};
