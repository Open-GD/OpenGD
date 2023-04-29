#pragma once

#include "2d/CCLayer.h"


class OptionsLayer : public ax::Layer {
private:
	bool init();
public:
	static OptionsLayer* create();
};