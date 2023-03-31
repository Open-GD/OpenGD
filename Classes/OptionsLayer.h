#pragma once
#include <axmol.h>
#include "DropDownLayer.h"

class OptionsLayer : public ax::Layer {
private:
	bool init();
public:
	static OptionsLayer* create();
};