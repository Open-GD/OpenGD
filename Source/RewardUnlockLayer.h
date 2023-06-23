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
#include "RewardUnlockLayer.h"
#include "GetGJRewards.h"
#include "GJChestSprite.h"

class RewardUnlockLayer;
class MenuItemSpriteExtra;

namespace ax 
{ 
	class Node;
	class Scene;
	class Object;
	class Label;
	class Vec2;
	namespace network 
	{ 
		class HttpRequest; 
		class HttpClient;
		class HttpResponse;
	} 
}

class RewardUnlockLayer : public PopupLayer
{
private:
	GJChestSprite* _chestObj;
	MenuItemSpriteExtra* _rewardBtn;
	MenuItemSpriteExtra* _closeBtn;
	ax::Label* _errorMsg;
public:
	static RewardUnlockLayer* create(int chestID);

	bool init(int chestID);
	void playRewardEffect(GetGJRewards* rewards);
	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response, int chestID);
	void sendHttpRequest(int chestID);
	void showEarnedCurrency(int currencyID, int currencyCount, float delay, ax::Vec2 position);
};