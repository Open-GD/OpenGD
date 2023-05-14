#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <span>

#include "PlayerObject.h"
#include "SpriteColor.h"
#include "2d/CCLayer.h"

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

    std::vector<GameObject*> _allObjects;
	std::vector<std::vector<GameObject*>> _sectionObjects;
	std::unordered_map<int, SpriteColor> _colorChannels, _originalColors;
	std::unordered_map<int, GroupProperties> _groups;

	PlayerObject* _player1, *_player2;

protected:
    virtual void loadLevel();
    virtual void initBatchNodes();
    virtual void createObjectsFromSetup(std::string_view uncompressedLevelString);
	virtual void setupLevel(std::string_view uncompressedLevelString);
    virtual void addObject(GameObject* obj);
    virtual void loadLevelData(std::string_view data);
	virtual void fillColorChannel(std::span<std::string_view> colorString, int id);
public:
    AX_SYNTHESIZE(GJGameLevel*, _level, Level);

    static BaseGameLayer* create(GJGameLevel*);
    virtual bool init(GJGameLevel*);
	static int sectionForPos(float x);
	static BaseGameLayer* getInstance() {return _instance;}
	virtual bool isObjectBlending(GameObject* obj);
};
