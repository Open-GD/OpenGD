#pragma once
#include <axmol.h>
#include "GJGameLevel.h"
#include "GameObject.h"
#include "GameToolbox.h"
#include "external/benchmark.h"
#include "PlayerObject.h"
#include "SpriteColor.h"
#include <span>

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
	std::vector<GameObject*> _objects;
	float _alpha = 1.f;
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

public:

	GJGameLevel* _level;
    std::vector<GameObject*> _allObjects;
	std::vector<std::vector<GameObject*>> _sectionObjects;
	std::unordered_map<int, SpriteColor> _colorChannels, _originalColors;
	std::unordered_map<int, GroupProperties> _groups;

	PlayerObject* _player1, _player2;

protected:
    void loadLevel();
    void initBatchNodes();
    void createObjectsFromSetup(std::string_view uncompressedLevelString);
	void setupLevel(std::string_view uncompressedLevelString);
    void addObject(GameObject* obj);
    void loadLevelData(std::string_view data);
	void fillColorChannel(std::span<std::string_view> colorString, int id);
public:
    static BaseGameLayer* create(GJGameLevel*);
    bool init(GJGameLevel*);
	static int sectionForPos(float x);
	static BaseGameLayer* getInstance() {return _instance;}
	bool isObjectBlending(GameObject* obj);
};
