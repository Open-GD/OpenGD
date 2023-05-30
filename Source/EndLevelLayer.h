#pragma once

#include <string_view>

#include "2d/CCLayer.h"

class DropDownLayer;
class PlayLayer;


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
	static std::string_view getRandomEndingString();

	bool init(PlayLayer *pl);
};