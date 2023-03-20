#pragma once
#include <axmol.h>
#include "DropDownLayer.h"
#include "PromoItemSprite.h"

class MoreGamesLayer : public DropDownLayer {
private:
    bool init();
public:
    static MoreGamesLayer* create();
};