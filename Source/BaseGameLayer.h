#pragma once
#include <axmol.h>
#include "GJGameLevel.h"
#include "GameObject.h"
#include "GameToolbox.h"

//follow structure
//1. private members
//2. public members
//3. private functions
//4. public functions

class BaseGameLayer : public ax::Layer {
private:

public:
	GJGameLevel* _level;

private:

public:
    static BaseGameLayer* create(GJGameLevel*);
    bool init(GJGameLevel*);
};
