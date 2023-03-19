#pragma once

#include "DropDownLayer.h"
#include <axmol.h>

class GJMoreGamesLayer : public ax::Layer {
private:
    DropDownLayer *m_pL;
public:
    // Currently uses 2.1 thing
    bool init();

    CREATE_FUNC(GJMoreGamesLayer);
};