#pragma once
#include <axmol.h>
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "ListLayer.h"

class DropDownLayer : ax::LayerColor {
    private:
        bool init(const char* label);
    public:
        ax::Layer* droplayer;

        static DropDownLayer* create(const char* label);
        void showLayer();
        void hideLayer();
};