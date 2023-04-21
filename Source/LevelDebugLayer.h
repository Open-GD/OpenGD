#pragma once
#include <axmol.h>

#include "BaseGameLayer.h"


//follow structure
//1. private members
//2. public members
//3. private functions
//4. public functions

class LevelDebugLayer : public ax::Layer {
private:

public:
	BaseGameLayer* _bgl;

private:

public:
    static LevelDebugLayer* create(GJGameLevel*);
    static ax::Scene* scene(GJGameLevel*);
    bool init(GJGameLevel*);
    void exit();
};
