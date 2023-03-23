#pragma once
#include "GameObject.h"
#include <axmol.h>

class EndPortalObject : public GameObject
{
	ax::Sprite* _gradientBar;
public:
	static EndPortalObject* create();
	bool init();
};