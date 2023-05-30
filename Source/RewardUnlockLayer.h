#pragma once

#include "PopupLayer.h"
#include "RewardUnlockLayer.h"

class RewardUnlockLayer;

namespace ax 
{ 
	class Node;
	class Scene;
	class Object;
	class Label;
}

class RewardUnlockLayer : public PopupLayer
{
public:
	static RewardUnlockLayer* create(int chestID);

	bool init(int chestID);
};