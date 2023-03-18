#pragma once
#include <axmol.h>
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "ListLayer.h"

class DropDownLayer : ax::LayerColor {
    private:
        ax::Layer* droplayer;
        
        bool init(const char* label);
    public:
        static DropDownLayer* create(const char* label);
        void showLayer();
        void hideLayer();
};