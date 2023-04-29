#pragma once
#include "GameObject.h"

namespace ax 
{ 
	class Sprite; 
}


class EndPortalObject : public GameObject
{
	ax::Sprite* _gradientBar;
public:
	static EndPortalObject* create();
	bool init();
};