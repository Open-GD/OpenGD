#pragma once
#include <axmol.h>
#include <network/HttpClient.h>
#include <ui/CocosGUI.h>

#ifdef AX_PLATFORM_PC
#include <glfw3.h>
#endif

class CreatorLayer : public ax::Scene {
public:
    static ax::Scene* scene();
    static CreatorLayer* create();
    bool init();
    
    void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);

private:
    ax::ui::TextField* _levelField;
};
