#pragma once

#include "PopupLayer.h"
#include "RewardsLayer.h"

class RewardsLayer;

namespace ax 
{ 
	class Node;
	class Scene;
	class Object;
	class Label;
}

class RewardsLayer : public PopupLayer
{
private:
	ax::Label* _dailyChestTimer1;
	ax::Label* _dailyChestTimer2;
public:
	static RewardsLayer* create();

	bool init();
	void onChestClicked(int chestID);
};