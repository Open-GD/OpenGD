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
    ax::Vec2 _camInput;
    float _camSpeed = 30.f;

public:
	BaseGameLayer* _bgl;

public:
    static LevelDebugLayer* create(GJGameLevel*);
    static ax::Scene* scene(GJGameLevel*);
    void onEnter() override;
    bool init(GJGameLevel*);
    void exit();
    void update(float delta);
    void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
};
