#pragma once
#include <axmol.h>
#include "DropDownLayer.h"
#include "PlayLayer.h"

class EndLevelLayer : public ax::Layer {
private:
    DropDownLayer *_statsLayer;
    PlayLayer *_playlayer;
    bool _createdWithoutPlaylayer;

    int _jumps;
    int _attempts;
    int _time;
    bool _everyplay_included;
public:

	static EndLevelLayer* create(PlayLayer *pl);
    static EndLevelLayer* create();
	static std::string_view getRandomEndingString();

    bool init(PlayLayer *pl);
};