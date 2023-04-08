#pragma once
#include <axmol.h>
#include "DropDownLayer.h"
#include "PlayLayer.h"

class LevelEndStatsLayer : public ax::Layer {
private:
    DropDownLayer *_statsLayer;
    PlayLayer *_playlayer;
    bool _createdWithoutPlaylayer;

    int _jumps;
    int _attempts;
    int _time;
    bool _everyplay_included;
public:

	static LevelEndStatsLayer* create(PlayLayer *pl);
    static LevelEndStatsLayer* create();

    bool init(PlayLayer *pl);
};