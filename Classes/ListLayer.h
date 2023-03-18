#pragma once
#include <axmol.h>
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"

class ListLayer : public ax::LayerColor {
    private:
        ax::Layer* listlayer;
        
        bool init(const char* label, ax::Color4B color, ax::Vec2 pos);
    public:
        static ListLayer* create(const char* label, ax::Color4B color, ax::Vec2 pos);
};