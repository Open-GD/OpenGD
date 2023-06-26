/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#pragma once

#include "PopupLayer.h"
// #include "RewardsLayer.h"
#include "RewardUnlockLayer.h"

class RewardsPage;

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

class RewardsPage : public PopupLayer
{
private:
	ax::Label* _dailyChestTimer1;
	ax::Label* _dailyChestTimer2;

	ax::Sprite* _chestSprite1;
	ax::Sprite* _chestSprite2;

	RewardUnlockLayer* _unlockLayer;
public:
	static RewardsPage* create();

	bool init();
	void onChestClicked(int chestID);
	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);
	void sendHttpRequest();
};