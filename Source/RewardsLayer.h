#pragma once

#include "PopupLayer.h"
#include "RewardsLayer.h"
#include "RewardUnlockLayer.h"

class RewardsLayer;

namespace ax 
{ 
	class Node;
	class Scene;
	class Object;
	class Label;
	class Sprite;
	namespace network 
	{ 
		class HttpRequest; 
		class HttpClient;
		class HttpResponse;
	} 
}

class RewardsLayer : public PopupLayer
{
private:
	ax::Label* _dailyChestTimer1;
	ax::Label* _dailyChestTimer2;

	ax::Sprite* _chestSprite1;
	ax::Sprite* _chestSprite2;

	RewardUnlockLayer* _unlockLayer;
public:
	static RewardsLayer* create();

	bool init();
	void onChestClicked(int chestID);
	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);
	void sendHttpRequest();
};