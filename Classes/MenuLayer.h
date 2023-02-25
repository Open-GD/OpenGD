#pragma once

#include <axmol.h>
class MenuLayer : public ax::Layer {
public:
    static ax::Scene* scene();
    bool init();
    CREATE_FUNC(MenuLayer);
};