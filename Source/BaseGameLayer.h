#pragma once
#include <axmol.h>
#include "GJGameLevel.h"
#include "GameObject.h"
#include "GameToolbox.h"
#include "external/benchmark.h"

//follow structure
//1. private members
//2. public members
//3. private functions
//4. public functions

class BaseGameLayer : public ax::Layer {
private:
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
public:
	GJGameLevel* _level;
    std::vector<GameObject*> _allObjects;

private:
    void loadLevel();
    void initBatchNodes();
    void createObjectsFromSetup(std::string_view uncompressedLevelString);
    void addObject(GameObject* obj);
    void loadLevelData(std::string_view data);
public:
    static BaseGameLayer* create(GJGameLevel*);
    bool init(GJGameLevel*);
};
