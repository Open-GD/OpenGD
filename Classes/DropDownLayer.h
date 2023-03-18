#pragma once
#include <axmol.h>
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"

class DropDownLayer : ax::LayerColor {
    private:
        ax::Layer* droplayer;
        
        bool init();
    public:
        static DropDownLayer* create();
        void showLayer();
        void hideLayer();
};