#pragma once
#include "BaseGameLayer.h"
#include "math/Vec2.h"
#include "CCEventKeyboard.h"

class GJGameLevel;

namespace ax 
{ 
    class Sprite; 
    class Scene;
    class Event;
}



//follow structure
//1. private members
//2. public members
//3. private functions
//4. public functions

class LevelDebugLayer : public BaseGameLayer {
private:
    ax::Vec2 _camInput;
    float _camSpeed = 420.f;

    ax::Sprite* _BG;

    bool _showDebugMenu = false;

public:
    static LevelDebugLayer* create(GJGameLevel*);
    static ax::Scene* scene(GJGameLevel*);
    void onEnter() override;
    void onExit() override;
    bool init(GJGameLevel*);
    void exit();
    void update(float delta);
    void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
    void updateVisibility();
    void updateTriggers(float dt);
    void onDrawImgui();
};
