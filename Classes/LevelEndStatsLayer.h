#pragma once
#include <axmol.h>
#include "DropDownLayer.h"
#include "PlayLayer.h"

class LevelEndStatsLayer : public ax::Layer {
private:
    DropDownLayer *_statsLayer;
    PlayLayer *_playlayer;
public:
	static LevelEndStatsLayer* create(PlayLayer *pl);

    bool init(PlayLayer *pl);
};