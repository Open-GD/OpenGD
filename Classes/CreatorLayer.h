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
#ifdef AX_PLATFORM_PC
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
#endif
    static CreatorLayer* getInstance();

private:
    static CreatorLayer* Instance;
    ax::ui::TextField* _levelField;
};
