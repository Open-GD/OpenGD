#pragma once
#include <axmol.h>
#include "GJGameLevel.h"
#include "ui/UIWidget.h"

class LevelCell : public ax::ui::Widget
{
public:
    static LevelCell* create(GJGameLevel* level);
    bool init(GJGameLevel* level);
};