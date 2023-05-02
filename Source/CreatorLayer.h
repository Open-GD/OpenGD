#pragma once
#include <axmol.h>
#include <ui/CocosGUI.h>

class CreatorLayer : public ax::Scene {
public:
    static ax::Scene* scene();
    static CreatorLayer* create();
    bool init();
    
    void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);

private:
    ax::ui::TextField* _levelField;
};
