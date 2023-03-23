#pragma once
#include <axmol.h>
#include "SimplePlayer.h"
#include <ui/CocosGUI.h>

class GarageLayer : public ax::Scene {
public:
    static ax::Scene* scene();
    static GarageLayer* create();
    bool init();
    void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
    void setupIconSelect();
    const char* getSpriteName(int id, bool actived);
    void setupPage(IconType modo);
    void createStat(const char* sprite, const char* statKey);

private:
    bool _ctrlDown;
    SimplePlayer* _iconPrev;
    ax::ui::TextField* _userNameField;
    ax::Menu* menuIcons;
    ax::Sprite* m_pSelect;
    int numPerRow = 12;
    int numPerColumn = 3;
    int stats = 0;
    IconType selectedMode;
};
