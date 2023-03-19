#pragma once
#include <axmol.h>
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "ListLayer.h"

class DropDownLayer : public ax::LayerColor {
    private:
        ax::Layer* droplayer;
        
        bool init(ax::Layer* scrollLayer, const char* label);
    public:
        static DropDownLayer* create(ax::Layer* scrollLayer, const char* label);
        void showLayer();
        void hideLayer();
};